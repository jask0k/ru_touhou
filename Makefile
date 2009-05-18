PROJNAME = danmaku

CC = g++

CFLAGS = -Wall -DDEBUG `sdl-config --cflags` -g2

COMPILE = $(CC) $(CFLAGS) -c

OBJFILES := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

HEADERS := $(wildcard *.hpp)



all: $(PROJNAME)



$(PROJNAME): $(OBJFILES)

	$(CC) $(CFLAGS) -lSDL_image -lGL -lGLU -g2 -o $(PROJNAME) $(OBJFILES)

$(PROJNAME)-static: $(OBJFILES) 

	$(CC) -static `sdl-config --static-libs` $(CFLAGS)  -o $(PROJNAME)-static $(OBJFILES)

%.o: %.cpp %.hpp config.hpp

	$(COMPILE) -o $@ $<

clean:

	rm -f *.o $(PROJNAME) $(PROJNAME)-static *~