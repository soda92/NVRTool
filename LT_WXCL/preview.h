#pragma once
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <sstream>

using boost::asio::ip::tcp;

enum { max_length = 1024 };

void preview(const char* ip, long long hwnd) {
    try
    {
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints =
            resolver.resolve(tcp::v4(), "127.0.0.1", "1234");

        tcp::socket s(io_context);
        boost::asio::connect(s, endpoints);

        std::stringstream ss;
        ss << ip << ' ' << hwnd;
        std::string message = ss.str();
        boost::asio::write(s, boost::asio::buffer(message, message.length()));
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

}
