CXX = g++

SDL_LIB = `sdl2-config --libs`
SDL_INCLUDE = `sdl2-config --cflags`
CXXFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB)
EXE = mandelbrot

all: $(EXE)

$(EXE): mandelbrot.o
	$(CXX) $< $(LDFLAGS) -o $@

mandelbrot.o: mandelbrot.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)
