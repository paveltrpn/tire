
#pragma once

#include <iostream>
#include <string>
#include <SDL2/SDL.h>

#include "image.h"

class mainApp_c {
	private:
		int wnd_height, wnd_width;
		int wnd_posx, wnd_posy;
		
		SDL_Window *window;
		SDL_Surface *screen;
		SDL_Surface *my_canvas;
		
		jpeg_img_c background;
		
		bool is_run;

		void render();

	public:
		~mainApp_c();
		mainApp_c();

		void init_app();
		void looper();
};
