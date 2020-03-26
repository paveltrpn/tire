
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "mainApp.h"

int main(int argc, char* argv[]) 
{
    mainApp_c app;

	app.init_app();

	app.looper();

	return 0;
}
