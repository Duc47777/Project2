﻿#ifndef game_h_
#define game_h_

#include <SDL.h>

#define SCREEN_WIDTH 1240
#define SCREEN_HEIGHT 840

class Game
{
    bool quit = false;
    bool restart = true;
    bool replay = false;
    bool played = false;
    int player = 0;
    int winner = -1;
    int boardMode = 0;
    int row = 5, col = 5;
    int board[10][15];// khoi tam mang 2 chieu
    int selected[2]; // toa do x, y cua nuoc di cua nguoi choi duoc ghi lai trong mang 2 chieu, [0] la X, [1] la y
    int x, y;
    int mode = 0;
    int ChenhLechXO=0;
    float DoDaiCanh = 0;
    int ChechLechX = 0, ChechLechY = 0;
    int MoveListFor_Ctrl_Z[2][2] = { -1,-1,-1,-1 };

public:
    void init();
    void run();

    void game_start(SDL_Event e);
    void menu_game_mode(SDL_Event e);
    void SetGameMode(int x, int y);
    void setBoardMode(int x, int y);
    void ApplyBoardMode();
    void PlayMedia(int a, int b=1);
    void Insert_MoveList(int x, int y);
    void Do_Ctrl_Z();

    void botPlay(int player);
    int  minimax(int depth,int alpha, int beta, bool maximizingPlayer);
    //hàm kiểm tra để bot chặn nước đi người chơi
    bool CheckPlayerWin2();
    bool CheckPlayerWin3();
    bool CheckPlayerWin4();
    bool CheckPlayerWinMid5();

    //hàm ưu tiên nước đi của bot
    bool CheckBotWin2();
    bool CheckBotWin4();
    bool CheckBotWinMid5();
    
    void GameMode1(SDL_Event e, int mouseXX, int mouseYY);
    void GameMode2(SDL_Event e, int mouseXX, int mouseYY);

private:
    void updateMouse(int mouseX, int mouseY);
    void renderboard();
    int  game_state();
};
#endif //game_h_h