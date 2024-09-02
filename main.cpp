#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include "Vector2.h"
using namespace std;

enum DIRECTION {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

const Vector2 directions[] = {
  {0,-1},//DOWN
  {0,1}, //UP
  {-1,0},//LEFT
  {1,0}  //RIGHT
};

int movementDir;
bool running;
Vector2 playerPos;

void move_player() {
  auto delta = directions[movementDir];
  playerPos = playerPos + delta;
}

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  const Vector2 res = { 100,100 };
  // create 600x600 window, which is actually 100x100 pixels scaled by 6x
  SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, res.x*6, res.y*6, 0);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_RenderSetScale(renderer,6,6);
  SDL_Event e;

  movementDir = 1;
  running = true;
  playerPos = (Vector2) {res.x/2,res.y/2};
  printf("Running\n");

  // Vector2 a = Vector2(5,2);
  // Vector2 b = Vector2(2,2);
  // Vector2 r = a+b;
  // printf("%s + %s = %s\n", a.toString().c_str(), b.toString().c_str(), r.toString().c_str());  

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
            case SDLK_ESCAPE:
              running = false;
            break;
            //movement
            case SDLK_UP:
              movementDir = UP;
            break;
            case SDLK_DOWN:
              movementDir = DOWN;
            break;
            case SDLK_LEFT:
              movementDir = LEFT;
            break;
            case SDLK_RIGHT:
              movementDir = RIGHT;
            break;
          }
          printf("Movement direction: %i\n", movementDir);
        break;
      }
    }
    move_player();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(renderer, playerPos.x, playerPos.y);
    
    SDL_RenderPresent(renderer);
    SDL_Delay(100);
  }
}