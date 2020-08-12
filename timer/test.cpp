
#include <iostream>
#include "timer.h"

int main(int argc, char **argv) {
    timer_c timer;

    timer.show_timer_stats();

    while (1) {
        sleep(2);

        std::cout << timer.get_delta_millisec() << "\n";
    }

    return 0;
}