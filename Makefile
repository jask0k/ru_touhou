PROJNAME = danmaku

CC = g++

CFLAGS = -Wall -O2 -DDEBUG -march=athlon-xp `sdl-config --cflags` 

COMPILE = $(CC) $(CFLAGS) -c

OBJFILES := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

HEADERS := $(wildcard *.hpp)



all: $(PROJNAME)



$(PROJNAME): $(OBJFILES)

	$(CC) $(CFLAGS) -lSDL_image -g -o $(PROJNAME) $(OBJFILES)

$(PROJNAME)-static: $(OBJFILES) 

	$(CC) -static `sdl-config --static-libs` $(CFLAGS)  -o $(PROJNAME)-static $(OBJFILES)

%.o: %.cpp %.hpp

	$(COMPILE) -o $@ $<

clean:

	rm -f *.o $(PROJNAME) $(PROJNAME)-static *~