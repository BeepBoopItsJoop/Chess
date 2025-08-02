#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "controller.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

int main(void) {
     SDL_Surface* winSurface = NULL;
     SDL_Window* window = NULL;

     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
          fprintf(stderr, "%s %s\n", "SDL initialization failed: ", SDL_GetError());
          return 1;
     }
     
     window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
     if (!window) {
          fprintf(stderr, "%s %s\n", "SDL creating window failed: ", SDL_GetError());
          return 1;
     }

     winSurface = SDL_GetWindowSurface(window);
     if (!winSurface) {
          fprintf(stderr, "%s %s\n", "SDL getting window surface failed: ", SDL_GetError());
          return 1;
     }
     
     // white Screen
     SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 255, 255, 255));
     

     // Tiles
     for(size_t i = 0; i < 8; i++) {
          for(size_t j = 0; j < 8; j++) {
               SDL_Rect tileRect;
               tileRect.y = i * WINDOW_HEIGHT/8;
               tileRect.x = j * WINDOW_WIDTH/8;
               tileRect.w = WINDOW_WIDTH/8;
               tileRect.h = WINDOW_HEIGHT/8;

               Uint32 currTileMapRGB;
               if((i + j) % 2 == 0) {
                    currTileMapRGB = SDL_MapRGB(winSurface->format, 234, 233, 226);
               } else {
                    currTileMapRGB = SDL_MapRGB(winSurface->format, 38, 33, 35);
               }

               SDL_FillRect(winSurface, &tileRect, currTileMapRGB);
          }
     }
     SDL_UpdateWindowSurface( window );




     SDL_Event event;
     int running = 1;
     while (running) {
          while (SDL_PollEvent(&event)) {
               switch (event.type) {
                    case SDL_QUIT:
                         running = false;  
                    break;
                    case SDL_KEYDOWN:
                         if(event.key.keysym.sym == SDLK_q) {
                              running = false;
                         }
                    break;
               }
          }
     }

     // Board chessBoard;
     // gameStart(&chessBoard);

     SDL_DestroyWindow(window);
     window = NULL;
     winSurface = NULL;

     SDL_Quit();
     return 0;
}
