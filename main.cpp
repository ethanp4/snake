#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <chrono>
#include <random>
#include <iostream>
#include <SDL2/SDL.h>
#include "Vector2.h"

using namespace std;

Vector2 movementDir;
bool running;
Vector2 playerPos;

enum objects { NONE, PLAYER, FOOD };
const Vector2 res = { 40,40 };
int playField[40][40];

random_device dev;
mt19937 rng(dev());
uniform_int_distribution<mt19937::result_type> distarea(1,res.x*res.y);
uniform_int_distribution<mt19937::result_type> distlen(1, res.x);

Uint32 move_player(Uint32 i, void *param) {
  playField[playerPos.x][playerPos.y] = NONE;
  playerPos = playerPos + movementDir;
  playField[playerPos.x][playerPos.y] = PLAYER;
  // cout << playerPos.toString() << endl;
  return i;
}

Uint32 spawn_food(Uint32 i, void *param) {
  Vector2 pos = {(int)distlen(rng), (int)distlen(rng)};
  playField[pos.x][pos.y] = FOOD;
  cout << "Spawning food at " << pos.toString() << endl;
  return i;
}

void draw_pixels(SDL_Renderer &renderer) {
  SDL_Colour colour;
  for (int i = 0; i < res.x; i++) {
    for (int j = 0; j < res.y; j++){
      switch (playField[i][j]) {
        case NONE:
          continue;
        break;
        case PLAYER:
          colour = {255,255,255,255};
        break;
        case FOOD:
          colour = {26, 209, 180, 255};
        break;      
      }
      SDL_SetRenderDrawColor(&renderer, colour.r, colour.g, colour.b, colour.a);
      SDL_RenderDrawPoint(&renderer, i, j);
    }
  }
}

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  auto window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, res.x*16, res.y*16, 0);
  auto renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_RenderSetScale(renderer,16,16);
  SDL_Event e;

  for (int i = 0; i < res.x; i++) {
    for (int j = 0; j < res.y; j++) {
      //approximately 10 will begin with food
      if (distarea(rng) < 10){
        playField[i][j] = FOOD;
      } else {
        playField[i][j] = NONE;
      }
      // cout << playField[i][j];
    }
    // cout << endl;
  }
  movementDir = 1;
  running = true;
  playerPos = (Vector2) {res.x/2,res.y/2};
  printf("Running\n");

  auto moveTimer = SDL_AddTimer(100, move_player, NULL);
  auto spawnFoodTimer = SDL_AddTimer(5000, spawn_food, NULL);

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
              movementDir = Vector2::UP;
            break;
            case SDLK_DOWN:
              movementDir = Vector2::DOWN;
            break;
            case SDLK_LEFT:
              movementDir = Vector2::LEFT;
            break;
            case SDLK_RIGHT:
              movementDir = Vector2::RIGHT;
            break;
          }
          // cout << "Movement dir is " << movementDir.toString() << endl;
        break;
      }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_pixels(*renderer);

    SDL_RenderPresent(renderer);

    SDL_Delay(10);
  }
}