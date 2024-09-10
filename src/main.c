// IMPORT SDL
#include <SDL.h>

// IMPORT CLibs
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

/* --------------------------------
* DEFINE STATIC CONSTANTS (macros)
*///-------------------------------
/* Window */
#define WINDOW_TITLE "Snake!"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define SCREEN_AREA WINDOW_WIDTH * WINDOW_HEIGHT

/* ----------
* "OBJECTS"
*///---------
/*  Player */
/*typedef struct SDL_Rect {
	int x, y;
	int w, h;
	int dirX, dirY;
	int speed;
} SDL_Rect;*/

typedef struct Direction {
	int x, y;
} Direction;

/* ---------------
* MAIN FUNCTION
*///--------------
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
				WINDOW_TITLE,
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				WINDOW_WIDTH, WINDOW_HEIGHT,
				0
				);
	if (!main_window)
	{
		printf("Error initializing SDL: %s\n", SDL_GetError());
		return 0;
	}

	/* RENDERER */
	// SDL_CreateRenderer (SDL_Window * window, int index -
	// initialization priority, Unit32 flags - 0 || one or more SDL_RenderFlags)
	SDL_Renderer *rend = SDL_CreateRenderer(main_window, -1, 
				SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (!rend)
	{
		printf("Error initializing SDL: %s\n", SDL_GetError());
		return 0;
	}

	/* RNG MODULE (stdlib.h) */
	srand(time(NULL));  // gen a random seed for rand() - uses <time.h>
	//int rng_ = rand();

	/* "OBJECTS" */
	// SDL struct
	SDL_Rect player;
	player.w = 30, player.h = 30;
	player.x = 150, player.y = 150;

	SDL_Rect food;
	food.w = 28, food.h = 28;
	void spawnNewFood()
	{
		food.x = rand() % WINDOW_WIDTH - food.w; 
		food.y = rand() % WINDOW_HEIGHT - food.h;
	}
	
	spawnNewFood();
	
	// my struct
	Direction dir;
	dir.x = 0, dir.y = 1;
	
	// non-struct
	float player_speed = 1.5;
	int player_score = 0;
	//int *ptr_p_score = &player_score;
	
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
						break;
					case SDLK_UP:
						dir.y = -1;
						//printf("dir.y: %d\n", dir.y);
						dir.x = 0;
						break;
					case SDLK_DOWN:
						dir.y = 1;
						//printf("dir.y: %d\n", dir.y);
						dir.x = 0;
						break;
					case SDLK_LEFT:
						dir.x = -1;
						//printf("dir.x: %d\n", dir.x);
						dir.y = 0;
						break;
					case SDLK_RIGHT:
						dir.x = 1;
						//printf("dir.x: %d\n", dir.x);
						dir.y = 0;
						break;
					default:
						break;
				}
				break;
			default:
				break;
			}
		}

		/* DRAWING ON SCREEN */
		{
			// Background
			SDL_SetRenderDrawColor(rend, 0,0,0,255);  // r, g, b, alpha
			SDL_RenderClear(rend);  // clear screen to selected color

			// FOOD
			SDL_SetRenderDrawColor(rend, 0,255,0,255);
			SDL_RenderFillRect(rend, &food);
			if (player.x == food.x && player.y == food.y)
			{
				spawnNewFood();
				player_score += 1;
				printf("score: %d\n", player_score);
			}

			// Player
			SDL_SetRenderDrawColor(rend, 255,0,0,255);
			SDL_RenderDrawRect(rend, &player);
			if (dir.x != 0)
			{
				//printf("dir.x is != 0.\n");
				switch(dir.x)
				{
					case 1:
						//printf("dir.x is 1\n");
						player.x += player_speed;
						break;
					case -1:
						//printf("dir.x is -1\n");
						player.x -= player_speed;
						break;
					default:
						break; 
				}
			}
			if (dir.y != 0)
			{
				//printf("dir.y is != 0.\n");
				switch(dir.y)
				{
					case 1:
						//printf("dir.y is 1\n");
						player.y += player_speed;
						break;
					case -1:
						//printf("dir.y is -1\n");
						player.y -= player_speed;
						break;
					default:
						break;
				}
			}

			
			/*if (player_score > *ptr_p_score)
			{
				player_speed += 1;
				*ptr_p_score = player_score;
			}*/

			// UI


			// SCREEN UPDATE()
			SDL_RenderPresent(rend);  // display the configured *render
		}
	}
	
	/*-----
	* END
	*///---
	printf("SDL was initialized successfully!\n");
	SDL_Quit();
	return 0;
}