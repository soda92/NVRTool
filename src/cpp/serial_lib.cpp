#include <boost/asio.hpp>
#include <fmt/core.h>
#include <thread>
#include <chrono>
#include <array>
using namespace std::literals;
using namespace std::chrono_literals;
using namespace std::literals::chrono_literals;
using namespace boost::asio;

#include <boost/json.hpp>

using boost::json::object;
using boost::json::value;
using boost::json::value_from_tag;
using boost::json::value_to;
using boost::json::value_to_tag;
// https://gist.github.com/kaliatech/427d57cb1a8e9a8815894413be337cf9
// https://stackoverflow.com/a/13997758/12291425

io_service ioc;
serial_port port(ioc);

struct detector
{
    std::string type;
    std::string status;
};

std::array<uint8_t, 4> get_data()
{
    port.open("COM2");
    port.set_option(boost::asio::serial_port_base::baud_rate(9600));
    std::array<uint8_t, 5> pre = {0xAA, 0xAA, 0x4F, 0x01, 0x22};
    write(port, buffer(pre));
    std::array<uint8_t, 128> data;
    data.fill(0xFF);
    boost::system::error_code ec;
    boost::asio::read(port, buffer(data), boost::asio::transfer_at_least(1), ec);
    port.close();
    return std::array<uint8_t, 4>{data[6], data[7], data[8], data[9]};
}

detector parse_data(uint8_t data)
{
    uint8_t status = data >> 3;
    uint8_t type = (data & 0b111000) >> 3;
    std::string str_status;
    std::string str_type;
    switch (status)
    {
    case 0b000:
        str_status = "offline"; // 离线
        break;
    case 0b001:
        str_status = "normal"; // 正常
        break;
    case 0b010:
        str_status = "error"; // 故障
        break;
    case 0b011:
        str_status = "polluted"; // 污染
        break;
    case 0b100:
        str_status = "alarm"; // 报警
        break;
    default:
        break;
    }
    switch (type)
    {
    case 0b000:
        str_type = "none"; // 未安装
        break;
    case 0b001:
        str_type = "smoke"; // 烟感
        break;
    case 0b010:
        str_type = "temperature"; // 高温
        break;
    case 0b011:
        str_type = "fire"; // 火焰
        break;
    case 0b100:
        str_type = "fire";
        break;
    case 0b101:
        str_type = "smoke";
        break;
    default:
        str_type = "unknown"; // 未知
        break;
    }
    detector d;
    d.type = str_type;
    d.status = str_status;
    return d;
}

// 序列化
void tag_invoke(const value_from_tag &, value &jv, detector const &c)
{
    jv = {
        {"type", c.type},
        {"status", c.status}};
}

// 反序列化
detector tag_invoke(value_to_tag<detector>, boost::json::value const &jv)
{
    object const &obj = jv.as_object();
    detector d;
    d.type = value_to<std::string>(obj.at("type"));
    d.status = value_to<std::string>(obj.at("status"));
    return d;
}

std::string get_all_data(std::array<uint8_t, 4> data)
{
    std::vector<detector> all_data;
    for (auto d : data)
    {
        all_data.push_back(parse_data(d));
    }
    return boost::json::serialize(boost::json::value_from(all_data));
}

std::string parse2_data()
{
    return get_all_data(get_data());
}

#include <boost/python.hpp>

BOOST_PYTHON_MODULE(serial_lib)
{
    using namespace boost::python;
    def("parse_data", parse2_data);
}
