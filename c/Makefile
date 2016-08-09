CXX = gcc

SDL_LIB = `sdl2-config --libs`
SDL_INCLUDE = `sdl2-config --cflags`
CXXFLAGS = -Wall -c $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB) -lm
EXE = mandelbrot

all: $(EXE)

$(EXE): mandelbrot.o
	$(CXX) $< $(LDFLAGS) -o $@

mandelbrot.o: mandelbrot.c
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)
