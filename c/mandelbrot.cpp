#include <iostream>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;


void log_sdl_error(const std::string &msg) {
  std::cout << msg << " error: " << SDL_GetError() << std::endl;
}


void render_mandelbrot(SDL_Renderer *ren, double viewport_size, double center_x, double center_y, int max_iterations) {
  int i, j, index;
  double x, y, z, zi, newz, newzi;
  for (j=0; j < SCREEN_HEIGHT; j++) {
    for (i=0; i < SCREEN_WIDTH; i++) {
      // transform pixel coordinates to viewport coordinates
      x = center_x - 0.5 * viewport_size + ((float)i / SCREEN_WIDTH) * viewport_size;
      y = center_y - 0.5 * viewport_size + ((float)j / SCREEN_HEIGHT) * viewport_size;

      // compute the mandelbrot formula
      index = 0;
      z = 0.0;
      zi = 0.0;
      while (index < max_iterations) {
        newz = (z * z) - (zi * zi) + x;
        newzi = 2 * z * zi + y;
        z = newz;
        zi = newzi;
        if (((z * z) + (zi * zi)) > 4) {
          break;
        }
        index++;
      }

      SDL_SetRenderDrawColor(ren, index % 255, 0, 0, 255);
      SDL_RenderDrawPoint(ren, i, j);
    }
  }
  SDL_RenderPresent(ren);
}


int main (int, char**) {
  SDL_Window *win;
  SDL_Renderer *ren;
  SDL_Event event;
  bool quit = false;
  double viewport_size;
  double x;
  double y;
  int max_iterations;

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

  viewport_size = 4.0;
  x = 0.0;
  y = 0.0;
  max_iterations = 256;
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
        case SDLK_UP:
          y -= 0.1 * viewport_size;
          break;
        case SDLK_DOWN:
          y += 0.1 * viewport_size;
          break;
        case SDLK_LEFT:
          x -= 0.1 * viewport_size;
          break;
        case SDLK_RIGHT:
          x += 0.1 * viewport_size;
          break;
        default:
          break;
        }
      }

      if (event.type == SDL_MOUSEBUTTONDOWN) {
        quit = true;
      }
    }
    render_mandelbrot(ren, viewport_size, x, y, max_iterations);
    viewport_size = 0.9 * viewport_size;
    max_iterations++;
  }

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
