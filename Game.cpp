﻿#include <iostream>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "draw.h"
#include "Game.h"

using namespace std;

void Game::init()
{
    restart = false;
    player = 0;
    winner = -1;
    memset(board, -1, sizeof(board));
    memset(selected, -1, sizeof(selected));
}

void Game::updateMouse(int mouseX, int mouseY)
{
    int X = mouseX / 100;
    int Y = mouseY / 100; //100 x 100 pixel
    if (X >= 0 && Y >= 0 && X < N && Y < N)
    {
        selected[0] = X;
        selected[1] = Y;
    }
}

int Game::game_state()
{
    //check draw
    bool draw = true;
    for (int i = 0; i < N;i++)
        for (int j = 0; j < N; j++)
            if (board[i][j] == -1) draw = false;
    if (draw) return 2;
                               
    for (int i = 0; i < N; i++)
    {
        
        bool checkcol = 1;
        bool checkrow = 1;

        for (int j = 0; j < N; j++)
        {
            // CHECK HANG NGANG
            if (board[i][j] != board[i][0])
            {
                checkrow = 0;
            }

            //CHECK HANG DOC
            if (board[j][i] != board[0][i])
            {
                checkcol = 0;
            }
        }

        if (checkrow) return board[i][0];

        if (checkcol) return board[0][i];

    }
    //hàng chéo
    bool checkcheoTayBacDongNam = 1;
    /*
    0
      0
        0
          0
    */
    bool checkcheoDongBacTayNam = 1;
    /*
                0
            0
        0
    0
    */
    for (int i = 0; i < N; i++)
    {
        if (board[i][i] != board[0][0]) checkcheoTayBacDongNam = 0;
        if (board[i][N - 1 - i] != board[0][N - 1]) checkcheoDongBacTayNam = 0;
    }
    if (checkcheoTayBacDongNam)  return board[0][0];
    if (checkcheoDongBacTayNam)  return board[0][N - 1];
   

    return -1;
}

int Game::bot(int step) //how bot plays
{
    int score = game_state();
    if (score != -1 || step == 4)
    {
        if (score == 1) score = 24 - step - 1;
        else if (score == 0) score = step - 1 - 24;
        else score = 0;
        return score;
    }
    int turn = step % 2;

    int test;
    if (turn == 1) score = -24; else score = 24;
    for (int i = 0;i < N;i++)
        for (int j = 0; j < N;j++)
            if (board[i][j] == -1)
            {
                board[i][j] = turn;
                test = bot(step + 1);
                if (turn == 1)
                {
                    if (test > score)//kiểm tra giá trị thử của bot có tốt hơn không
                    {
                        score = test;
                        if (step == 1) { x = i; y = j; }
                    }
                }
                else
                {
                    if (test < score)//kiểm tra giá trị thử của người chơi
                    {
                        score = test;
                        if (step == 0) { x = i; y = j; }
                    }
                }
                board[i][j] = -1;
            }
    return score;
}

void Game::update()
{
    if (selected[0] != -1 || player == 1)
    {
        if (player == 1)
        {
            bot(1);
            selected[0] = x;
            selected[1] = y;
        }
        int X = selected[0];
        int Y = selected[1];
        selected[0] = -1;
        if (board[X][Y] != -1) return;
        board[X][Y] = player;
        player = 1 - player;
    }
}

void Game::run()
{
    int mouseX, mouseY;
    SDL_Event e;
    Init();
    game_start();
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) quit = true;
            
            if (played || (e.type == SDL_MOUSEBUTTONDOWN && mouseX >= 170 && mouseX <= 327 && mouseY >= 277 && mouseY <= 320))
            {
                played = true;
                if (restart)
                {
                    init();
                    e.type = NULL;
                }
                if (winner == -1 && player == 0 && e.type == SDL_MOUSEBUTTONDOWN && e.button.clicks == 1)
                {
                    updateMouse(mouseX, mouseY);
                    e.type = NULL;
                }

                if (winner != -1 && e.type == SDL_MOUSEBUTTONDOWN && mouseX >= 173 && mouseX <= 329 && mouseY >= 279 && mouseY <= 324)
                {
                    restart = true;
                }
            }
        }
        if (winner == -1 && played)
        {
            update();
            render();
            winner = game_state();
        }
        if (winner != -1) game_over(winner);

        if (e.type == SDL_MOUSEMOTION) {
            mouseX = e.button.x;
            mouseY = e.button.y;
        }
    }

    close();
}