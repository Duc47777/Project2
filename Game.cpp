
#include <iostream>
#include <algorithm>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "draw.h"
#include "Game.h"

using namespace std;

//khởi tạo bàn chơi và set các trạng thái về ban đầu
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
// -1 là chưa đánh gì, 0 là X, 1 là O

// chọn vị trí chuột để ấn trên bàn
void Game::updateMouse(int mouseX, int mouseY)
{
    float X = 1.0* (mouseX - ChechLechX) / DoDaiCanh;
    float Y = 1.0* (mouseY - ChechLechY) / DoDaiCanh;
    if (X >= 0 && Y >= 0 && X < col && Y < row)
    {
        int temp_x = X;
        int temp_y = Y;
        selected[0] = temp_x;
        selected[1] = temp_y;
    }
}

//load trạng thái win game
int Game::game_state()
{
    //check draw
    if (boardMode == 1)
    {
        for (int i = 0; i < 3; i++)
            if (board[i][1] == board[i][0] && board[i][0] == board[i][2] && board[i][0] != -1)
                return board[i][0];
        for (int i = 0; i < 3; i++)
            if (board[0][i] == board[1][i] && board[0][i] == board[2][i] && board[0][i] != -1)
                return board[0][i];
        if (board[1][1] == board[0][0] && board[0][0] == board[2][2] && board[0][0] != -1)
            return board[0][0];
        if (board[0][2] == board[1][1] && board[2][0] == board[1][1] && board[1][1] != -1)
            return board[1][1];
        bool checkDraw = true;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == -1) checkDraw = false;
            }
        }
        if (checkDraw) return 2;
        return -1;


    }
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col - 4; j++)
        {
            // check hàng ngang
            if (board[i][j] != -1)
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

// cài đặt chế độ pvp hay pvbot : 1 là pvbot; 2 là pvp
void Game::SetGameMode(int x, int y)
{
    if (x >= 465 && x <= 775 && y >= 375 && y <= 450)
    {
        mode = 1;
        Load_image(13, 0, 0);
        PlayMedia(0);
    }
    else if (x >= 465 && x <= 775 && y >= 495 && y <= 570)
    {
        mode = 2;
        Load_image(13, 0, 0);
        PlayMedia(0);
    }
    else if (x >= 465 && x <= 775 && y >= 615 && y <= 690)
    {
        mode = 3;
        PlayMedia(0);
        quit = 1;
    }
}

// cài đặt chế độ bàn: 1 là 3x3, 2 là 5x5, 3 là 10x15
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

//chỉnh các thông số trò chơi theo chế độ bàn
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

//cách mà bot chơi
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
                int score = minimax(0, -9999, 9999, false);

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
    if (boardMode == 1)
    {
        if (CheckBotWin2())
            return;
        if (CheckPlayerWin2())
            return;
        if (board[1][1] == -1)
        {
            board[1][1] = 1;
            return;
        }
    }

    if (CheckBotWinMid5()) {
        return;
    }

    if (CheckPlayerWinMid5()) {
        return;
    }

    if (CheckBotWin4()) {
        return;
    }

    if (CheckPlayerWin4())
    {
        return;
    }

    if (CheckPlayerWin3())
    {
        return;
    }

    // Thực hiện nước đi tốt nhất
    if (bestMoveX != -1 && bestMoveY != -1)
    {
        board[bestMoveX][bestMoveY] = player;
    }

}

//kiểm tra xem có 4 ô nào của người chơi không, nếu có thì thực hiện chặn
bool Game::CheckPlayerWin4()
{
    //ngang
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col - 4; j++) {
            if (board[i][j] == 0 && board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2] && board[i][j] == board[i][j + 3] && board[i][j + 4] == -1) {
                board[i][j + 4] = 1;
                return true;
            }
        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = 1; j < col - 3; j++) {
            if (board[i][j] == 0 && board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2] && board[i][j] == board[i][j + 3] && board[i][j - 1] == -1) {
                board[i][j - 1] = 1;
                return true;
            }
        }
    }

    //doc

    for (int i = 0; i < row - 4; i++) {
        for (int j = 0; j < col; j++) {
            if (board[i][j] == 0 && board[i][j] == board[i + 1][j] && board[i][j] == board[i + 2][j] && board[i][j] == board[i + 3][j] && board[i + 4][j] == -1) {
                board[i + 4][j] = 1;
                return true;
            }
        }
    }

    for (int i = 1; i < row - 3; i++) {
        for (int j = 0; j < col; j++) {
            if (board[i][j] == 0 && board[i][j] == board[i + 1][j] && board[i][j] == board[i + 2][j] && board[i][j] == board[i + 3][j] && board[i - 1][j] == -1) {
                board[i - 1][j] = 1;
                return true;
            }
        }
    }


    // Kiểm tra hàng chéo từ trên bên trái xuống dưới bên phải
            /*
            0
             0
              0
               0
                0
            */
    for (int i = 0; i < row - 4; i++) {
        for (int j = 0; j < col - 4; j++) {
            if (board[i][j] == 0 && board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 2][j + 2] && board[i][j] == board[i + 3][j + 3] && board[i + 4][j + 4] == -1) {
                board[i + 4][j + 4] = 1;
                return true;
            }
        }
    }

    for (int i = 1; i < row - 3; i++) {
        for (int j = 1; j < col - 3; j++) {
            if (board[i][j] == 0 && board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 2][j + 2] && board[i][j] == board[i + 3][j + 3] && board[i - 1][j - 1] == -1) {
                board[i - 1][j - 1] = 1;
                return true;
            }
        }
    }


    // Kiểm tra hàng chéo ngược từ trên bên phải xuống dưới bên trái
             /*
                 0
                0
               0
              0
             0
             */

    for (int i = 0; i < row - 4; i++) {
        for (int j = row - 1; j >= 4; j--) {
            if (board[i][j] == 0 && board[i][j] == board[i + 1][j - 1] && board[i][j] == board[i + 2][j - 2] && board[i][j] == board[i + 3][j - 3] && board[i + 4][j - 4] == -1) {
                board[i + 4][j - 4] = 1;
                return true;
            }
        }
    }

    for (int i = 1; i < row - 3; i++) {
        for (int j = row - 2; j >= 3; j--) {
            if (board[i][j] == 0 && board[i][j] == board[i + 1][j - 1] && board[i][j] == board[i + 2][j - 2] && board[i][j] == board[i + 3][j - 3] && board[i - 1][j + 1] == -1) {
                board[i - 1][j + 1] = 1;
                return true;
            }
        }
    }

    return false;
}

//kiểm tra xem có 3 ô nào của người chơi không, nếu có thì thực hiện chặn ở 1 trong 2 đầu
bool Game::CheckPlayerWin3()
{

    //ngang
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col - 3; j++) {
            if (board[i][j] == 0 && board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2] && board[i][j + 3] == -1) {
                board[i][j + 3] = 1;
                return true;
            }
        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = 1; j < col - 2; j++) {
            if (board[i][j] == 0 && board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2] && board[i][j - 1] == -1) {
                board[i][j - 1] = 1;
                return true;
            }
        }
    }

    //doc

    for (int i = 0; i < row - 3; i++) {
        for (int j = 0; j < col; j++) {
            if (board[i][j] == 0 && board[i][j] == board[i + 1][j] && board[i][j] == board[i + 2][j] && board[i + 3][j] == -1) {
                board[i + 3][j] = 1;
                return true;
            }
        }
    }

    for (int i = 1; i < row - 2; i++) {
        for (int j = 0; j < col; j++) {
            if (board[i][j] == 0 && board[i][j] == board[i + 1][j] && board[i][j] == board[i + 2][j] && board[i - 1][j] == -1) {
                board[i - 1][j] = 1;
                return true;
            }
        }
    }


    // Kiểm tra hàng chéo từ trên bên trái xuống dưới bên phải
            /*
            0
             0
              0
               0
                0
            */
    for (int i = 0; i < row - 3; i++) {
        for (int j = 0; j < col - 3; j++) {
            if (board[i][j] == 0 && board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 2][j + 2] && board[i + 3][j + 3] == -1) {
                board[i + 3][j + 3] = 1;
                return true;
            }
        }
    }

    for (int i = 1; i < row - 2; i++) {
        for (int j = 1; j < col - 2; j++) {
            if (board[i][j] == 0 && board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 2][j + 2] && board[i - 1][j - 1] == -1) {
                board[i - 1][j - 1] = 1;
                return true;
            }
        }
    }


    // Kiểm tra hàng chéo ngược từ trên bên phải xuống dưới bên trái
             /*
                 0
                0
               0
              0
             0
             */

    for (int i = 0; i < row - 3; i++) {
        for (int j = row - 1; j >= 3; j--) {
            if (board[i][j] == 0 && board[i][j] == board[i + 1][j - 1] && board[i][j] == board[i + 2][j - 2] && board[i + 3][j - 3] == -1) {
                board[i + 3][j - 3] = 1;
                return true;
            }
        }
    }

    for (int i = 1; i < row - 2; i++) {
        for (int j = row - 2; j >= 2; j--) {
            if (board[i][j] == 0 && board[i][j] == board[i + 1][j - 1] && board[i][j] == board[i + 2][j - 2] && board[i - 1][j + 1] == -1) {
                board[i - 1][j + 1] = 1;
                return true;
            }
        }
    }



    return false;
}

//dành cho mode 3x3: kiểm tra xem có 2 ô nào chưa, nếu có thì chặn
bool Game::CheckPlayerWin2()
{
    // Kiểm tra hàng và cột
    for (int i = 0; i < 3; i++) {
        if ((board[i][0] == board[i][1] && board[i][0] == 0) || (board[i][1] == board[i][2] && board[i][1] == 0) || (board[i][0] == board[i][2] && board[i][0] == 0 ))
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == -1)
                {
                    board[i][j] = 1;
                    return true;
                }
            }

        }

        if ((board[0][i] == board[1][i] && board[0][i] == 0) || (board[1][i] == board[2][i] && board[1][i] == 0) || (board[0][i] == board[2][i] && board[0][i] == 0 )) 
        {
            for (int j = 0; j < 3; j++) {
                if (board[j][i] == -1) {
                    board[j][i] = 1;
                    return true;
                }
            }

        }
    }

    // Kiểm tra đường chéo
    if ((board[0][0] == board[1][1] && board[0][0] == 0) || (board[1][1] == board[2][2] && board[1][1] == 0) || (board[0][0] == board[2][2] && board[0][0] == 0 )) 
    {
        for (int i = 0; i < 3; i++) {
            if (board[i][i] == -1) {
                board[i][i] = 1;
                return true;
            }
        }

    }

if ((board[0][2] == board[1][1] && board[0][2] == 0) || (board[1][1] == board[2][0] && board[1][1] == 0) || (board[0][2] == board[2][0] && board[0][2] == 0)) {
    for (int i = 0; i < 3; i++) {
        if (board[i][2 - i] == -1) {
            board[i][2 - i] = 1;
            return true;
        }
    }

}

return false;
}

bool Game::CheckBotWin4() {
    //ngang
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col - 4; j++) {
            if (board[i][j] == 1 && board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2] && board[i][j] == board[i][j + 3] && board[i][j + 4] == -1) {
                board[i][j + 4] = 1;
                return true;
            }
        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = 1; j < col - 3; j++) {
            if (board[i][j] == 1 && board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2] && board[i][j] == board[i][j + 3] && board[i][j - 1] == -1) {
                board[i][j - 1] = 1;
                return true;
            }
        }
    }

    //doc

    for (int i = 0; i < row - 4; i++) {
        for (int j = 0; j < col; j++) {
            if (board[i][j] == 1 && board[i][j] == board[i + 1][j] && board[i][j] == board[i + 2][j] && board[i][j] == board[i + 3][j] && board[i + 4][j] == -1) {
                board[i + 4][j] = 1;
                return true;
            }
        }
    }

    for (int i = 1; i < row - 3; i++) {
        for (int j = 0; j < col; j++) {
            if (board[i][j] == 1 && board[i][j] == board[i + 1][j] && board[i][j] == board[i + 2][j] && board[i][j] == board[i + 3][j] && board[i - 1][j] == -1) {
                board[i - 1][j] = 1;
                return true;
            }
        }
    }


    // Kiểm tra hàng chéo từ trên bên trái xuống dưới bên phải
            /*
            0
             0
              0
               0
                0
            */
    for (int i = 0; i < row - 4; i++) {
        for (int j = 0; j < col - 4; j++) {
            if (board[i][j] == 1 && board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 2][j + 2] && board[i][j] == board[i + 3][j + 3] && board[i + 4][j + 4] == -1) {
                board[i + 4][j + 4] = 1;
                return true;
            }
        }
    }

    for (int i = 1; i < row - 3; i++) {
        for (int j = 1; j < col - 3; j++) {
            if (board[i][j] == 1 && board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 2][j + 2] && board[i][j] == board[i + 3][j + 3] && board[i - 1][j - 1] == -1) {
                board[i - 1][j - 1] = 1;
                return true;
            }
        }
    }


    // Kiểm tra hàng chéo ngược từ trên bên phải xuống dưới bên trái
             /*
                 0
                0
               0
              0
             0
             */

    for (int i = 0; i < row - 4; i++) {
        for (int j = row - 1; j >= 4; j--) {
            if (board[i][j] == 1 && board[i][j] == board[i + 1][j - 1] && board[i][j] == board[i + 2][j - 2] && board[i][j] == board[i + 3][j - 3] && board[i + 4][j - 4] == -1) {
                board[i + 4][j - 4] = 1;
                return true;
            }
        }
    }

    for (int i = 1; i < row - 3; i++) {
        for (int j = row - 2; j >= 3; j--) {
            if (board[i][j] == 1 && board[i][j] == board[i + 1][j - 1] && board[i][j] == board[i + 2][j - 2] && board[i][j] == board[i + 3][j - 3] && board[i - 1][j + 1] == -1) {
                board[i - 1][j + 1] = 1;
                return true;
            }
        }
    }

    return false;
}

bool Game::CheckBotWin2()
{

    // Kiểm tra hàng và cột
    for (int i = 0; i < 3; i++) {
        if ((board[i][0] == board[i][1] && board[i][0] == 1) || (board[i][1] == board[i][2] && board[i][1] == 1) || (board[i][0] == board[i][2] && board[i][0] == 1))
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == -1)
                {
                    board[i][j] = 1;
                    return true;
                }
            }

        }

        if ((board[0][i] == board[1][i] && board[0][i] == 1) || (board[1][i] == board[2][i] && board[1][i] == 1) || (board[0][i] == board[2][i] && board[0][i] == 1)) {
            for (int j = 0; j < 3; j++) {
                if (board[j][i] == -1) {
                    board[j][i] = 1;
                    return true;
                }
            }

        }
    }

    // Kiểm tra đường chéo
    if ((board[0][0] == board[1][1] && board[0][0] == 1) || (board[1][1] == board[2][2] && board[1][1] == 1) || (board[0][0] == board[2][2] && board[0][0] == 1)) {
        for (int i = 0; i < 3; i++) {
            if (board[i][i] == -1) {
                board[i][i] = 1;
                return true;
            }
        }

    }

    if ((board[0][2] == board[1][1] && board[0][2] == 1) || (board[1][1] == board[2][0] && board[1][1] == 1) || (board[0][2] == board[2][0] && board[0][2] == 1)) {
        for (int i = 0; i < 3; i++) {
            if (board[i][2 - i] == -1) {
                board[i][2 - i] = 1;
                return true;
            }
        }

    }

    return false;
}

bool Game::CheckBotWinMid5() {
    //ngang
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col - 4; j++) {
            if (board[i][j] == 1 && board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 3] && board[i][j] == board[i][j + 4] && board[i][j + 2] == -1) {
                board[i][j + 2] = 1;
                return true;
            }
        }
    }

    //doc
    for (int i = 0; i < row - 4; i++) {
        for (int j = 0; j < col; j++) {
            if (board[i][j] == 1 && board[i][j] == board[i + 1][j] && board[i][j] == board[i + 3][j] && board[i][j] == board[i + 4][j] && board[i + 2][j] == -1) {
                board[i + 2][j] = 1;
                return true;
            }
        }
    }

    //cheo trai tren xuong phai duoi
    for (int i = 0; i < row - 4; i++) {
        for (int j = 0; j < col - 4; j++) {
            if (board[i][j] == 1 && board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 3][j + 3] && board[i][j] == board[i + 4][j + 4] && board[i + 2][j + 2] == -1) {
                board[i + 2][j + 2] = 1;
                return true;
            }
        }
    }

    //cheo phai tren xuong trai duoi
    for (int i = 0; i < row - 4; i++) {
        for (int j = row - 1; j >= 4; j--) {
            if (board[i][j] == 1 && board[i][j] == board[i + 1][j - 1] && board[i][j] == board[i + 3][j - 3] && board[i][j] == board[i + 4][j - 4] && board[i + 2][j - 2] == -1) {
                board[i + 2][j - 2] = 1;
                return true;
            }
        }
    }


    return false;
}

bool Game::CheckPlayerWinMid5() {
    
    //ngang xx x
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col - 4; j++) {
            if (board[i][j] == 0 && board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 3] && board[i][j + 2] == -1) {
                board[i][j + 2] = 1;
                return true;
            }
        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col - 4; j++) {
            if (board[i][j] == 0 && board[i][j] == board[i][j + 2] && board[i][j] == board[i][j + 3] && board[i][j + 1] == -1) {
                board[i][j + 1] = 1;
                return true;
            }
        }
    }

    //doc
    for (int i = 0; i < row - 4; i++) {
        for (int j = 0; j < col; j++) {
            if (board[i][j] == 0 && board[i][j] == board[i + 1][j] && board[i][j] == board[i + 3][j] && board[i + 2][j] == -1) {
                board[i + 2][j] = 1;
                return true;
            }
        }
    }

    for (int i = 0; i < row - 4; i++) {
        for (int j = 0; j < col; j++) {
            if (board[i][j] == 0 && board[i][j] == board[i + 2][j] && board[i][j] == board[i + 3][j] && board[i + 1][j] == -1) {
                board[i + 1][j] = 1;
                return true;
            }
        }
    }


    //cheo trai tren xuong phai duoi
    for (int i = 0; i < row - 4; i++) {
        for (int j = 0; j < col - 4; j++) {
            if (board[i][j] == 0 && board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 3][j + 3] && board[i + 2][j + 2] == -1) {
                board[i + 2][j + 2] = 1;
                return true;
            }
        }
    }

    for (int i = 0; i < row - 4; i++) {
        for (int j = 0; j < col - 4; j++) {
            if (board[i][j] == 0 && board[i][j] == board[i + 2][j + 2] && board[i][j] == board[i + 3][j + 3] && board[i + 1][j + 1] == -1) {
                board[i + 1][j + 1] = 1;
                return true;
            }
        }
    }


    //cheo phai tren xuong trai duoi
    for (int i = 0; i < row - 4; i++){
        for (int j = row - 1; j >= 4; j--) {
            if (board[i][j] == 0 && board[i][j] == board[i + 1][j - 1] && board[i][j] == board[i + 3][j - 3] && board[i + 2][j - 2] == -1) {
                board[i + 2][j - 2] = 1;
                return true;
            }
        }
    }

    for (int i = 0; i < row - 4; i++) {
        for (int j = row - 1; j >= 4; j--) {
            if (board[i][j] == 0 && board[i][j] == board[i + 2][j - 2] && board[i][j] == board[i + 3][j - 3] && board[i + 1][j - 1] == -1) {
                board[i + 1][j - 1] = 1;
                return true;
            }
        }
    }

    return false;
}

// tìm ra nước đi tối ưu
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


void Game::Insert_MoveList(int y, int x)
{
    MoveListFor_Ctrl_Z[0][0] = MoveListFor_Ctrl_Z[1][0];
    MoveListFor_Ctrl_Z[0][1] = MoveListFor_Ctrl_Z[1][1];
    MoveListFor_Ctrl_Z[1][0] = y;
    MoveListFor_Ctrl_Z[1][1] = x;
}

void Game::Do_Ctrl_Z()
{
    if (MoveListFor_Ctrl_Z[1][0] != -1)
    {
        int YY = MoveListFor_Ctrl_Z[1][0];
        int XX = MoveListFor_Ctrl_Z[1][1];
        board[YY][XX] = -1;
        MoveListFor_Ctrl_Z[1][0] = MoveListFor_Ctrl_Z[0][0];
        MoveListFor_Ctrl_Z[1][1] = MoveListFor_Ctrl_Z[0][1];
        MoveListFor_Ctrl_Z[0][0] = -1;
        player--;
    }
}

// chế độ pvbot
void Game::GameMode1(SDL_Event e, int mouseXX, int mouseYY)
{
    if (winner == -1 && played)
    {
        if (player % 2 == 0)
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
                        board[X][Y] = player % 2;
                        PlayMedia(2);
                        player++;
                    }
                }
            }
        }
        else
        {
            botPlay(player % 2);
            PlayMedia(2);
            player++ ;
        }
        renderboard();
        winner = game_state();
    }
}

// chế độ pvp
void Game::GameMode2(SDL_Event e, int mouseXX, int mouseYY)
{

    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        mouseXX = e.button.x;
        mouseYY = e.button.y;

        updateMouse(mouseXX, mouseYY);

        if (selected[0] != -1)
        {
            int X = selected[0];
            int Y = selected[1];
            selected[0] = -1;
            if (board[Y][X] == -1)
            {
                Insert_MoveList(Y, X);
                board[Y][X] = player % 2;
                PlayMedia(2);
                player++;
            }
        }
    }
    renderboard();
    winner = game_state();
}

// hàm chạy của game
void Game::run()
{
    int mouseX = 0, mouseY = 0;
    bool endgameSound_played = 0;
    SDL_Event e;
    Init();
    playmusic();
    while (!quit)
    {

        while (SDL_PollEvent(&e) != 0) // vòng lặp chính của trò chơi
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }

            // đầu tiên nếu chưa chơi thì sẽ render ra màn hình menu
            if (!played && e.type == SDL_MOUSEMOTION)
                game_start(e);

            // sau đó chọn chế độ
            if (!boardMode && !mode && e.type == SDL_MOUSEBUTTONDOWN)
            {
                mouseX = e.motion.x;
                mouseY = e.motion.y;
                SetGameMode(mouseX, mouseY);
                if (mode == 3)
                {
                    break;
                }


                
                continue;
            }

            //sau đó là render ra màn hình chọn bàn chơi 3x3 hoặc 5x5 10x15
            if (mode && !played && e.type == SDL_MOUSEMOTION)
                menu_game_mode(e);

            //chọn bàn chơi 3x3 5x5 10x15 và thay đổi các thông số game cho phù hợp với bàn chơi
            if (mode && !boardMode && e.type == SDL_MOUSEBUTTONDOWN)
            {
                mouseX = e.motion.x;
                mouseY = e.motion.y;
                setBoardMode(mouseX, mouseY);// chon mode trong img
                ApplyBoardMode();
                continue;
            }

            // nếu nút restart được ấn thì khởi tạo lại toàn bộ thông số game
            if (restart)
            {
                init();
                endgameSound_played = 0;
                e.type = NULL;
            }
            //nếu replay được chọn thì khởi tạo lại toàn bộ thông số trong bàn chơi
            if (replay)
            {
                memset(board, -1, sizeof(board));
                memset(selected, -1, sizeof(selected));
                player = 0;
                replay = 1 - replay;
            }

            // thực hiện chơi chỉ khi đã chơi từ vòng lặp trước hoặc đã chọn thành công chế độ chơi & bàn chơi
            if (played || (mode && boardMode))
            {
                played = true;

                if (restart)// option choi lai
                {
                    init();
                    e.type = NULL;
                }

                //khi ấn nút replay
                if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                    mouseX = e.motion.x;
                    mouseY = e.motion.y;
                    if (mouseX > 30 && mouseX < 95 && mouseY >32 && mouseY < 90)
                    {
                        replay = true;
                        Load_image(boardMode + 9, 0, 0);
                    }

                //khi ấn nút home (restart)
                    if (mouseX > 120 && mouseX < 190 && mouseY >32 && mouseY < 90)
                    {
                        restart = true;
                        Load_image(0, 0, 0);
                        continue;
                    }
                }

                //khi ấn nút restart 
                if (winner != -1 && e.type == SDL_MOUSEBUTTONDOWN)
                {

                    mouseX = e.button.x;
                    mouseY = e.button.y;
                    if (mouseX >= 467 && mouseX <= 776 && mouseY >= 462 && mouseY <= 540)
                    {
                        restart = true;
                        Load_image(0, 0, 0);
                        continue;
                    }
                }

                //khi ấn nút chơi lại 1 nước
                if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                    mouseX = e.button.x;
                    mouseY = e.button.y;
                    if (mouseX >= 28 && mouseX <= 88 && mouseY >= 98 && mouseY <= 158)
                    {
                        Do_Ctrl_Z();
                    }

                }

                //nhận vào input lúc chơi game và khi chưa có người thắng
                if (winner == -1 && boardMode)
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

            //nếu đã có winner thì render ra màn hình winner và âm thanh
            if (winner != -1)
            {
                game_over(winner);
                if (endgameSound_played == 0)(winner == 2) ? PlayMedia(4) : PlayMedia(3);
                endgameSound_played = 1;
            }
            Renderer();
        }

    }

    close();
}