	PROJNAME = danmaku

CC = g++

CFLAGS = -Wall -Wextra -DDEBUG `sdl-config --cflags` -ggdb3

COMPILE = $(CC) $(CFLAGS) -c

LINK = $(CC) $(CFLAGS) `sdl-config --libs` -lSDL_image -lGL -lGLU -llua -ldl -o 

#LINK-STATIC = $(CC) $(CFLAGS) -static `sdl-config --static-libs` -lSDL_image -lGL -lGLU -o 


OBJFILES := controller.o copypasta.o game.o hero.o main.o sprite.o text.o back.o script.o

HEADERS := $(wildcard *.hpp)



all: $(PROJNAME)



$(PROJNAME): $(OBJFILES)

	 $(LINK) $@ $(OBJFILES)

#$(PROJNAME)-static: $(OBJFILES)
#
#	$(LINK-STATIC) $@ $(OBJFILES)

controller.o: controller.cpp controller.hpp config.hpp

	$(COMPILE) -o $@ controller.cpp

copypasta.o: copypasta.cpp copypasta.hpp config.hpp

	$(COMPILE) -o $@ copypasta.cpp

game.o: game.cpp game.hpp sprite.o back.o copypasta.o controller.o hero.o text.o script.o config.hpp

	$(COMPILE) -o $@ game.cpp

hero.o: hero.cpp hero.hpp sprite.o config.hpp

	$(COMPILE) -o $@ hero.cpp

main.o: game.o main.cpp main.hpp

	$(COMPILE) -o $@ main.cpp

script.o: copypasta.o script.cpp script.hpp config.hpp

	$(COMPILE) -o $@ script.cpp

sprite.o: copypasta.o sprite.cpp sprite.hpp config.hpp

	$(COMPILE) -o $@ sprite.cpp

text.o: text.cpp text.hpp sprite.o

	$(COMPILE) -o $@ text.cpp

back.o: back.cpp back.hpp copypasta.o config.hpp

	$(COMPILE) -o $@ back.cpp


clean:

	rm -f *.o $(PROJNAME) $(PROJNAME)-static *~
