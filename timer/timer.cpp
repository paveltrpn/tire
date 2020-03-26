
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "timer.h"

timer_c::timer_c(uint64_t clk) {
    struct timespec tmp;

    click = false;
    click_res = clk;

    clock_gettime(CLOCK_MONOTONIC,&tmp);
    now_time = prev_time = timespec_to_milsec(tmp);
};

timer_c::~timer_c() {
    
};

void timer_c::set_click_res(const uint64_t clk){
    click_res = clk;
}

bool timer_c::get_click() {
    return click;
};

uint64_t timer_c::timespec_to_milsec(const struct timespec &time) {
    return time.tv_sec*1000 + time.tv_nsec/1000000;
}

void timer_c::update() {
    struct timespec tmp;

    clock_gettime(CLOCK_MONOTONIC,&tmp);
    now_time = timespec_to_milsec(tmp);

    if ((now_time - prev_time) > click_res) {
        click = true;
        prev_time = now_time;
    }   else {
        click = false;
    };
};

void timer_c::show_timespec(const struct timespec &time) {
    std::cout << "time = " << time.tv_sec << "." << time.tv_nsec << "\n";
}

void timer_c::show_uptime(time_units time){
    struct timespec uptime;
    uint64_t ret;

    clock_gettime(CLOCK_MONOTONIC,&uptime);

    switch (time) {
        case MINUTES: {
            ret = timespec_to_milsec(uptime)/60000;
            break;
        }

        case SECONDS: {
            ret = timespec_to_milsec(uptime)/1000;
            break;
        }

        case MILSEC: {
            ret = timespec_to_milsec(uptime);
            break;
        }
    }

    std::cout << "current uptime = " << ret << "\n";
}