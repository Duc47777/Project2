#include<iostream>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Game.h"

using namespace std;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture[10];
SDL_Surface* surface[10];
TTF_Font* font = NULL;

void Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) >= 0)
	{
		window = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window != NULL)
		{
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer != NULL)
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		}
	}
	surface[0] = IMG_Load("Image/background.png");
	texture[0] = SDL_CreateTextureFromSurface(renderer, surface[0]);

	surface[1] = IMG_Load("Image/icon_x.png");
	texture[1] = SDL_CreateTextureFromSurface(renderer, surface[1]);

	surface[2] = IMG_Load("Image/icon_o.png");
	texture[2] = SDL_CreateTextureFromSurface(renderer, surface[2]);

	surface[3] = IMG_Load("Image/start.png");
	texture[3] = SDL_CreateTextureFromSurface(renderer, surface[3]);

	surface[4] = IMG_Load("Image/state_player_win.png");
	texture[4] = SDL_CreateTextureFromSurface(renderer, surface[4]);

	surface[5] = IMG_Load("Image/state_bot_win.png");
	texture[5] = SDL_CreateTextureFromSurface(renderer, surface[5]);

	surface[6] = IMG_Load("Image/state_tie.png");
	texture[6] = SDL_CreateTextureFromSurface(renderer, surface[6]);

}

void loadMedia(int id, int x, int y)
{
	SDL_Rect dest = { x, y,surface[id]->w,surface[id]->h };

	SDL_RenderCopy(renderer, texture[id], NULL, &dest);
}

void game_start()
{
	loadMedia(3, 0, 0);
	SDL_RenderPresent(renderer);
}

void game_over(int winner)
{
	loadMedia(winner + 4, 0, 0);
	SDL_RenderPresent(renderer);
}

void Game::render()
{
	loadMedia(0, 0, 0);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (board[i][j] != -1)
				loadMedia(board[i][j] + 1, i * 100 + 10, j * 100 + 10);
	SDL_RenderPresent(renderer);
}

void close()
{
	for (int i = 0; i < 10;i++)
		SDL_DestroyTexture(texture[i]);

	for (int i = 0; i < 10;i++)
		SDL_FreeSurface(surface[i]);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;
	IMG_Quit();
	SDL_Quit();
}