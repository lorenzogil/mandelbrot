# TODO: write setup.py to install PySDL2

import sdl2
import sdl2.ext


SCREEN_WIDTH = 600
SCREEN_HEIGHT = 600
MAX_ITERATIONS = 128


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

    running = True
    while running:

        for event in sdl2.ext.get_events():
            if event.type == sdl2.SDL_QUIT:
                running = False
                break

        win.refresh()

    sdl2.ext.quit()


if __name__ == '__main__':
    main()
