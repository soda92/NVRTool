#include "stdafx.h"
#include "progress_bar.h"
#include <thread>
#include <boost/process.hpp>
#include <boost/process/start_dir.hpp>
#include <string>
#include "config.h"
#include "D:/src/vcpkg/installed/x86-windows/include/httplib.h"


namespace progress_bar {

    namespace bp = boost::process;
    httplib::Client cli("http://localhost:55555");

    void show_impl() {
        auto res = cli.Get("/show");
    }

    void hide_impl() {
        auto res = cli.Get("/hide");
    }

    void show() {
        std::thread(show_impl).detach();
    }

    void hide() {
        std::thread(hide_impl).detach();
    }
}
