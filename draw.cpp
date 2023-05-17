#include<iostream>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Game.h"
#include "draw.h"

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
	surface[0] = IMG_Load("NewImage/Menu0.png");
	texture[0] = SDL_CreateTextureFromSurface(renderer, surface[0]);

	surface[1] = IMG_Load("NewImage/MenuBot.png");
	texture[1] = SDL_CreateTextureFromSurface(renderer, surface[1]);

	surface[2] = IMG_Load("NewImage/MenuPvP.png");
	texture[2] = SDL_CreateTextureFromSurface(renderer, surface[2]);


	surface[3] = IMG_Load("NewImage/MenuQuit.png");
	texture[3] = SDL_CreateTextureFromSurface(renderer, surface[3]);


	surface[4] = IMG_Load("NewImage/Board10x15.png");
	texture[4] = SDL_CreateTextureFromSurface(renderer, surface[4]);


	surface[5] = IMG_Load("NewImage/image_X.png");
	texture[5] = SDL_CreateTextureFromSurface(renderer, surface[5]);


	surface[6] = IMG_Load("NewImage/image_O.png");
	texture[6] = SDL_CreateTextureFromSurface(renderer, surface[6]);


	surface[7] = IMG_Load("NewImage/X_win.png");
	texture[7] = SDL_CreateTextureFromSurface(renderer, surface[7]);


	surface[8] = IMG_Load("NewImage/O_win.png");
	texture[8] = SDL_CreateTextureFromSurface(renderer, surface[8]);

	surface[9] = IMG_Load("NewImage/Draw_win.png");
	texture[9] = SDL_CreateTextureFromSurface(renderer, surface[9]);

	/*
	0: menu khong co gi
	1: menu sang Bot
	2: menu sang PvP
	3: menu sang Quit
	4: board 15*10
	5: X
	6: O
	7: X win
	8: O win
	9: Draw win
	*/
}
/*
void loadMedia(int id, int x, int y)
{
	SDL_Rect dest = { x, y,SCREEN_WIDTH,SCREEN_HEIGHT};

	SDL_RenderCopy(renderer, texture[id], nullptr, &dest);
}
*/
void loadMedia(int id, int x, int y)
{

	SDL_Rect dest = { x, y,surface[id]->w,surface[id]->h };

	SDL_RenderCopy(renderer, texture[id], NULL, &dest);
}

void game_start()
{
	loadMedia(0, 0, 0); // render man hinh Menu
	//SDL_RenderPresent(renderer);
}

void game_over(int winner)
{
	loadMedia(winner + 7, 0, 0); // 0 la X win, 1 la O win, 2 la draw,  +7 de render anh X hoac O win
	//SDL_RenderPresent(renderer);
}
void Renderer()
{
	SDL_RenderPresent(renderer);
}
void Game::renderboard()//ve X hoac O len man hinh
{
	loadMedia(4, 0, 0);
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			if (board[i][j] != -1)
				loadMedia(board[i][j] + 5, j * 70.5 + 103, i * 70.5 + 100);
	//SDL_RenderPresent(renderer);//tinh sau
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
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