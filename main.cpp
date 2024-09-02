#include <iostream>
#include <SDL2/SDL.h>
using namespace std;

struct Vector2 {
  float x;
  float y;
};
#define UP (Vector2) {0,1}
#define DOWN (Vector2) {0,-1}
#define LEFT (Vector2) {-1,0}
#define RIGHT (Vector2) {1,0}

int main() {
  const Vector2 res = { 640,480 };

  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_Event e;
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(res.x, res.y, 0, &window, &renderer);

  bool running = true;
  Vector2 movementdir = (Vector2) {0,0};
  while (running) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        //exit 
        case SDL_QUIT:
          running = false;
        break;

        //keypress
        case SDL_KEYDOWN:
          switch(e.key.keysym.sym) {
            case SDLK_UP:
              movementdir = UP;
            break;
            case SDLK_DOWN:
              movementdir = DOWN;
            break;
            case SDLK_LEFT:
              movementdir = LEFT;
            break;
            case SDLK_RIGHT:
              movementdir = RIGHT;
            break;
          }

      }
    }
    SDL_Delay(10);
  }
}