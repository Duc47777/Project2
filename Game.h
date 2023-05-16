#ifndef game_h_
#define game_h_


#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500
#define N 5

class Game
{
    bool quit = false;
    bool restart = true;
    bool played = false;
    int player = 0;
    int winner = -1;
    int board[N][N];
    int selected[2];
    int x, y;
public:
    void init();
    void run();
private:
    void updateMouse(int mouseX, int mouseY);
    void update();
    void render();
    int game_state();
    int bot(int turn);
};
#endif //game_h_h