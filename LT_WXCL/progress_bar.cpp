#include "stdafx.h"
#include "progress_bar.h"
#include <thread>
#include <boost/process.hpp>
#include <boost/process/start_dir.hpp>
#include <string>
#include "config.h"


namespace progress_bar {

    namespace bp = boost::process;

    void show_impl() {
        bp::system("client.exe", "show", bp::start_dir(config::start_dir));
    }

    void hide_impl() {
        bp::system("client.exe", "hide", bp::start_dir(config::start_dir));
    }

    void show() {
        std::thread(show_impl).detach();
    }

    void hide() {
        std::thread(hide_impl).detach();
    }
}
