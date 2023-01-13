
#ifndef __win_SDL2_init_h__
#define __win_SDL2_init_h__

#include <iostream>
#include <string>
#include "SDL.h"

#include "bmp.h"
#include "tga.h"
#include "jpeg.h"

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

#endif
