#include <iostream>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


void log_sdl_error(const std::string &msg) {
  std::cout << msg << " error: " << SDL_GetError() << std::endl;
}


int main (int, char**) {
  SDL_Window *win;
  SDL_Renderer *ren;
  SDL_Event event;
  bool quit = false;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    log_sdl_error("SDL_Init");
    return 1;
  }

  win = SDL_CreateWindow("Mandelbrot set",
                         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         SCREEN_WIDTH, SCREEN_HEIGHT,
                         SDL_WINDOW_SHOWN);
  if (win == nullptr) {
    log_sdl_error("SDL_CreateWindow");
    SDL_Quit();
    return 1;
  }

  ren = SDL_CreateRenderer(win, -1, (SDL_RENDERER_ACCELERATED |
                                     SDL_RENDERER_PRESENTVSYNC));

  if (ren == nullptr) {
    SDL_DestroyWindow(win);
    log_sdl_error("SDL_CreateRenderer");
    SDL_Quit();
    return 1;
  }

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      }

      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          quit = true;
          break;
        default:
          break;
        }
      }

      if (event.type == SDL_MOUSEBUTTONDOWN) {
        quit = true;
      }
    }

    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_RenderDrawPoint(ren, 400, 300);
    SDL_RenderPresent(ren);
  }

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
