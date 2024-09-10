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

/* TIME */
#define CLOCK_TICK 60

/* ----------
* "OBJECTS"
*///---------
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
	// player
	SDL_Rect player;
	player.w = 32, player.h = 32;
	player.x = 0, player.y = 0;
	unsigned int walk_delay = 0;
	unsigned int speed = 30;

	// food
	SDL_Rect food;
	food.w = 32, food.h = 32;
	unsigned int food_blink = 0;
	unsigned int blink_speed = 60;
	
	// my struct
	Direction dir;
	dir.x = 0, dir.y = 0;
	
	// non-struct
	int player_score = 0;
	int base_score = 0;
	
	unsigned int timer = 150;
	unsigned int slapsed_time = 0;
	
	/* -----------
	* FUNCTIONS
	*///----------
	/* SCREEN */
	void clear_screen()
	{
		SDL_SetRenderDrawColor(rend, 0,0,0,255);  // r, g, b, alpha
		SDL_RenderClear(rend);  // clear screen to selected color
	}
	
	void spawnNewFood()
	{
		// rand() -- range syntax -- % (max + 1 - min) + min;
		food.x = rand() % 20*32;
		food.y = rand() % 15*32;
	}
	
	void runTimer()
	{
		slapsed_time += 1;
		if (slapsed_time == 60)
		{
			timer -= 1;
			slapsed_time = 0;
			printf("countdown: %d\n", timer);
		}
	}
	
	spawnNewFood();
	
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
			clear_screen();

			// FOOD
			//.draw
			{
				food_blink += 1;
				
				if (food_blink > 30 && food_blink < blink_speed){
					SDL_SetRenderDrawColor(rend, 0,255,0,255);
					SDL_RenderFillRect(rend, &food);
				} /*else if (food_blink > 45 && food_blink < 60)
				{
					SDL_SetRenderDrawColor(rend, 0,255,0,255);
					SDL_RenderFillRect(rend, &food);
				}*/
				else if (food_blink >= blink_speed) {
					food_blink = 0;
				}
			}
			
			//SDL_SetRenderDrawColor(rend, 0,255,0,255);
			//SDL_RenderFillRect(rend, &food);
			
			//.get collision
			if (player.x == food.x && player.y == food.y)
			{
				spawnNewFood();
				player_score += 1;
				printf("score: %d\n", player_score);
			}

			// Player
			SDL_SetRenderDrawColor(rend, 255,0,0,255);
			SDL_RenderDrawRect(rend, &player);
			
			walk_delay += 1;
			if (walk_delay >= 0 && walk_delay <= 1)
			{
				if (dir.x != 0)
				{
					//printf("dir.x is != 0.\n");
					switch(dir.x)
					{
						case 1:
							//printf("dir.x is 1\n");
							player.x += player.w;
							break;
						case -1:
							//printf("dir.x is -1\n");
							player.x -= player.w;
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
							player.y += player.w;
							break;
						case -1:
							//printf("dir.y is -1\n");
							player.y -= player.w;
							break;
						default:
							break;
					}
				}
			} else if (walk_delay == speed)
			{
				walk_delay = 0;
			}

			// increase player speed if add player_score;
			if (base_score < player_score)
			{
				base_score += 1;
				if (speed < 5)
				{
					speed -= 2;
				} else if (speed > 5 && speed != 1)
				{
					speed -= 1;
					if (blink_speed >= 24)
					{
						blink_speed -= 1;
					}
				}
			}

			// UI
			runTimer();
			
			// check win
			if (player_score == 50)
			{
				running = false;
				printf("\nYou win!\n\n");
			} else if (timer == 0)
			{
				running = false;
				printf("\nSorry, you loose! :(\n\n");
			}

			// SCREEN UPDATE()
			SDL_RenderPresent(rend);  // display the configured *render
			SDL_Delay(1000/CLOCK_TICK);  // set target fps to 60;
		}
	}
	
	/*-----
	* END
	*///---
	printf("The program did exit successfully!\n");
	SDL_Quit();
	return 0;
}