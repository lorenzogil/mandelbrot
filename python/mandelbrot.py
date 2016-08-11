# TODO: write setup.py to install PySDL2

from math import floor, log, sqrt

import sdl2
import sdl2.ext


SCREEN_WIDTH = 600
SCREEN_HEIGHT = 600
MAX_ITERATIONS = 16


def gradient(pos):

    if (pos < 0.16):
        ratio = (pos - 0.0) / (0.16 - 0.0)
        red = 0 + ratio * (32 - 0)
        green = 7 + ratio * (107 - 7)
        blue = 100 + ratio * (203 - 100)
    elif (pos < 0.42):
        ratio = (pos - 0.16) / (0.42 - 0.16)
        red = 32 + ratio * (237 - 32)
        green = 107 + ratio * (255 - 107)
        blue = 203 + ratio * (255 - 203)
    elif (pos < 0.6425):
        ratio = (pos - 0.42) / (0.6425 - 0.42)
        red = 237 + ratio * (255 - 237)
        green = 255 + ratio * (170 - 255)
        blue = 255 + ratio * (0 - 255)
    elif (pos < 0.8575):
        ratio = (pos - 0.6425) / (0.8575 - 0.6425)
        red = 255 + ratio * (0 - 255)
        green = 170 + ratio * (2 - 170)
        blue = 0
    else:
        ratio = (pos - 0.8575) / (1.0 - 0.8575)
        red = 0
        green = 2 + ratio * (7 - 2)
        blue = 0 + ratio * (100 - 0)

    return sdl2.ext.Color(red, green, blue, 255)


def render_mandelbrot(ren, colors, viewport_size, center_x, center_y):
    ren.clear(0)
    black = sdl2.ext.Color(0, 0, 0, 255)

    for j in xrange(SCREEN_HEIGHT):
        for i in xrange(SCREEN_WIDTH):
            # transform pixel coordinates to viewport coordinates
            x = (center_x - 0.5 * viewport_size +
                 (float(i) / SCREEN_WIDTH) * viewport_size)
            y = (center_y - 0.5 * viewport_size +
                 (float(j) / SCREEN_HEIGHT) * viewport_size)

            # compute the Mandelbrot formula
            index = 0
            z = 0.0
            zi = 0.0
            while index < MAX_ITERATIONS:
                newz = (z * z) - (zi * zi) + x
                newzi = 2 * z * zi + y
                z = newz
                zi = newzi
                if ((z * z) + (zi * zi)) > 4:
                    break
                index += 1

            if index == 0 or index >= MAX_ITERATIONS:
                ren.draw_point([i, j], black)
            else:
                smooth = index + 1 - log(log(sqrt(z * z + zi * zi))) / log(2)
                color = colors[int(floor(smooth)) - 1]
                ren.draw_point([i, j], color)

    ren.present()


def main():
    try:
        sdl2.ext.init()
    except sdl2.ext.SDLError as e:
        print("SDL init error: %s" % e.msg)

    try:
        win = sdl2.ext.Window(
            "Mandelbrot set",
            size=(SCREEN_WIDTH, SCREEN_HEIGHT)
        )
        win.show()
    except sdl2.ext.SDLError as e:
        print("SDL Window init error: %s" % e.msg)
        sdl2.ext.quit()
        return 1

    try:
        flags = (
            sdl2.SDL_RENDERER_ACCELERATED |
            sdl2.SDL_RENDERER_PRESENTVSYNC
        )
        ren = sdl2.ext.Renderer(win, flags=flags)
    except sdl2.ext.SDLError as e:
        print("SDL Renderer init error: %s" % e.msg)
        sdl2.ext.quit()
        return 1

    colors = [gradient(float(i) / MAX_ITERATIONS)
              for i in xrange(MAX_ITERATIONS)]

    viewport_size = 4.0
    x = 0.0
    y = 0.0
    running = True
    while running:

        for event in sdl2.ext.get_events():
            if event.type == sdl2.SDL_QUIT:
                running = False
                break
            elif event.type == sdl2.SDL_KEYDOWN:
                key = event.key.keysym.sym
                if key == sdl2.SDLK_ESCAPE:
                    running = False
                elif key == sdl2.SDLK_UP:
                    y -= 0.1 * viewport_size
                elif key == sdl2.SDLK_DOWN:
                    y += 0.1 * viewport_size
                elif key == sdl2.SDLK_LEFT:
                    x -= 0.1 * viewport_size
                elif key == sdl2.SDLK_RIGHT:
                    x += 0.1 * viewport_size

                break

        render_mandelbrot(ren, colors, viewport_size, x, y)
        viewport_size = 0.9 * viewport_size

        win.refresh()

    sdl2.ext.quit()


if __name__ == '__main__':
    main()
