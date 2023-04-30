
#include "mainApp.h"

int main(int argc, char* argv[]) {
    mainApp_glfw_c app;

    app.init_app(argc, argv);

    app.looper();

    return 0;
}
