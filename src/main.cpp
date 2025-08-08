#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "chess.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

bool init();
void kill();
bool loop();

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;

bool init() {
     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
          fprintf(stderr, "%s %s\n", "SDL initialization failed: ", SDL_GetError());
          return false;
     }

     window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
     if (!window) {
          fprintf(stderr, "%s %s\n", "SDL creating window failed: ", SDL_GetError());
          return false;
     }

     renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
     if (!renderer) {
          fprintf(stderr, "%s %s\n", "SDL creating renderer failed: ", SDL_GetError());
          return false;
     }

     if(IMG_Init(IMG_INIT_PNG) < 0) {
          fprintf(stderr, "%s %s\n", "IMG_Init  failed: ", SDL_GetError());
          return false;
     }


     SDL_Surface* buffer = IMG_Load("assets/b-king.png");
     if(!buffer) {
          fprintf(stderr, "%s %s\n", "IMG_Load failed: ", SDL_GetError());
          return false;
     }

     texture = SDL_CreateTextureFromSurface(renderer, buffer);
     SDL_FreeSurface(buffer);
     buffer = NULL;
     if(!texture) {
          fprintf(stderr, "%s %s\n", "SDL_CreateTextureFromSurface with SVG Surface failed ", SDL_GetError());
          return false;
     }

     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
     SDL_RenderClear(renderer);
     return true;
}

void kill() {
     SDL_DestroyTexture(texture);
     SDL_DestroyRenderer(renderer);
     SDL_DestroyWindow(window);
     texture = NULL;
     window = NULL;
     renderer = NULL;
     IMG_Quit();
     SDL_Quit();
}

bool loop() {
     SDL_Event event;

     SDL_SetRenderDrawColor(renderer, 234, 233, 226, 0);
     SDL_RenderClear(renderer);

     int width, height;
     SDL_GetWindowSize(window, &width, &height);

     // Tiles
     for (size_t i = 0; i < 8; i++) {
          for (size_t j = 0; j < 8; j++) {
               SDL_Rect tileRect;
               tileRect.x = j * width / 8;
               tileRect.y = i * height / 8;
               tileRect.w = width / 8;
               tileRect.h = height / 8;


               if ((i + j) % 2 == 0) {
                    SDL_SetRenderDrawColor(renderer, 234, 233, 226, 255);
               } else {
                    SDL_SetRenderDrawColor(renderer, 38, 33, 35, 255);
               }
               SDL_RenderFillRect(renderer, &tileRect);
          }
     }

     int tileHeight = height/ 8;
     int tileWidth = width / 8;
     
     SDL_Rect dest;
     int squareSize = (tileWidth < tileHeight) ? tileWidth : tileHeight;
     dest.w = dest.h = squareSize;
     dest.x = (tileWidth - squareSize) / 2;
     dest.y = (tileHeight - squareSize) / 2;
     SDL_RenderCopy(renderer, texture, NULL, &dest);

     while (SDL_PollEvent(&event) != 0) {
          switch (event.type) {
               case SDL_QUIT:
                    return false;
               case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_q) {
                         return false;
                    }
                    break;
          }
     }

     SDL_RenderPresent(renderer);
     
     return true;
}

int main(int argc, char* argv) {
     // if (!init())
     //      return 1;

     // while(loop()) {
     //      SDL_Delay(10);
     // }

     Chess chessBoard;
     chessBoard.Run();

     // kill();
     return 0;


}
