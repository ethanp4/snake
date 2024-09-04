#include <SDL2/SDL.h>
#include "SDL2/SDL_ttf.h"
// #include <SDL2/SDL_render.h>
// #include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <random>
#include <iostream>
#include <vector>
#include "classes/Vector2.h"

using namespace std;

enum objects { NONE, PLAYER, FOOD };

//global stuff
bool running = true;
bool gameOver = false;
Vector2 movementDir;
Vector2 playerPos;
int foodCount = 0;
int playerLength = 0;
vector<Vector2> positionHistory;
const Vector2 res = { 40,40 };
int playField[40][40];

vector<SDL_TimerID> timers;

//rng stuff
random_device dev;
mt19937 rng(dev());
uniform_int_distribution<mt19937::result_type> distdir(0,3);
uniform_int_distribution<mt19937::result_type> distarea(1,res.x*res.y);
uniform_int_distribution<mt19937::result_type> distlen(1, res.x);

void quit_game() {
  running = false;
  for (int i = 0; i < timers.size(); i++) {
    SDL_RemoveTimer(timers[i]);
  }
  cout << "Cleanly(?) exited" << endl; 
}

//timer'd functions
Uint32 move_player(Uint32 i, void *param) {
  playField[playerPos.x][playerPos.y] = NONE;
  //add the previous position to the start of the vector
  positionHistory.insert(positionHistory.begin(), playerPos);

  playerPos = playerPos + movementDir;
  if (playerPos.x > 39) {
    playerPos.x = 0;
  }
  if (playerPos.x < 0) {
    playerPos.x = 39;
  }
  if (playerPos.y > 39) {
    playerPos.y = 0;
  }
  if (playerPos.y < 0) {
    playerPos.y = 39;
  }

  switch (playField[playerPos.x][playerPos.y]) {
    case NONE:
    break;
    case PLAYER:
      cout << "Game over" << endl;
      gameOver = true;
    break;
    case FOOD:
      playerLength++;
      foodCount--;
      cout << "Length is " << playerLength << endl;
    break;
  }
  playField[playerPos.x][playerPos.y] = PLAYER;

  //limit the size of the history
  if (positionHistory.size() > playerLength) {
    auto end = positionHistory.back();
    playField[end.x][end.y] = NONE;
    positionHistory.pop_back();
  }

  //update playfield array
  for (int i = 0; i < positionHistory.size(); i++) {
    auto pos = positionHistory[i];
    playField[pos.x][pos.y] = PLAYER;
  }
  // cout << playerPos.toString() << endl;
  return i;
}

Uint32 spawn_food(Uint32 i, void *param) {
  do {
    Vector2 pos = {(int)distlen(rng), (int)distlen(rng)};
    if (playField[pos.x][pos.y] != PLAYER) {
      playField[pos.x][pos.y] = FOOD;
      foodCount++;
      cout << "Spawning food at " << pos.toString() << endl;
      cout << "Foodcount is " << foodCount << endl;
    }
    //if food is low then spawn more
  } while (foodCount < 7);
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
        if (gameOver) {
          colour = {255,10,10,10};
        } else {
          colour = {255,255,255,255};
        }
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
  //initialize sdl stuff
  SDL_Init(SDL_INIT_EVERYTHING);
  auto window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, res.x*16, res.y*16, 0);
  auto renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_RenderSetScale(renderer,16,16);
  SDL_Event e;

  //text
  TTF_Font* font = TTF_OpenFont("fonts/Tiny5-Regular.ttf", 7);
  SDL_Surface* gameoverSurface = TTF_RenderText_Solid(font, "Gameover", {255,255,255});
  SDL_Texture* gameoverTexture = SDL_CreateTextureFromSurface(renderer, gameoverSurface);
  SDL_Rect gameoverRect;
  gameoverRect.x = 0;
  gameoverRect.y = 0;
  gameoverRect.w = 40;
  gameoverRect.h = 40;

  //initialize playfield
  for (int i = 0; i < res.x; i++) {
    for (int j = 0; j < res.y; j++) {
      //approximately 10 will begin with food
      if (distarea(rng) < 7){
        playField[i][j] = FOOD;
        foodCount++;
      } else {
        playField[i][j] = NONE;
      }
    }
  }
  //select starting direction and position
  switch(distdir(rng)) {
    case 0:
      movementDir = Vector2::UP;
    break;
    case 1:
      movementDir = Vector2::DOWN;
    break;
    case 2:
      movementDir = Vector2::LEFT;
    break;
    case 3:
      movementDir = Vector2::RIGHT;
    break;
  }
  playerPos = (Vector2) {(int)distlen(rng), (int)distlen(rng)};

  //create timers
  auto moveTimer = SDL_AddTimer(100, move_player, NULL);
  timers.push_back(moveTimer);
  auto spawnFoodTimer = SDL_AddTimer(5000, spawn_food, NULL);
  timers.push_back(spawnFoodTimer);
  
  printf("Running\n");
  //main loop
  while (running) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        //exit 
        case SDL_QUIT:
          quit_game();
        break;

        //keypress
        case SDL_KEYDOWN:
          switch(e.key.keysym.sym) {
            case SDLK_ESCAPE:
              quit_game();
            break;
            //movement
            case SDLK_UP:
              //prevent turning backwards
              if (movementDir != Vector2::DOWN){
                movementDir = Vector2::UP;
              }
            break;
            case SDLK_DOWN:
              if (movementDir != Vector2::UP){
                movementDir = Vector2::DOWN;
              }
            break;
            case SDLK_LEFT:
              if (movementDir != Vector2::RIGHT) {
                movementDir = Vector2::LEFT;
              } 
            break;
            case SDLK_RIGHT:
              if (movementDir != Vector2::LEFT) {
                movementDir = Vector2::RIGHT;
              }
            break;
          }
        break;
      }
    }
    //clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //draw objects
    draw_pixels(*renderer);
    SDL_RenderCopy(renderer, gameoverTexture, NULL, &gameoverRect);
    if (gameOver) { 
      movementDir = {0,0}; 
      // SDL_FreeSurface(gameoverSurface);
      // SDL_DestroyTexture(gameoverTexture);
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
  }
}