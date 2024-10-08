#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <random>
#include <iostream>
#include <vector>
#include "../include/vector2.h"
#include "../include/web-requests.h"
#include "../include/credentials.h"

using namespace std;

enum objects { NONE, PLAYER, FOOD };

bool window_resized = false;
//global stuff
bool running = true;
bool gameOver = false;
Vector2 movementDir;
Vector2 playerPos;
int foodCount = 0;
int playerLength = 0;
vector<Vector2> positionHistory;

//constants
const int res = 600;
const int resScale = 1;
const int pixelSize = 18;
const int playFieldSize = res / pixelSize;
int playField[playFieldSize][playFieldSize];

const int minFood = 7;
const int approxStartingFood = 7;

vector<SDL_TimerID> timers;

//rng stuff
random_device dev;
mt19937 rng(dev());
uniform_int_distribution<mt19937::result_type> distdir(0, 3);
uniform_int_distribution<mt19937::result_type> distarea(1, playFieldSize*playFieldSize);
uniform_int_distribution<mt19937::result_type> distlen(1, playFieldSize);

TTF_Font* bigFont;
TTF_Font* littleFont;

void quit_game(SDL_Window *window, SDL_Renderer* renderer) {
  running = false;
  for (int i = 0; i < (int)timers.size(); i++) {
    SDL_RemoveTimer(timers[i]);
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  cout << "Cleanly(?) exited" << endl;
}

//timer'd functions
Uint32 move_player(Uint32 interval, void *param) {
  playField[playerPos.x][playerPos.y] = NONE;
  //add the previous position to the start of the vector
  positionHistory.insert(positionHistory.begin(), playerPos);

  //calculate future position then wrap around if necessary
  playerPos = playerPos + movementDir;
  if (playerPos.x > playFieldSize-1) {
    playerPos.x = 0;
  }
  if (playerPos.x < 0) {
    playerPos.x = playFieldSize-1;
  }
  if (playerPos.y > playFieldSize-1) {
    playerPos.y = 0;
  }
  if (playerPos.y < 0) {
    playerPos.y = playFieldSize-1;
  }

  //switch for collision detection
  switch (playField[playerPos.x][playerPos.y]) {
    case NONE:
    break;
    case PLAYER:
      cout << "Game over" << endl;
      gameOver = true;
      if (authIncluded){
        cout << "Uploading score" << endl;
        postScore(playerLength);
      } else {
        cout << "Credentials not included" << endl;
      }
    break;
    case FOOD:
      playerLength++;
      foodCount--;
      cout << "Length is " << playerLength << endl;
    break;
  }
  playField[playerPos.x][playerPos.y] = PLAYER;

  //limit the size of the history
  if ((int)positionHistory.size() > playerLength) {
    auto end = positionHistory.back();
    playField[end.x][end.y] = NONE;
    positionHistory.pop_back();
  }

  //update playfield array
  for (int i = 0; i < (int)positionHistory.size(); i++) {
    auto pos = positionHistory[i];
    playField[pos.x][pos.y] = PLAYER;
  }
  // cout << playerPos.toString() << endl;
  return interval;
}

Uint32 spawn_food(Uint32 interval, void *param) {
  //stop spawning while gameover
  if (gameOver) {
    return interval;
  }
  do {
    Vector2 pos = {(int)distlen(rng), (int)distlen(rng)};
    if (playField[pos.x][pos.y] != PLAYER) {
      playField[pos.x][pos.y] = FOOD;
      foodCount++;
      cout << "Spawning food at " << pos.toString() << endl;
      cout << "Foodcount is " << foodCount << endl;
    }
    //if food is low then spawn more
  } while (foodCount < minFood);
  return interval;
}

void draw_pixels(SDL_Renderer *renderer) {
  SDL_Colour colour;
  for (int i = 0; i < playFieldSize; i++) {
    for (int j = 0; j < playFieldSize; j++){
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
        //this case shouldnt ever happen
        default:
          colour = {0,0,0,0};
      }
      SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
      SDL_Rect pixel = {i*pixelSize, j*pixelSize, pixelSize, pixelSize};
      SDL_RenderFillRect(renderer, &pixel);
      // SDL_RenderDrawPoint(&renderer, i, j);
    }
  }
}

void render_text(SDL_Renderer *renderer) {
  SDL_Surface* scoreSurface = TTF_RenderText_Solid(littleFont, to_string(playerLength).c_str(), {255,255,255});
  SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
  int scoreTexW, scoreTexH;
  SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreTexW, &scoreTexH);
  SDL_Rect scoreRect = { 0, 0, scoreTexW, scoreTexH};
  SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

  SDL_FreeSurface(scoreSurface);
  SDL_DestroyTexture(scoreTexture);

  if (gameOver) {
    SDL_Surface* gameoverSurface = TTF_RenderText_Solid(bigFont, "Gameover", {255,255,255});
    SDL_Texture* gameoverTexture = SDL_CreateTextureFromSurface(renderer, gameoverSurface);
    int texW, texH;
    SDL_QueryTexture(gameoverTexture, NULL, NULL, &texW, &texH);
    SDL_Rect gameoverRect = { (res-texW)/2, res/2-texH, texW, texH };
    SDL_Surface* infoSurface = TTF_RenderText_Solid(littleFont, "Press space to restart", {255,255,255});
    SDL_Texture* infoTexture = SDL_CreateTextureFromSurface(renderer, infoSurface);
    int infoTexW, infoTexH;
    SDL_QueryTexture(infoTexture, NULL, NULL, &infoTexW, &infoTexH);
    SDL_Rect infoRect = { (res-infoTexW)/2, res/2, infoTexW, infoTexH };

    SDL_RenderCopy(renderer, gameoverTexture, NULL, &gameoverRect);
    SDL_RenderCopy(renderer, infoTexture, NULL, &infoRect);

    SDL_DestroyTexture(gameoverTexture);
    SDL_DestroyTexture(infoTexture);

    SDL_FreeSurface(gameoverSurface);
    SDL_FreeSurface(infoSurface);
  }
}

Vector2 getRandomDir() {
    switch(distdir(rng)) {
    case 0:
      return Vector2::UP;
    break;
    case 1:
      return Vector2::DOWN;
    break;
    case 2:
      return Vector2::LEFT;
    break;
    case 3:
      return Vector2::RIGHT;
    break;
    //suppress a warning
    default:
      return Vector2::UP;
  }
}

void initPlayfield() {
  //initialize playfield
  for (int i = 0; i < playFieldSize; i++) {
    for (int j = 0; j < playFieldSize; j++) {
      if (distarea(rng) < approxStartingFood){
        playField[i][j] = FOOD;
        foodCount++;
      } else {
        playField[i][j] = NONE;
      }
    }
  }
}

void restartGame() {
  gameOver = false;
  foodCount = 0;
  playerLength = 0;
  initPlayfield();
  movementDir = getRandomDir();
  positionHistory.clear();
}

int main() {
  //initialize sdl stuff
  SDL_Init(SDL_INIT_EVERYTHING);
  auto window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, res*resScale, res*resScale, 0);
  auto renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_RenderSetScale(renderer,resScale,resScale);
  SDL_Event e;
  SDL_SetWindowResizable(window, SDL_TRUE);
  //text
  TTF_Init();
  //this file path isnt reliable
  bigFont = TTF_OpenFont("../fonts/Tiny5-Regular.ttf", 72);
  littleFont = TTF_OpenFont("../fonts/Tiny5-Regular.ttf", 36);

  initPlayfield();
  movementDir = getRandomDir();
  //select starting direction and position

  playerPos = {(int)distlen(rng), (int)distlen(rng)};

  //create timers
  auto moveTimer = SDL_AddTimer(100, move_player, NULL);
  timers.push_back(moveTimer);
  auto spawnFoodTimer = SDL_AddTimer(5000, spawn_food, NULL);
  timers.push_back(spawnFoodTimer);

  cout << "Running" << endl;

  initRequests();
  // postScore(10);
  // getLeaderboard();

  //main loop
  while (running) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_WINDOWEVENT_RESIZED:
        case SDL_WINDOWEVENT_SIZE_CHANGED:
          window_resized = true;
        break;
        //exit
        case SDL_QUIT:
          quit_game(window, renderer);
        break;

        //keypress
        case SDL_KEYDOWN:
          switch(e.key.keysym.sym) {
            case SDLK_ESCAPE:
              quit_game(window, renderer);
            break;
            //movement
            case SDLK_SPACE:
              if (gameOver) {
                restartGame();
              }
            break;
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
    if(window_resized) {
      cout << "window resized!" << endl;
      window_resized = false;
    }
    //clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //draw objects
    draw_pixels(renderer);
    render_text(renderer);

    if (gameOver) {
      movementDir = {0,0};
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(8);
  }
}
