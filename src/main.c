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
/* TIME/FRAMERATE */
#define CLOCK_TICK 60

/* ---------
* "OBJECTS"
*///--------
typedef struct Direction {
	int x, y;
} Direction;

/* -------------
* MAIN FUNCTION
*///------------
int main(int argc, char *argv[])
{
	/*-----------
	* INITIALIZE
	*///---------
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
	// SDL_CreateRenderer (SDL_Window * window, int index - initialization 
	// priority, Unit32 flags - 0 || one or more SDL_RenderFlags)
	SDL_Renderer *rend = SDL_CreateRenderer(main_window, -1, 
				SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (!rend)
	{
		printf("Error initializing SDL: %s\n", SDL_GetError());
		return 0;
	}

	/* RNG MODULE -> <stdlib.h> */
	srand(time(NULL));  // generate a random seed for rand() -> <time.h>

	/* "OBJECTS" */
	// player //
	SDL_Rect player;
	player.w = 32, player.h = 32;
	player.x = 0, player.y = 0;
	unsigned short walk_delay = 0;
	unsigned short speed = CLOCK_TICK/2;

	// food //
	SDL_Rect food;
	food.w = 32, food.h = 32;
	unsigned short food_blink = 0;
	unsigned short blink_speed = CLOCK_TICK;
	
	// vector2 //
	Direction dir;
	dir.x = 0, dir.y = 0;
	
	// ui //
	unsigned short player_score = 0;
	unsigned short base_score = 0;
	
	unsigned short timer = 155;
	unsigned short elapsed_time = 0;
	
	/* ---------
	* FUNCTIONS
	*///--------
	/* SCREEN */
	void ClearScreen()
	{
		SDL_SetRenderDrawColor(rend, 0,0,0,255);  // r, g, b, alpha
		SDL_RenderClear(rend);  // clear screen to selected color
	}
	
	void SpawnNewFood()
	{
		// rand() -- range syntax -- % (max + 1 - min) + min;
		food.x = rand() % 20*32;
		food.y = rand() % 15*32;
	}
	
	void RunTimer()
	{
		elapsed_time += 1;
		if (elapsed_time == 60)
		{
			timer -= 1;
			elapsed_time = 0;
			printf("countdown: %d\n", timer);
		}
	}
	
	SpawnNewFood();  // places the first food in the game;
	
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
			// get event type //
			switch (event.type)
			{
			// window event //
			case SDL_QUIT:
				running = false; // end main loop;
				break;
			// keyboard input //
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)  // keysym == "key symbol"
				{
				case SDLK_ESCAPE:
					running = false; // end main loop (esc);
					break;
				case SDLK_UP:
					dir.y = -1;  // -y
					dir.x = 0;
					break;
				case SDLK_DOWN:
					dir.y = 1;  // +y
					dir.x = 0;
					break;
				case SDLK_LEFT:
					dir.x = -1;  // -x
					dir.y = 0;
					break;
				case SDLK_RIGHT:
					dir.x = 1;  // +x
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

		/* DRAWING ON THE SCREEN */
		{
			// background //
			ClearScreen();

			// food //
			{
				{ // blink func:
					food_blink += 1;
					
					if (food_blink > 30 && food_blink < blink_speed)
					{
						SDL_SetRenderDrawColor(rend, 0,255,0,255);
						SDL_RenderFillRect(rend, &food);
					} 
					else if (food_blink >= blink_speed) 
					{
						food_blink = 0;
					}
				}
				
				// collision:
				if (player.x == food.x && player.y == food.y)
				{
					SpawnNewFood();
					player_score += 1;
					printf("Score: %d\n", player_score);
				}
			}

			/* PLAYER */
			{ // draw rect
				SDL_SetRenderDrawColor(rend, 255,0,0,255);
				SDL_RenderDrawRect(rend, &player);
				walk_delay += 1;
				
				// frame-update delay
				if (walk_delay >= 0 && walk_delay <= 1)  // draw 1-frame;
				{
					if (dir.x != 0)  // check for horizontal axis;
					{
						switch(dir.x)
						{
						case 1:  // +x
							player.x += player.w;
							break;
						case -1:  // -x
							player.x -= player.w;
							break;
						default:
							break; 
						}
					}
					if (dir.y != 0)  // check for vertical axis;
					{
						switch(dir.y)
						{
						case 1:  // +y
							player.y += player.h;
							break;
						case -1:  // -y
							player.y -= player.h;
							break;
						default:
							break;
						}
					}
				} 
				else if (walk_delay == speed)  // wait for 1s (60f) and reset;
											// changing 'speed' reduces waiting time;
				{
					walk_delay = 0;
				}
			}

			// data proccess //
			// scoring
			if (base_score < player_score)  // increase player speed if 'add player_score';
			{
				base_score += 1;
				
				if (speed < 5)  // check speed level;
				{
					speed -= 3;
				} 
				else if (speed > 5 && speed != 1)  // check speed level and limit;
				{
					speed -= 1;
					
					if (blink_speed >= 24)  // change food's blink speed;
					{
						blink_speed -= 1;
					}
				}
			}
			
			RunTimer();  // load timer
			
			if (player_score == 50)  // check win;
			{
				running = false;
				printf("\nGreat! You win!\n\n");
			} 
			else if (timer == 0)  // check loss;
			{
				running = false;
				printf("\nSorry, you loose! :(\n\n");
			}

			/* SCREEN UPDATE */
			SDL_RenderPresent(rend);  // display the *render;
			SDL_Delay(1000/CLOCK_TICK);  // set target fps to CLOCK_TICK;
		}
	}
	
	/*----
	* END
	*///--
	SDL_Quit();
	printf("The program ended successfully.\n");
	return 0;
}