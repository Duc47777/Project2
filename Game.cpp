#include <iostream>
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
    int X = (mouseX - 105) / 70;
    int Y = (mouseY - 98 ) / 70 ; 
    if (X >= 0 && Y >= 0 && X < col && Y < row)
    {
        selected[0] = X;
        selected[1] = Y;
    }
}

int Game::game_state()
{
    //check draw
    bool draw = true;
    for (int i = 0; i < row;i++)
        for (int j = 0; j < col; j++)
            if (board[i][j] == -1) draw = false;
    if (draw) return 2;

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j <= (col - 1) - 4; j++)
        {
            // check hàng ngang
            bool checkrow = true;
            for (int k = j + 1; k <= j + 4; k++)
            {
                if (board[i][k] != board[i][j])
                {
                    checkrow = false;
                    break;
                }
            }
            if (checkrow) return board[i][j];
        }
    }
    
    for (int i = 0; i <= (row - 1) - 4; i++)
    {
        for (int j = 0; j < col; j++)
        {
            // Check hàng dọc
            bool checkcol = true;
            for (int k = i + 1; k <= i + 4; k++)
            {
                if (board[k][j] != board[i][j])
                {
                    checkcol = false;
                    break;
                }
            }
            if (checkcol) return board[i][j];
        }
    }

    
    //hàng chéo tren trai -> duoi phai
    for (int i = 0; i <= (row - 1) - 4; i++)
    {
        for (int j = 0; j <= (col - 1) - 4; j++)
        {
            // Kiểm tra hàng chéo từ trên bên trái xuống dưới bên phải
            bool checkYbangX = true;
            for (int k = 1; k <= 4; k++)
            {
                if (board[i + k][j + k] != board[i][j])
                {
                    checkYbangX = false;
                    break;
                }
            }
            if (checkYbangX) return board[i][j];
        }
    }

    for (int i = 0; i <= (row - 1) - 4; i++)
    {
        for (int j = 4; j < col; j++)
        {
            // Kiểm tra hàng chéo ngược từ trên bên phải xuống dưới bên trái
            bool checkYbangtruX = true;
            for (int k = 1; k <= 4; k++)
            {
                if (board[i + k][j - k] != board[i][j])
                {
                    checkYbangtruX = false;
                    break;
                }
            }
            if (checkYbangtruX) return board[i][j];
        }
    }

    return -1;
}

int Game::bot(int step) // Cách bot chơi
{
    int score = game_state();
    if (score != -1 || step == 4)
    {
        if (score == 1)
            score = row * col - 1 - step - 1;
        else if (score == 0)
            score = step - 1 - (row * col - 1);
        else
            score = 0;
        return score;
    }
    int turn = step % 2;

    int test;
    if (turn == 1)
        score = -(row * col - 1);
    else
        score = row * col - 1;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (board[i][j] == -1)
            {
                board[i][j] = turn;
                test = bot(step + 1);
                if (turn == 1)
                {
                    if (test > score)
                    {
                        score = test;
                        if (step == 1)
                        {
                            x = i;
                            y = j;
                        }
                    }
                }
                else
                {
                    if (test < score)
                    {
                        score = test;
                        if (step == 0)
                        {
                            x = i;
                            y = j;
                        }
                    }
                }
                board[i][j] = -1;
            }
        }
    }
    return score;
}

void Game::setMode(int m)
{
    mode = m;
}

void Game::update()
{
    if (selected[0] != -1 )
    {
        int X = selected[0];
        int Y = selected[1];
        selected[0] = -1;
        if (board[X][Y] != -1) return;
        board[X][Y] = player;
        player = 1 - player;

        if (mode == 1 && player == 1)
        {
            bot(1);
            selected[0] = x;
            selected[1] = y;
        }
    }
}

void Game::run()
{
    int mouseX, mouseY;
    SDL_Event e;
    Init();
    game_start();
    setMode(0);
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            
            if (!played && e.type == SDL_MOUSEMOTION)
            {
                mouseX = e.motion.x;
                mouseY = e.motion.y;
                if (mouseX >= 465 && mouseX <= 775 && mouseY >= 375 && mouseY <= 450)
                    loadMedia(1, 0, 0);
                if (mouseX >= 465 && mouseX <= 775 && mouseY >= 495 && mouseY <= 570)
                    loadMedia(2, 0, 0);
                if (mouseX >= 465 && mouseX <= 775 && mouseY >= 615 && mouseY <= 690)
                    loadMedia(3, 0, 0);

            }
            if (!mode && e.type == SDL_MOUSEBUTTONDOWN)
            {
                
                mouseX = e.button.x;
                mouseY = e.button.y;

                if (mouseX >= 465 && mouseX <= 775 && mouseY >= 375 && mouseY <= 450)
                        setMode(1);
                if (mouseX >= 465 && mouseX <= 775 && mouseY >= 495 && mouseY <= 570)
                        setMode(2);
                if (mouseX >= 465 && mouseX <= 775 && mouseY >= 615 && mouseY <= 690)
                        setMode(3);
            }
            
            if (played || mode)
            {
                played = true;

                if (restart)// option choi lai
                {
                    init();
                    e.type = NULL;
                }
                
                bool pushedRestart = 0;
                if (winner != -1 && e.type == SDL_MOUSEBUTTONDOWN)
                {
                    
                    mouseX = e.button.x;
                    mouseY = e.button.y;
                    if (mouseX >= 467 && mouseX <= 776 && mouseY >= 462 && mouseY <= 540)
                    {
                        pushedRestart = 1;
                    }
                }
                if (winner != -1 && pushedRestart)// xu li vi tri an restart
                {
                    restart = true;
                }

                if (winner == -1)// nhan vao input tu chuot trong luc choi game
                {
                    if (mode == 1)
                    {
                        // choi voi bot
                        
                    }
                    if (mode == 2)
                    {
                        // choi pvp
                        if (e.type == SDL_MOUSEBUTTONDOWN)
                        {
                            mouseX = e.button.x;
                            mouseY = e.button.y;
                           /* for (int i = 0; i < row; i++)
                            {
                                for (int j = 0; j < col; j++)
                                {
                                    cout << board[i][j] << "  ";
                                }
                                cout << endl;
                            }
                            cout << endl << endl;
                            cout << "               " << winner << endl;
                            */
                        }
                        updateMouse(mouseX, mouseY);

                        if (selected[0] != -1)
                        {
                            int Y = selected[0];
                            int X = selected[1];
                            selected[0] = -1;
                            if (board[X][Y] == -1)
                            {
                                board[X][Y] = player;
                                player = 1 - player;
                            }
                            
                        }
                        renderboard();
                        winner=game_state();
                        
                    }
                    if (mode == 3)
                    {
                        quit = true;
                    }
                }

            }
            if (winner != -1)
                game_over(winner);
        }

        Renderer();
    }

    close();
}