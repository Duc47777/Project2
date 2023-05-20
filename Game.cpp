
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
    replay = false;
    mode = 0;
    boardMode = 0;
    played = 0;
    player = 0;
    winner = -1;
    memset(board, -1, sizeof(board));
    memset(selected, -1, sizeof(selected));
}

void Game::updateMouse(int mouseX, int mouseY)
{
    int X = (mouseX - ChechLechX) / DoDaiCanh;
    int Y = (mouseY - ChechLechY) / DoDaiCanh;
    if (X >= 0 && Y >= 0 && X < col && Y < row)
    {
        selected[0] = X;
        selected[1] = Y;
    }
}

int Game::game_state()
{
    //check draw
    if(boardMode == 1)
    { 
        for (int i = 0; i < 3; i++)
            if (board[i][1] == board[i][0] && board[i][0] == board[i][2] && board[i][0] != -1 && board[i][1] != -1 && board[i][2] != -1)
                return board[i][0];
        for (int i = 0; i < 3; i++)
            if (board[0][i] == board[1][i] && board[0][i] == board[2][i] && board[0][i] != -1 && board[1][i] != -1 && board[2][i] != -1)
                return board[0][i];
        if (board[1][1] == board[0][0] && board[0][0] == board[2][2] && board[1][1] != -1 && board[0][0] != -1 && board[2][2] != -1)
            return board[0][0];
        if (board[0][2] == board[1][1] && board[2][0] == board[1][1] && board[2][0] != -1 && board[1][1] != -1 && board[0][2] != -1)
            return board[1][1];
        bool checkDraw = true;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board [i][j] == -1) checkDraw = false;
            }
        }
        if (checkDraw) return 2;
        return -1;


    }
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

    bool draw = true;
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            if (board[i][j] == -1) draw = false;
    if (draw) return 2;

    return -1;
}

void Game::SetGameMode(int x, int y)
{
    if (x >= 465 && x <= 775 && y >= 375 && y <= 450)
    {
        mode = 1;
        PlayMedia(0);
    }
    else if (x >= 465 && x <= 775 && y >= 495 && y <= 570)
    {
        mode = 2;
        PlayMedia(0);
    }
    else if (x >= 465 && x <= 775 && y >= 615 && y <= 690)
    {
        mode = 3;
        PlayMedia(0);
        quit = 1;
    }
}

void Game::setBoardMode(int x, int y)
{
    if (x >= 0 && x < 867 && y >= 260 && y <= 840)
    {
        boardMode = 1; // impossible
        PlayMedia(1);
    }
    else if (x >= 867 && x <= 1240 && y >= 260 && y < 622)
    {
        boardMode = 2; // normal
        PlayMedia(1);
    }
    else if (x >= 867 && x <= 1240 && y >= 622 && y <= 840)
    {
        boardMode = 3; // easy
        PlayMedia(1);
    }
}

void Game::ApplyBoardMode()
{
    switch (boardMode)
    {
    case 1:
        row = 3;
        col = 3;
        ChenhLechXO = 17;
        DoDaiCanh = 238;
        ChechLechX = 280;
        ChechLechY = 83;
        break;
    case 2:
        row = 5;
        col = 5;
        ChenhLechXO = 19;
        DoDaiCanh = 142;
        ChechLechX = 274;
        ChechLechY = 77;
        break;
    case 3:
        row = 10;
        col = 15;
        ChenhLechXO = 5;
        DoDaiCanh = 70.5;
        ChechLechX = 105;
        ChechLechY = 100;
        break;
    default:
        break;
    }
}

void Game::botPlay(int player)
{
    
        int bestScore = INT_MIN;
        int bestMoveX = -1;
        int bestMoveY = -1;
        if (Checkif4())
        {
            return;
        }
        
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
}  // Hàm Minimax với cắt tỉa Alpha-Beta để tìm nước đi tốt nhất

bool Game::Checkif4()
{
    return false;
}


bool Game::Checkif3()
{
    return false;
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
                        PlayMedia(2);
                        player = 1 - player;
                    }
                }

            }
        }
        else
        {
            botPlay(player);
            PlayMedia(2);
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
                PlayMedia(2);
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
    bool endgameSound_played = 0;
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


            if (!played && e.type == SDL_MOUSEMOTION)
                game_start(e);

            if (!boardMode && !mode && e.type == SDL_MOUSEBUTTONDOWN)
            {
                mouseX = e.motion.x;
                mouseY = e.motion.y;
                SetGameMode(mouseX, mouseY);
                if (mode == 3)
                {
                    break;
                }
                
                
                Load_image(13, 0, 0);
                continue;
            }
            
            if (mode && !played && e.type == SDL_MOUSEMOTION )
                menu_game_mode(e);

            if (mode && !boardMode && e.type == SDL_MOUSEBUTTONDOWN)
            {
                mouseX = e.motion.x;
                mouseY = e.motion.y;
                setBoardMode(mouseX, mouseY);// chon mode trong img
                continue;
            }

            ApplyBoardMode();
            cout <<boardMode << " " << row << " " << col << endl;

            if (restart)
            {
                init();endgameSound_played = 0;
                e.type = NULL;
            }
            if (replay)
            {
                memset(board, -1, sizeof(board));
                memset(selected, -1, sizeof(selected));
                replay = 1 - replay;
            }
            if (played || (mode&&boardMode))
            {
                played = true;

                if (restart)// option choi lai
                {
                    init();
                    
                    e.type = NULL;
                }
                if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                    mouseX = e.motion.x;
                    mouseY = e.motion.y;
                    if (mouseX > 32 && mouseX < 99 && mouseY >32 && mouseY < 99)
                    {
                        replay = true;
                        Load_image(boardMode + 9, 0, 0);
                    }
                }
                if (winner != -1 && e.type == SDL_MOUSEBUTTONDOWN )
                {

                    mouseX = e.button.x;
                    mouseY = e.button.y;
                    if (mouseX >= 467 && mouseX <= 776 && mouseY >= 462 && mouseY <= 540 )
                    {
                        restart = true;
                        Load_image(0, 0, 0);
                        continue;
                    }
                }
                
                if (winner == -1 && boardMode)// nhan vao input tu chuot trong luc choi game
                {
                    switch (mode)
                    {
                        case 1:
                            GameMode1(e, mouseX, mouseY);
                            break;
                        case 2:
                            GameMode2(e, mouseX, mouseY);
                            break;
                        default:
                            break;
                    }
                }
            }
            if (winner != -1)
            {
                game_over(winner);
                if(endgameSound_played==0)(winner == 2) ? PlayMedia(4) : PlayMedia(3);
                endgameSound_played = 1;
            }
            Renderer();
        }
        
    }

    close();
}