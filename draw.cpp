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
SDL_Texture* texture[25];
SDL_Surface* surface[25];
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

	surface[5] = IMG_Load("NewImage/image_X_1015.png");
	texture[5] = SDL_CreateTextureFromSurface(renderer, surface[5]);

	surface[6] = IMG_Load("NewImage/image_O_1015.png");
	texture[6] = SDL_CreateTextureFromSurface(renderer, surface[6]);

	surface[7] = IMG_Load("NewImage/X_win.png");
	texture[7] = SDL_CreateTextureFromSurface(renderer, surface[7]);

	surface[8] = IMG_Load("NewImage/O_win.png");
	texture[8] = SDL_CreateTextureFromSurface(renderer, surface[8]);

	surface[9] = IMG_Load("NewImage/Draw_win.png");
	texture[9] = SDL_CreateTextureFromSurface(renderer, surface[9]);

	surface[10] = IMG_Load("NewImage/Board3x3.png");
	texture[10] = SDL_CreateTextureFromSurface(renderer, surface[10]);

	surface[11] = IMG_Load("NewImage/Board5x5.png");
	texture[11] = SDL_CreateTextureFromSurface(renderer, surface[11]);

	surface[12] = IMG_Load("NewImage/Board10x15.png");
	texture[12] = SDL_CreateTextureFromSurface(renderer, surface[12]);

	surface[13] = IMG_Load("NewImage/ChooseMode.png");
	texture[13] = SDL_CreateTextureFromSurface(renderer, surface[13]);
	/*
	surface[14] = IMG_Load("NewImage/ChooseMode3x3.png");
	texture[14] = SDL_CreateTextureFromSurface(renderer, surface[14]);

	surface[15] = IMG_Load("NewImage/ChooseMode5x5.png");
	texture[15] = SDL_CreateTextureFromSurface(renderer, surface[15]);

	surface[16] = IMG_Load("NewImage/ChooseMode10x15.png");
	texture[16] = SDL_CreateTextureFromSurface(renderer, surface[16]);
	*/
	surface[17] = IMG_Load("NewImage/image_X_33.png");
	texture[17] = SDL_CreateTextureFromSurface(renderer, surface[17]);

	surface[18] = IMG_Load("NewImage/image_O_33.png");
	texture[18] = SDL_CreateTextureFromSurface(renderer, surface[18]);

	surface[19] = IMG_Load("NewImage/image_X_55.png");
	texture[19] = SDL_CreateTextureFromSurface(renderer, surface[19]);

	surface[20] = IMG_Load("NewImage/image_O_55.png");
	texture[20] = SDL_CreateTextureFromSurface(renderer, surface[20]);





	/*
	0: menu khong co gi
	1: menu sang Bot
	2: menu sang PvP
	3: menu sang Quit
	4: khong co gia tri
	5: X 10 15
	6: O 10 15
	7: X win
	8: O win
	9: Draw win
	10:Board 3*3
	11:Board 5x5
	12:Board 10*15
	13:ChooseMode0
	14:Chon mode 3x3
	15:Chon mode 5x5
	16:Chon mode 10x15
	17:X 33
	18:O 33
	19:X 55
	20:O 55
	*/
}

void loadMedia(int id, int x, int y)
{

	SDL_Rect dest = { x, y,surface[id]->w,surface[id]->h };

	SDL_RenderCopy(renderer, texture[id], NULL, &dest);
}

void Game::game_start(SDL_Event e)
{
		int mouseX = e.motion.x;
		int mouseY = e.motion.y;
		if (mouseX >= 465 && mouseX <= 775 && mouseY >= 375 && mouseY <= 450)
			loadMedia(1, 0, 0); //pvbot
		else if (mouseX >= 465 && mouseX <= 775 && mouseY >= 495 && mouseY <= 570)
			loadMedia(2, 0, 0); // pvp
		else if (mouseX >= 465 && mouseX <= 775 && mouseY >= 615 && mouseY <= 690)
			loadMedia(3, 0, 0);// quit
		else
			loadMedia(0, 0, 0);
}

void Game::menu_game_mode(SDL_Event e)
{
	int mouseX = e.motion.x;
	int mouseY = e.motion.y;
	/*if (mouseX >= 0 && mouseX < 867 && mouseY >= 260 && mouseY <= 840)
		loadMedia(1, 0, 0); // impossible
	else if (mouseX >= 867 && mouseX <= 1240 && mouseY >= 260 && mouseY < 622)
		loadMedia(2, 0, 0); // normal
	else if (mouseX >= 867 && mouseX <= 1240 && mouseY >= 622 && mouseY <= 840)
		loadMedia(3, 0, 0);// quit
	else*/
	loadMedia(13, 0, 0);
}
void game_over(int winner)
{
	loadMedia(winner + 7, 0, 0); // 0 la X win, 1 la O win, 2 la draw,  +7 de render anh X hoac O win
}
void Renderer()
{
	SDL_RenderPresent(renderer);
}
void Game::renderboard()//ve X hoac O len man hinh
{
	loadMedia(boardMode + 9, 0, 0); // 9 la chenh lech giua hang so cua boardmode va anh cua no
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			if (board[i][j] != -1)
				loadMedia(board[i][j] + ChenhLechXO, j * DoDaiCanh + ChechLechX, i * DoDaiCanh + ChechLechY);
}

void close()
{
	for (int i = 0; i < 25;i++)
		SDL_DestroyTexture(texture[i]);

	for (int i = 0; i < 25;i++)
		SDL_FreeSurface(surface[i]);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;
	IMG_Quit();
	SDL_Quit();
}