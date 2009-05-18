PROJNAME = danmaku

CC = g++

CFLAGS = -Wall -Wextra -DDEBUG `sdl-config --cflags` -ggdb3

COMPILE = $(CC) $(CFLAGS) -c

OBJFILES := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

HEADERS := $(wildcard *.hpp)



all: $(PROJNAME)



$(PROJNAME): $(OBJFILES)

	$(CC) $(CFLAGS) -lSDL_image -lGL -lGLU -ggdb3 -o $(PROJNAME) $(OBJFILES)

%.o: %.cpp %.hpp config.hpp

	$(COMPILE) -o $@ $<

clean:

	rm -f *.o $(PROJNAME) $(PROJNAME)-static *~
