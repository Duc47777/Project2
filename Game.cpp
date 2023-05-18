
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
    mode = 0;
    played = 0;
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

void Game::setMode(int x, int y)
{
    if (x >= 465 && x <= 775 && y >= 375 && y <= 450)
        mode = 1;
    if (x >= 465 && x <= 775 && y >= 495 && y <= 570)
        mode = 2;
    if (x >= 465 && x <= 775 && y >= 615 && y <= 690)
        mode = 3;
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

void Game::GameMode1(SDL_Event e, int mouseXX, int mouseYY)
{
    if (winner == -1 && played)
    {
        if (player == 0)
        {
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                mouseXX = e.button.x;
                mouseYY = e.button.y;
                updateMouse(mouseXX, mouseYY);

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
        renderboard();
        winner = game_state();
    }
}

void Game::GameMode2(SDL_Event e, int mouseXX, int mouseYY)
{
    
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        mouseXX = e.button.x;
        mouseYY = e.button.y;

        updateMouse(mouseXX, mouseYY);

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

void Game::run()
{
    int mouseX = 0, mouseY = 0;
    SDL_Event e;
    Init();
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }

            game_start(e);
            
            if (!mode && e.type == SDL_MOUSEBUTTONDOWN)
            {

                mouseX = e.button.x;
                mouseY = e.button.y;
                setMode(mouseX, mouseY);
                
            }
            
            if (played || mode)
            {
                played = true;

                if (restart)// option choi lai
                {
                    init();
                    e.type = NULL;
                }
                
                if (winner != -1 && e.type == SDL_MOUSEBUTTONDOWN )
                {

                    mouseX = e.button.x;
                    mouseY = e.button.y;
                    if (mouseX >= 467 && mouseX <= 776 && mouseY >= 462 && mouseY <= 540 )
                    {
                        restart = true;
                        loadMedia(0, 0, 0);
                        continue;
                    }
                }
                
                if (winner == -1)// nhan vao input tu chuot trong luc choi game
                {
                    switch (mode)
                    {
                        case 1:
                            GameMode1(e, mouseX, mouseY);
                            break;
                        case 2:
                            GameMode2(e, mouseX, mouseY);
                            break;
                        case 3:
                            quit = true;
                            break;
                        default:
                            break;
                    }
                }
            }

            if (winner != -1)
                game_over(winner);
            Renderer();
        }
        
    }

    close();
}