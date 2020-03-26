
/* Класс таймера. Выставляет click в true с заданным интервалом
   в миллисекундах. Время берётся из timespec переводом из
   целых секунд и наносекунд в миллисекунды (большая точность ни к чему)
*/

#ifndef __timer_h_
#define __timer_h_

#include <time.h>
#include <unistd.h>
#include <signal.h>

enum time_units {MINUTES, SECONDS, MILSEC};

class timer_c {
    private:
        bool        click;
        uint64_t    click_res; // in milliseconds (ms) 
        uint64_t    now_time;
        uint64_t    prev_time;

        uint64_t timespec_to_milsec(const struct timespec &time);
        void swap_time();

    public:
        timer_c(const uint64_t clk);
        ~timer_c();

        void set_click_res(const uint64_t clk);
        bool get_click();
        void update();

        void show_timespec(const struct timespec &time);
        void show_uptime(time_units time); 
};

#endif