#include <math.h>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int MAX_ITERATIONS = 128;

typedef struct {
  double r;
  double g;
  double b;
} rgb;


void log_sdl_error(const char *msg) {
  printf("%s error: %s\n", msg, SDL_GetError());
}


rgb gradient (double pos) {
  double ratio;
  rgb out;

  if (pos < 0.16) {
    ratio = (pos - 0.0) / (0.16 - 0.0);
    out.r = 0 + ratio * (32 - 0);
    out.g = 7 + ratio * (107 - 7);
    out.b = 100 + ratio * (203 - 100);
  } else if (pos < 0.42) {
    ratio = (pos - 0.16) / (0.42 - 0.16);
    out.r = 32 + ratio * (237 - 32);
    out.g = 107 + ratio * (255 - 107);
    out.b = 203 + ratio * (255 - 203);
  } else if (pos < 0.6425) {
    ratio = (pos - 0.42) / (0.6425 - 0.42);
    out.r = 237 + ratio * (255 - 237);
    out.g = 255 + ratio * (170 - 255);
    out.b = 255 + ratio * (0 - 255);
  } else if (pos < 0.8575) {
    ratio = (pos - 0.6425) / (0.8575 - 0.6425);
    out.r = 255 + ratio * (0 - 255);
    out.g = 170 + ratio * (2 - 170);
    out.b = 0;
  } else {
    ratio = (pos - 0.8575) / (1.0 - 0.8575);
    out.r = 0;
    out.g = 2 + ratio * (7 - 2);
    out.b = 0 + ratio * (100 - 0);
  }
  return out;
}


void render_mandelbrot(SDL_Renderer *ren, rgb *colors, double viewport_size, double center_x, double center_y) {
  int i, j, index;
  double x, y, z, zi, newz, newzi, smooth;
  rgb color;
  for (j=0; j < SCREEN_HEIGHT; j++) {
    for (i=0; i < SCREEN_WIDTH; i++) {
      // transform pixel coordinates to viewport coordinates
      x = center_x - 0.5 * viewport_size + ((double)i / SCREEN_WIDTH) * viewport_size;
      y = center_y - 0.5 * viewport_size + ((double)j / SCREEN_HEIGHT) * viewport_size;

      // compute the mandelbrot formula
      index = 0;
      z = 0.0;
      zi = 0.0;
      while (index < MAX_ITERATIONS) {
        newz = (z * z) - (zi * zi) + x;
        newzi = 2 * z * zi + y;
        z = newz;
        zi = newzi;
        if (((z * z) + (zi * zi)) > 4) {
          break;
        }
        index++;
      }
      if (index == 0 || index >= MAX_ITERATIONS) {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

      } else {
        smooth = index + 1 - log(log(sqrt(z * z + zi * zi))) / log(2);
        color = colors[(int)smooth];
        SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, 255);
      }
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
  int i;
  rgb colors[MAX_ITERATIONS];

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    log_sdl_error("SDL_Init");
    return 1;
  }

  win = SDL_CreateWindow("Mandelbrot set",
                         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         SCREEN_WIDTH, SCREEN_HEIGHT,
                         SDL_WINDOW_SHOWN);
  if (win == NULL) {
    log_sdl_error("SDL_CreateWindow");
    SDL_Quit();
    return 1;
  }

  ren = SDL_CreateRenderer(win, -1, (SDL_RENDERER_ACCELERATED |
                                     SDL_RENDERER_PRESENTVSYNC));

  if (ren == NULL) {
    SDL_DestroyWindow(win);
    log_sdl_error("SDL_CreateRenderer");
    SDL_Quit();
    return 1;
  }

  for (i=0; i < MAX_ITERATIONS; i++) {
    colors[i] = gradient((float)i / MAX_ITERATIONS);
  }

  viewport_size = 4.0;
  x = 0.0;
  y = 0.0;
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
    render_mandelbrot(ren, colors, viewport_size, x, y);
    viewport_size = 0.9 * viewport_size;
  }

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
