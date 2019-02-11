#include <iostream>
#include <SDL.h>
#include <Box2D/Box2D.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define BAR_WIDTH 30
#define BAR_HEIGHT 120
#define BAR_SPEED 5

#define BALL_SIZE 20
#define BALL_SPEED 4

using namespace std;

typedef struct NormalSpeed NormalSpeed;
struct NormalSpeed {
    int x;
    int y;
};

void reverse(int *num) {
    if(*num == 1)
        *num = -1;
    else
        *num = 1;
}

int main(int argc, char *args[]) {

    SDL_Window *window = nullptr;
    SDL_Surface *screen = nullptr;

    SDL_Surface *player = nullptr;
    SDL_Rect playerPos;
    playerPos.x = 10;
    playerPos.y = SCREEN_HEIGHT / 2 - BAR_HEIGHT / 2;

    SDL_Surface *enemy = nullptr;
    SDL_Rect enemyPos;
    enemyPos.x = SCREEN_WIDTH - BAR_WIDTH - 10;
    enemyPos.y = SCREEN_HEIGHT / 2 - BAR_HEIGHT / 2;
    int enemyNormalSpeed = 1;


    SDL_Surface *ball = nullptr;
    SDL_Rect ballPos;
    ballPos.x = SCREEN_WIDTH / 2;
    ballPos.y = SCREEN_HEIGHT / 2;
    NormalSpeed ballNormalSpeed;
    ballNormalSpeed.x = 1;
    ballNormalSpeed.y = 1;


    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    screen = SDL_GetWindowSurface(window);

    bool quit = false;
    SDL_Event e;


    // Surface creation
    player = SDL_CreateRGBSurface(0, BAR_WIDTH, BAR_HEIGHT, 32, 0, 0, 0, 0);
    ball = SDL_CreateRGBSurface(0, BALL_SIZE, BALL_SIZE, 32, 0, 0, 0, 0);
    enemy = SDL_CreateRGBSurface(0, BAR_WIDTH, BAR_HEIGHT, 32, 0, 0, 0, 0);

    // Colors creation
    Uint32 backgroundColor = SDL_MapRGB(screen->format, 17, 206, 112);
    Uint32 white = SDL_MapRGB(player->format, 255, 255, 255);



    while(!quit) {

        // Events
        while(SDL_PollEvent(&e) != 0) {

            switch (e.type) {

                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }

        // Keyboard state (Il vaut mieux utiliser cela plutot que les evenement)
        // Les evenements du clavier sont entièrement gérés par l'OS et sur Windows, il y a une pause lors d'un appui long
        // On peut aussi gerer plusieurs appuis en même temps
        SDL_PumpEvents();

        if(keyboardState[SDL_SCANCODE_UP])
            playerPos.y -= BAR_SPEED;

        else if(keyboardState[SDL_SCANCODE_DOWN] && playerPos.y + BAR_HEIGHT < SCREEN_HEIGHT)
            playerPos.y += BAR_SPEED;


        // Enemy calculations

        if(ballPos.x > SCREEN_WIDTH / 3) {
            enemyNormalSpeed = enemyPos.y < ballPos.y ? 1 : -1;
            enemyPos.y += enemyNormalSpeed * BAR_SPEED;
        }

        if(enemyPos.y < 0) {
            enemyPos.y = 0;
            enemyNormalSpeed = 1;
        }

        else if(enemyPos.y + BAR_HEIGHT > SCREEN_HEIGHT) {
            enemyPos.y = SCREEN_HEIGHT - BAR_HEIGHT;
            enemyNormalSpeed = -1;
        }

        // Ball Calculations
        ballPos.x += ballNormalSpeed.x * BALL_SPEED;
        ballPos.y += ballNormalSpeed.y * BALL_SPEED;

        if(ballPos.x >= SCREEN_WIDTH - BALL_SIZE)
            reverse(&ballNormalSpeed.x);

        else if(ballPos.x <= 0)
            quit = true;
            //reverse(&ballNormalSpeed.x);

        if(ballPos.y >= SCREEN_HEIGHT - BALL_SIZE)
            reverse(&ballNormalSpeed.y);

        else if(ballPos.y <= 0)
            reverse(&ballNormalSpeed.y);


        // Collision
        if(ballPos.x <= playerPos.x + BAR_WIDTH && ballPos.y >= playerPos.y && ballPos.y <= playerPos.y + BAR_HEIGHT)
            reverse(&ballNormalSpeed.x);


        else if(ballPos.x + BALL_SIZE >= enemyPos.x && ballPos.y >= enemyPos.y && ballPos.y <= enemyPos.y + BAR_HEIGHT)
            reverse(&ballNormalSpeed.x);


        // Graphic

        // Background
        SDL_FillRect(screen, nullptr, backgroundColor);

        // Player
        SDL_FillRect(player, nullptr, white);
        SDL_BlitSurface(player, nullptr, screen, &playerPos);

        // Enemy
        SDL_FillRect(enemy, nullptr, white);
        SDL_BlitSurface(enemy, nullptr, screen, &enemyPos);

        // Ball
        SDL_FillRect(ball, nullptr, white);
        SDL_BlitSurface(ball, nullptr, screen, &ballPos);

        // Update
        SDL_UpdateWindowSurface(window);
        SDL_Delay(17);
    }

    SDL_FreeSurface(player);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}