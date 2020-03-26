
#include <iostream>
#include "timer.h"

int main(int argc, char **argv) {
    timer_c timer(2000);

    timer.show_uptime(SECONDS);

    while (1) {
        timer.update();

        if (timer.get_click()) {
            std::cout << "click!!! \n";
            timer.show_uptime(SECONDS);
        }
    }

    return 0;
}