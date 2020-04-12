
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <SDL2/SDL.h>

#include "win_SDL2_init.h"
#include "../img/bmp.h"
#include "../img/tga.h"
#include "../img/jpeg.h"

using namespace std;

mainApp_c::mainApp_c() {
	wnd_width = 640;
	wnd_height = 480;

	wnd_posx = SDL_WINDOWPOS_CENTERED;
	wnd_posy = SDL_WINDOWPOS_CENTERED;
	
	is_run = true;
}

mainApp_c::~mainApp_c() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void mainApp_c::init_app() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "mainApp_c::init_app(): Error - Unable to init SDL; " << SDL_GetError() << std::endl;
		exit(1);
	}

	background.from_file("test.jpg");
	cout << background.get_widht() << " " << background.get_height() << " " << background.get_chanels_count() << "\n";

	wnd_width = background.get_widht();
	wnd_height = background.get_height();

	window = SDL_CreateWindow("Cube", wnd_posx, wnd_posy, 
							wnd_width, wnd_height, 
							SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if(window == NULL) {
		std::cout << "mainApp_c::init_app(): Error - Unable to creaate window!" << std::endl;
		exit(1);
	}
	
	// В этом месте просто заливка красным цветом
	/*
	my_canvas = SDL_CreateRGBSurface(0,wnd_width,wnd_height,32,0,0,0,0);
	SDL_FillRect(my_canvas, NULL, SDL_MapRGB(my_canvas->format,255,0,0));
	*/
	my_canvas = SDL_CreateRGBSurfaceFrom(background.get_data(),wnd_width,wnd_height,
										 background.get_chanels_count()*8,
										 background.get_chanels_count()*background.get_widht(),
										 0,0,0,0);
	
	screen = SDL_GetWindowSurface(window);
	SDL_BlitSurface(my_canvas, NULL, screen, NULL);
	
	SDL_UpdateWindowSurface(window);
	
	SDL_FreeSurface(my_canvas);
}

void mainApp_c::looper() {
	while(is_run) {
		SDL_Event event; 
		
		while (SDL_PollEvent(&event)) {
			switch(event.type) { 
				case SDL_QUIT: 
					is_run = false;
					break;

				case SDL_KEYDOWN: 
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE: 
						is_run = false; 
						break;
					}
				break;
			}
		}
		
		/* It's RENDER TIME*/
		
	}
}

void mainApp_c::render() {

}

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