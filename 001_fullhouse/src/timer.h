
#ifndef __timer_h_
#define __timer_h_

#include <time.h>
#include <unistd.h>
#include <utility>
#include <tuple>
#include <signal.h>

// 
// struct timespec {
//      time_t   tv_sec;        /* секунды */
//      long     tv_nsec;       /* наносекунды */
// }; 
//
// CLOCK_REALTIME (системные часы) - значение этих часов содержит время в секундах и наносекундах, истекшее с 0 часов 1 января 1970 года.
//
// CLOCK_REALTIME_COARSE (начиная с Linux 2.6.32; есть только в Linux) - более быстрая, но менее точная версия CLOCK_REALTIME. Используйте, 
// если нужны не очень точные метки времени, но быстро.
// 
// CLOCK_MONOTONIC - часы, которые не могут быть настроены и показывают монотонный ход времени отсчитываемой с некой неопределённой начальной точки. 
// Эти часы не подвержены скачкам системного времени (например, системный администратор вручную изменил время), но на них влияет постепенная подгонка, 
// выполняемая adjtime(3) и NTP. 
//
// CLOCK_MONOTONIC_COARSE (начиная с Linux 2.6.32; есть только в Linux) - более быстрая, но менее точная версия CLOCK_MONOTONIC. 
// Используйте, если нужны не очень точные метки времени, но быстро. 
//
// CLOCK_MONOTONIC_RAW (начиная с Linux 2.6.28; есть только в Linux) - похожи на CLOCK_MONOTONIC, но предоставляют прямой доступ к аппаратным часам, 
// которые не подводятся NTP или постепенной подгонкой, выполняемой adjtime(3).
// 
// CLOCK_BOOTTIME (начиная с Linux 2.6.39; есть только в Linux) - идентичны CLOCK_MONOTONIC, но также содержат любое время, на которое система 
// была приостановлена (suspended). Это позволяет приложениям получить учитывающие приостановку монотонные часы без обращения к сложностям CLOCK_REALTIME, 
// которые могут быть неоднородны, если время изменили с помощью settimeofday(2). 
// 
// CLOCK_PROCESS_CPUTIME_ID (начиная с Linux 2.6.12) - настраиваемые для каждого процесса часы ЦП (измеряют время ЦП, затраченное всеми нитями процесса).
// 
// CLOCK_THREAD_CPUTIME_ID (начиная с Linux 2.6.12) -часы, работающие на ЦП, для каждой нити. 
// 

enum time_units {MINUTES, SECONDS, MILSEC};

class timer_c {
    private:
        timespec    watch_time;
        timespec    now_time;
        timespec    prev_time;

        timespec    clk_res_REALTIME;
        timespec    clk_res_MONOTONIC;
        timespec    clk_start_REALTIME;
        timespec    clk_start_MONOTONIC;

        clockid_t   clock_id = CLOCK_MONOTONIC;

        // Наносекунда 10^-9
        uint64_t timespec_to_nanosec(const struct timespec &time) {
            return time.tv_sec*1000000000 + time.tv_nsec;
        };

        // Микросекунда 10^-6
        uint64_t timespec_to_microsec(const struct timespec &time) {
            return time.tv_sec*1000000 + time.tv_nsec/1000;
        };
        
        // Миллисекунда 10^-3
        uint64_t timespec_to_millisec(const struct timespec &time) {
            return time.tv_sec*1000 + time.tv_nsec/1000000;
        };

        uint64_t timespec_to_seconds(const struct timespec &time) {
            return time.tv_sec;
        };

        uint64_t timespec_to_minutes(const struct timespec &time) {
            return time.tv_sec/60;
        };

        uint64_t timespec_to_hours(const struct timespec &time) {
            return time.tv_sec/3600;
        };

        uint64_t timespec_to_days(const struct timespec &time) {
            return time.tv_sec/(3600*24);
        };

        //  timespec в человекочетаемом формате (ЧЧ:ММ:СС)
        std::tuple<uint32_t, uint32_t, uint32_t> timespec_to_hr(const struct timespec &time) {
            uint64_t total_sec = timespec_to_seconds(time);

            uint32_t hours = total_sec / 3600;
            uint32_t minutes = total_sec / 60 - hours*60;
            uint32_t seconds = total_sec - hours*3600 - minutes*60;

            return {hours, minutes, seconds};
        };

        timer_c(timer_c& other) = delete;
        timer_c& operator=(timer_c& other) = delete;
        
    public:
        timer_c() {
            struct timespec tmp;

            // Определяем разрешающую способность часов для CLOCK_REALTIME 
            clock_getres(CLOCK_REALTIME, &clk_res_REALTIME);

            // Определяем разрешающую способность часов для CLOCK_MONOTONIC 
            clock_getres (CLOCK_MONOTONIC, &clk_res_MONOTONIC);

            // Время отсчёта для CLOCK_REALTIME
            clock_gettime(CLOCK_REALTIME,&clk_start_REALTIME);

            // Время отсчёта для CLOCK_MONOTONIC
            clock_gettime(CLOCK_MONOTONIC,&clk_start_MONOTONIC);

            clock_gettime(clock_id,&tmp);
            now_time = prev_time = watch_time = tmp;
        };

        ~timer_c() {};

        void show_timer_stats() {
            switch(clock_id) {
                case CLOCK_MONOTONIC:
                    std::cout << "timer_c::clock_id = CLOCK_MONOTONIC\n";
                    break;

                case CLOCK_MONOTONIC_COARSE:
                    std::cout << "timer_c::clock_id = CLOCK_MONOTONIC_COARSE\n";
                    break;

                case CLOCK_REALTIME:
                    std::cout << "timer_c::clock_id = CLOCK_REALTIME\n";
                    break;

                 case CLOCK_REALTIME_COARSE:
                    std::cout << "timer_c::clock_id = CLOCK_REALTIME_COARSE\n";
                    break;
            }
        
    
            std::cout << "timer_c::clk_res_REALTIME = " << 
                            timespec_to_nanosec(clk_res_REALTIME) << " ns; " <<
                            timespec_to_microsec(clk_res_REALTIME) << " mks; " <<
                            timespec_to_millisec(clk_res_REALTIME) << " ms; " <<
                            timespec_to_seconds(clk_res_REALTIME) << " sec; " <<
                            timespec_to_minutes(clk_res_REALTIME) << " min; " <<
                            timespec_to_hours(clk_res_REALTIME) << " hrs; " <<
                            timespec_to_days(clk_res_REALTIME) << " days;\n";

            std::cout << "timer_c::clk_res_MONOTONIC = " << 
                            timespec_to_nanosec(clk_res_MONOTONIC) << " ns; " <<
                            timespec_to_microsec(clk_res_MONOTONIC) << " mks; " <<
                            timespec_to_millisec(clk_res_MONOTONIC) << " ms; " <<
                            timespec_to_seconds(clk_res_MONOTONIC) << " sec; " <<
                            timespec_to_minutes(clk_res_MONOTONIC) << " min; " <<
                            timespec_to_hours(clk_res_MONOTONIC) << " hrs; " <<
                            timespec_to_days(clk_res_MONOTONIC) << " days;\n";

            std::cout << "timer_c::clk_start_REALTIME = " << 
                            timespec_to_nanosec(clk_start_REALTIME) << " ns; " <<
                            timespec_to_microsec(clk_start_REALTIME) << " mks; " <<
                            timespec_to_millisec(clk_start_REALTIME) << " ms; " <<
                            timespec_to_seconds(clk_start_REALTIME) << " sec; " <<
                            timespec_to_minutes(clk_start_REALTIME) << " min; " <<
                            timespec_to_hours(clk_start_REALTIME) << " hrs; " <<
                            timespec_to_days(clk_start_REALTIME) << " days;\n";
            auto clk_start_REALTIME_hr = timespec_to_hr(clk_start_REALTIME);
            std::cout << std::get<0>(clk_start_REALTIME_hr) << ":" << std::get<1>(clk_start_REALTIME_hr) << ":" << std::get<2>(clk_start_REALTIME_hr) << "\n";

            std::cout << "timer_c::clk_start_MONOTONIC = " << 
                            timespec_to_nanosec(clk_start_MONOTONIC) << " ns; " <<
                            timespec_to_microsec(clk_start_MONOTONIC) << " mks; " <<
                            timespec_to_millisec(clk_start_MONOTONIC) << " ms; " <<
                            timespec_to_seconds(clk_start_MONOTONIC) << " sec; " <<
                            timespec_to_minutes(clk_start_MONOTONIC) << " min; " <<
                            timespec_to_hours(clk_start_MONOTONIC) << " hrs; " <<
                            timespec_to_days(clk_start_MONOTONIC) << " days;\n";
            auto clk_start_MONOTONIC_hr = timespec_to_hr(clk_start_MONOTONIC);
            std::cout << std::get<0>(clk_start_MONOTONIC_hr) << ":" << std::get<1>(clk_start_MONOTONIC_hr) << ":" << std::get<2>(clk_start_MONOTONIC_hr) << "\n";
        };

        void set_clock_id(clockid_t clk) {
            clock_id = clk;
        };

        // Возвращает время в миллисекундах, соответсвующее clock_id
        uint64_t get_time_millisec() {
            struct timespec tmp;

            clock_gettime(clock_id,&tmp);
        
            return timespec_to_millisec(tmp);
        };

        // Возвращает время в миллисикундах с последнего вызова этой фйнкции или от 
        // момента создания экземпляра класса
        uint64_t get_delta_millisec() {
            uint64_t rt;

            clock_gettime(clock_id,&now_time);

            rt = timespec_to_millisec(now_time) - timespec_to_millisec(prev_time);

            prev_time = now_time;

            return rt;
        };

        std::pair<bool, uint32_t> watch_millisec(uint32_t delta) {
            timespec watch_now;
            uint32_t cdelta;

            clock_gettime(clock_id,&watch_now);
            cdelta = timespec_to_millisec(watch_now) - timespec_to_millisec(watch_time);

            if (cdelta > delta) {
                watch_time = watch_now;
                return {true, cdelta};
            }

            return {false, 0};
        };
};

#endif