#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>
using namespace std;

typedef unsigned char UCHAR;
typedef struct _TAXTIME
{
	unsigned short Year;
	unsigned char Month;
	unsigned char Day;
	unsigned char Hour;
	unsigned char Minute;
	unsigned char Second;
} TAXTIME, *pTAXTIME;

typedef struct _TAXDATA
{
	TAXTIME TAXTime;							  //时间
	unsigned char FactRoute;					  //实际交路号
	unsigned short ValvePressure;				  //阀缸压力
	unsigned short DieselSpeedAndOriginalCurrent; //柴油机转速、原边电流。
	unsigned short Speed;						  //速度
	unsigned short MaxSpeed;					  //限速
	unsigned char TrainSign;					  //机车信号
	unsigned char TrainState;					  //机车工况
	unsigned int SignNo;						  //信号机编号
	unsigned char SignType;						  //信号机种类
	unsigned int Signpost;						  //公里标
	unsigned short CarWeight;					  //总重
	unsigned short CarLong;						  //计长
	unsigned char CarCount;						  //辆数
	unsigned char TrainFlag;					  //客货、本补标志
	unsigned int TrainNum;						  //车次
	unsigned char SectionNo;					  //区段号
	unsigned char StationNo;					  //车站号
	unsigned int DriverNo;						  //司机号
	unsigned int CopilotNo;						  //副司机号
	unsigned int EngineNo;						  //机车号
	unsigned char EngineType;					  //机车型号
	unsigned char DevicePlace;					  //设备位置
	unsigned short PipePressure;				  //管压
	unsigned char TAXDataBuf[72];				  // TAX数组

	// 车次种类标识符
	unsigned char TrainTypeId[4];

} TAXDATA, *pTAXDATA;

// 从szBuf解析 TaxData 数据
int AnalyseTax(UCHAR *DataBuf, TAXDATA *TaxData)
{
	UCHAR Tax40_Check, Tax32_Check, Tax72_Check;
	int i, k, m;
	i = 0, m = 0;
	Tax40_Check = 0;
	for (i = 0; i < 184; i++)
	{
		if ((DataBuf[i] == 0x38) && (DataBuf[i + 32] == 0x39)) // 72字节
		{
			Tax32_Check = 0;
			k = 0;
			for (k = 0; k < 31; k++)
				Tax32_Check = Tax32_Check + DataBuf[k + i];
			Tax32_Check = ~Tax32_Check + 1;
			Tax72_Check = 0;
			k = 0;
			for (k = 0; k < 39; k++)
				Tax72_Check = Tax72_Check + DataBuf[k + i + 32];
			Tax72_Check = ~Tax72_Check + 1;
			if ((Tax32_Check == DataBuf[i + 31]) && (Tax72_Check == DataBuf[i + 71]))
			{
				//实际交路号
				TaxData->FactRoute = DataBuf[15 + i];
				//阀缸压力
				TaxData->ValvePressure = DataBuf[17 + i] * 256 + DataBuf[16 + i];
				//柴油机转速、原边电流。
				TaxData->DieselSpeedAndOriginalCurrent = DataBuf[i + 20] * 256 + DataBuf[i + 19];
				//车次
				TaxData->TrainNum = DataBuf[30 + i] * 65536 + DataBuf[29 + i] * 256 + DataBuf[28 + i];

				// 车次种类标识符
				memcpy_s(TaxData->TrainTypeId, 4,
						 &DataBuf[i + 6], 4);

				//运行的时间
				TaxData->TAXTime.Second = DataBuf[35 + i] & 0x3f;
				TaxData->TAXTime.Minute = (DataBuf[36 + i] & 0x0f) * 4 + ((DataBuf[35 + i] & 0xc0) >> 6);
				TaxData->TAXTime.Hour = (DataBuf[37 + i] & 0x01) * 16 + ((DataBuf[36 + i] & 0xf0) >> 4);
				TaxData->TAXTime.Day = (DataBuf[37 + i] & 0x3e) / 2;
				TaxData->TAXTime.Month = (DataBuf[38 + i] & 0x03) * 4 + ((DataBuf[37 + i] & 0xc0) >> 6);
				TaxData->TAXTime.Year = ((DataBuf[38 + i] & 0xfc) >> 2) + 2000;
				//速度、限速
				TaxData->Speed = (DataBuf[40 + i] & 0x02) * 256 + DataBuf[39 + i];
				TaxData->MaxSpeed = (DataBuf[41 + i] & 0x0f) * 64 + (DataBuf[40 + i] >> 2);

				//机车信号
				TaxData->TrainSign = DataBuf[42 + i];
				//机车工况
				TaxData->TrainState = DataBuf[43 + i];

				//信号机编号
				TaxData->SignNo = DataBuf[45 + i] * 256 + DataBuf[44 + i];
				//信号机种类
				TaxData->SignType = DataBuf[46 + i] & 0x07;
				//公里标
				TaxData->Signpost = (DataBuf[49 + i] & 0x003F) * 65536 + DataBuf[48 + i] * 256 + DataBuf[47 + i];
				//总重
				TaxData->CarWeight = DataBuf[51 + i] * 256 + DataBuf[50 + i];
				//计长
				TaxData->CarLong = DataBuf[53 + i] * 256 + DataBuf[52 + i];
				//辆数
				TaxData->CarCount = DataBuf[54 + i];
				//客/货、本/补 标志
				TaxData->TrainFlag = DataBuf[55 + i];
				//区段号
				TaxData->SectionNo = DataBuf[58 + i];
				//车站号
				TaxData->StationNo = DataBuf[59 + i];
				//司机号
				TaxData->DriverNo = DataBuf[10 + i] * 65536 + DataBuf[61 + i] * 256 + DataBuf[60 + i];
				//副司机号
				TaxData->CopilotNo = DataBuf[11 + i] * 65536 + DataBuf[63 + i] * 256 + DataBuf[62 + i];
				//机车号
				TaxData->EngineNo = DataBuf[65 + i] * 256 + DataBuf[64 + i];
				//机车型号
				TaxData->EngineType = (DataBuf[14 + i] & 0x3E) * 256 + DataBuf[66 + i];
				//设备位置 80 A节 40 B节
				TaxData->DevicePlace = DataBuf[14 + i] & 0xC0;
				//管压
				TaxData->PipePressure = (DataBuf[68 + i] & 0x03) * 256 + DataBuf[67 + i];
				memset(TaxData->TAXDataBuf, 0, sizeof(TaxData->TAXDataBuf));
				for (m = 0; m < 72; m++)
				{
					TaxData->TAXDataBuf[m] = DataBuf[i + m];
				}
				return 72;
			}
		}
	}
	i = 0;
	for (i = 0; i < 184; i++)
	{
		if (DataBuf[i] == 0x39) // 40字节
		{

			Tax40_Check = 0;
			k = 0;
			for (k = 0; k < 39; k++)
				Tax40_Check = Tax40_Check + DataBuf[k + i];
			Tax40_Check = ~Tax40_Check + 1;
			if (Tax40_Check == DataBuf[i + 39])
			{
				//运行的时间
				TaxData->TAXTime.Second = DataBuf[i + 3] & 0x3f;
				TaxData->TAXTime.Minute = (DataBuf[i + 4] & 0x0f) * 4 + ((DataBuf[i + 3] & 0xc0) >> 6);
				TaxData->TAXTime.Hour = (DataBuf[i + 5] & 0x01) * 16 + ((DataBuf[i + 4] & 0xf0) >> 4);
				TaxData->TAXTime.Day = (DataBuf[i + 5] & 0x3e) / 2;
				TaxData->TAXTime.Month = (DataBuf[i + 6] & 0x03) * 4 + ((DataBuf[i + 5] & 0xc0) >> 6);
				TaxData->TAXTime.Year = ((DataBuf[i + 6] & 0xfc) >> 2) + 2000;
				//速度、限速
				TaxData->Speed = (DataBuf[i + 8] & 0x03) * 256 + DataBuf[i + 7];
				TaxData->MaxSpeed = (DataBuf[i + 9] & 0x0f) * 64 + (DataBuf[i + 8] >> 2);
				//信号机编号
				TaxData->SignNo = DataBuf[i + 13] * 256 + DataBuf[i + 12];
				//信号机种类
				TaxData->SignType = DataBuf[i + 14] & 0x07;
				//公里标
				TaxData->Signpost = DataBuf[i + 17] * 65536 + DataBuf[i + 16] * 256 + DataBuf[i + 15];
				//总重
				TaxData->CarWeight = DataBuf[i + 19] * 256 + DataBuf[i + 18];
				//计长
				TaxData->CarLong = DataBuf[i + 21] * 256 + DataBuf[i + 20];
				//辆数
				TaxData->CarCount = DataBuf[i + 22];
				//客/货、本/补 标志
				TaxData->TrainFlag = DataBuf[i + 23];
				//车次
				TaxData->TrainNum = ((DataBuf[i + 23] & 0x40) >> 6) * 65536 + DataBuf[i + 25] * 256 + DataBuf[i + 24];
				//区段号
				TaxData->SectionNo = DataBuf[i + 26];
				//车站号
				TaxData->StationNo = DataBuf[i + 27];
				//司机号
				TaxData->DriverNo = DataBuf[i + 28] * 256 + DataBuf[i + 29];
				//副司机号
				TaxData->CopilotNo = DataBuf[i + 30] * 256 + DataBuf[i + 31];
				//机车号
				TaxData->EngineNo = DataBuf[i + 33] * 256 + DataBuf[i + 32];
				//机车型号
				TaxData->EngineType = DataBuf[i + 34];
				//管压
				TaxData->PipePressure = (DataBuf[i + 36] & 0x03) * 256 + DataBuf[i + 35];
				memset(TaxData->TAXDataBuf, 0, sizeof(TaxData->TAXDataBuf));
				for (m = 0; m < 40; m++)
				{
					TaxData->TAXDataBuf[m] = DataBuf[i + m];
				}
				return 40;
			}
		}
	}
	return 0;
}

#include <boost/asio.hpp>
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

io_service ioc;
serial_port port(ioc);

TAXDATA get_tax_data()
{
	port.set_option(boost::asio::serial_port_base::baud_rate(28800));
	std::array<uint8_t, 72> data;
	data.fill(0xFF);
	boost::system::error_code ec;
	boost::asio::read(port, buffer(data), boost::asio::transfer_at_least(1), ec);
	UCHAR DataBuf[72];
	for (int i = 0; i < data.size(); i++)
	{
		DataBuf[i] = data[i];
	}
	TAXDATA tax_data;
	AnalyseTax(DataBuf, &tax_data);
	return tax_data;
}

// 序列化
void tag_invoke(const value_from_tag &, value &jv, TAXDATA const &c)
{
	jv = {
		{
			"TAXTime",
			{
				{"Year", c.TAXTime.Year},
				{"Month", c.TAXTime.Month},
				{"Day", c.TAXTime.Day},
				{"Hour", c.TAXTime.Hour},
				{"Minute", c.TAXTime.Minute},
				{"Second", c.TAXTime.Second},
			},
		},
		{"FactRoute", c.FactRoute},
		{"ValvePressure", c.ValvePressure},
		{"DieselSpeedAndOriginalCurrent", c.DieselSpeedAndOriginalCurrent},
		{"Speed", c.Speed},
		{"MaxSpeed", c.MaxSpeed},
		{"TrainSign", c.TrainSign},
		{"TrainState", c.TrainState},
		{"SignNo", c.SignNo},
		{"SignType", c.SignType},
		{"Signpost", c.Signpost},
		{"CarWeight", c.CarWeight},
		{"CarLong", c.CarLong},
		{"CarCount", c.CarCount},
		{"TrainFlag", c.TrainFlag},
		{"TrainNum", c.TrainNum},
		{"SectionNo", c.SectionNo},
		{"StationNo", c.StationNo},
		{"DriverNo", c.DriverNo},
		{"CopilotNo", c.CopilotNo},
		{"EngineNo", c.EngineNo},
		{"EngineType", c.EngineType},
		{"DevicePlace", c.DevicePlace},
		{"PipePressure", c.PipePressure},
		{"TrainTypeId", c.TrainTypeId},
	};
}

bool run_ = true;
TAXDATA tax_data_;

void start_impl()
{
	while (run_)
	{
		auto tmp = get_tax_data();
		if (tmp.Speed < 50000 && tmp.Speed >= 0)
		{
			tax_data_ = tmp;
		}
		std::this_thread::sleep_for(20ms);
	}
}

void start()
{
	port.open("COM1");
	std::thread(start_impl).detach();
}

void stop()
{
	run_ = false;
	port.close();
}

std::string get_tax()
{
	return boost::json::serialize(boost::json::value_from(tax_data_));
}

#include <boost/python.hpp>

BOOST_PYTHON_MODULE(tax_lib)
{
	using namespace boost::python;
	def("start", start);
	def("stop", stop);
	def("get", get_tax);
}
