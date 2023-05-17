#ifndef game_h_
#define game_h_


#define SCREEN_WIDTH 1240
#define SCREEN_HEIGHT 840
#define row 10
#define col 15
class Game
{
    bool quit = false;
    bool restart = true;
    bool played = false;
    int player = 0;
    int winner = -1;
    int board[row][col];// khoi tam mang 2 chieu
    int selected[2]; // toa do x, y cua nuoc di cua nguoi choi duoc ghi lai trong mang 2 chieu, [0] la X, [1] la y
    int x, y;
    int mode;
    
public:
    void init();
    void run();
    void setMode(int m);
private:
    void updateMouse(int mouseX, int mouseY);
    void update();
    void renderboard();
    int  game_state();
    int  bot(int turn);
};
#endif //game_h_h