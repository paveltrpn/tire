
#include <iostream>
#include <SDL2/SDL.h>

#include "mainApp.h"

int WinMain() {
	mainApp_c app;

	app.init_app();

	app.looper();

	return 0;
}

int main(int argc, char* argv[]) 
{
    return WinMain();
}
