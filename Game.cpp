
#include <iostream>
#include <algorithm>
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
    int X = (mouseX - 106) / 70;
    int Y = (mouseY - 100) / 70;
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
        for (int j = 0; j < col  - 4; j++)
        {
            // check hàng ngang
            if (board[i][j] != -1 )
            {
                bool checkrow = true;
                for (int k = j + 1; k < j + 5; k++)
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
    }

    for (int i = 0; i <= (row - 1) - 4; i++)
    {
        for (int j = 0; j < col; j++)
        {
            // Check hàng dọc
            bool checkcol = true;
            if (board[i][j] != -1)
            {
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
    }


    //hàng chéo tren trai -> duoi phai
    for (int i = 0; i <= (row - 1) - 4; i++)
    {
        for (int j = 0; j <= (col - 1) - 4; j++)
        {
            // Kiểm tra hàng chéo từ trên bên trái xuống dưới bên phải
            if (board[i][j] != -1)
            {
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
    }

    for (int i = 0; i <= (row - 1) - 4; i++)
    {
        for (int j = 4; j < col; j++)
        {
            // Kiểm tra hàng chéo ngược từ trên bên phải xuống dưới bên trái
            if (board[i][j] != -1)
            {
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

void Game::botPlay(int player)
{
    
        int bestScore = INT_MIN;
        int bestMoveX = -1;
        int bestMoveY = -1;

        // Duyệt qua tất cả các ô trống trên bàn cờ
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (board[i][j] == -1) {
                    // Thử đánh vào ô trống này
                    board[i][j] = player;

                    // Tính điểm cho nước đi này
                    int score = minimax(0,-9999,9999, false);

                    // Lưu lại nước đi có điểm tốt nhất
                    if (score > bestScore) {
                        bestScore = score;
                        bestMoveX = i;
                        bestMoveY = j;
                    }

                    // Đặt lại giá trị của ô sau khi thử nước đi
                    board[i][j] = -1;
                }
            }
        }

        // Thực hiện nước đi tốt nhất
        if (bestMoveX != -1 && bestMoveY != -1) {
            board[bestMoveX][bestMoveY] = player;
        }
}

// Hàm Minimax với cắt tỉa Alpha-Beta để tìm nước đi tốt nhất
int Game::minimax(int depth, int alpha, int beta, bool maximizingPlayer) {
    int score = game_state();

    // Trả về điểm nếu trạng thái hiện tại là trạng thái kết thúc
    if (score != 0) {
        return score;
    }

    // Trường hợp hòa cờ
    if (depth >= col * row) {
        return 0;
    }

    if (maximizingPlayer) {
        int bestScore = INT_MIN;

        // Duyệt qua tất cả các ô trống trên bàn cờ
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (board[i][j] == -1) {
                    // Thử đánh vào ô trống này
                    board[i][j] = 1;

                    // Tính điểm cho nước đi này
                    int currentScore = minimax(depth + 1, alpha, beta, false);

                    // Cập nhật điểm tốt nhất
                    bestScore = max(bestScore, currentScore);
                    alpha = max(alpha, bestScore);

                    // Đặt lại giá trị của ô sau khi thử nước đi
                    board[i][j] = -1;

                    // Cắt tỉa Beta
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }

        return bestScore;
    }
    else {
        int bestScore = INT_MAX;

        // Duyệt qua tất cả các ô trống trên bàn cờ
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (board[i][j] == -1) {
                    // Thử đánh vào ô trống này
                    board[i][j] = 0;

                    // Tính điểm cho nước đi này
                    int currentScore = minimax(depth + 1, alpha, beta, true);

                    // Cập nhật điểm tốt nhất
                    bestScore = min(bestScore, currentScore);
                    beta = min(beta, bestScore);

                    // Đặt lại giá trị của ô sau khi thử nước đi
                    board[i][j] = -1;

                    // Cắt tỉa Alpha
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }

        return bestScore;
    }
}


void Game::update()
{
    if (selected[0] != -1)
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
                if (winner != -1 && e.type == SDL_MOUSEBUTTONDOWN )
                {

                    mouseX = e.button.x;
                    mouseY = e.button.y;
                    if ((mouseX >= 467 && mouseX <= 776 && mouseY >= 462 && mouseY <= 540 ))
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
                        if (winner == -1 && played)
                        {
                            if (player == 0)
                            {
                                if (e.type == SDL_MOUSEBUTTONDOWN)
                                {
                                    mouseX = e.button.x;
                                    mouseY = e.button.y;
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
                                    
                                }
                            }
                            else
                            {
                                botPlay(player);
                                
                                player = 1 - player;
                            }

                            //update();
                            renderboard();
                            winner = game_state();
                        }
                              
                    }
                    if (mode == 2)
                    {
                        
                        if (e.type == SDL_MOUSEBUTTONDOWN)
                        {
                            mouseX = e.button.x;
                            mouseY = e.button.y;
                        
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
                        }
                        renderboard();
                        winner = game_state();
                        
                    }
                    if (mode == 3)
                    {
                        quit = true;
                    }
                }

            }
        }
        if (winner != -1)
            game_over(winner);

        Renderer();
    }

    close();
}