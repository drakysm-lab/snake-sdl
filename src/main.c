#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
	/*-------------------
	* INITIALIZATION
	*///-----------------
	/* VIDEO */
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Error initializing SDL: %s\n", SDL_GetError());
		return 0;
	}
	
	/* WINDOW */
	SDL_Window *main_window = SDL_CreateWindow(
					"Snake!",
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					800, 600,
					0
					);
	if (!main_window)
	{
		printf("Error initializing SDL: %s\n", SDL_GetError());
		return 0;
	}
	
	/* -------------
	* MAIN LOOP
	*///------------
	bool running = true;
	SDL_Event event;
	while (running)
	{
		/* EVENT POLL */
		while (SDL_PollEvent(&event))
		{
			/* GET EVENT/INPUT */
			switch (event.type)
			{
			// WINDOW EVENT:
			case SDL_QUIT:
				running = false; // end main loop;
				break;
			// KEYBOARD INPUT:
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)  // keysym == "key symbol"
				{
					case SDLK_ESCAPE:  // esc;
						running = false; // end main loop (esc);
					default:
						break;
				}
				break;
			default:
				break;
			}
		}
	}
	
	/*-----
	* END
	*///---
	printf("SDL was initialized successfully!\n");
	SDL_Quit();
	return 0;
}