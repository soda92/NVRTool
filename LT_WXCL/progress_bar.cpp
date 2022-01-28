#include "stdafx.h"
#include "progress_bar.h"
#include <thread>
#include <boost/process.hpp>
#include <boost/process/start_dir.hpp>
#include "http_get.h"

#include "config.h"


namespace progress_bar {

    namespace bp = boost::process;

    void show_impl() {
        auto res = http_get("/show", 55555);
    }

    void hide_impl() {
        auto res = http_get("/hide", 55555);
    }

    void show() {
        std::thread(show_impl).detach();
    }

    void hide() {
        std::thread(hide_impl).detach();
    }
}
