PROJNAME = danmaku

CC = g++

OBJFILES := controller.o copypasta.o game.o hero.o main.o sprite.o text.o back.o script.o

CFLAGS = -Wall -Wextra -DDEBUG `pkg-config --cflags lua sdl gl glu` -ggdb3

COMPILE = $(CC) $(CFLAGS) -c

LINK = $(CC) $(CFLAGS) -lSDL_image `pkg-config --libs lua sdl gl glu` -o 

LINK-STATIC = $(CC) $(OBJFILES) -Wl,-Bstatic -Wl,-L/usr/lib `pkg-config --libs --static lua sdl` -lSDL_image -Wl,-Bdynamic `pkg-config --libs glu gl` -o 




all: $(PROJNAME) th_ru/level1.luc



$(PROJNAME): $(OBJFILES)

	 $(LINK) $@ $(OBJFILES)

$(PROJNAME)-static: $(OBJFILES)

	$(LINK-STATIC) $@

th_ru/level1.luc: th_ru/level1.lua
	luac -o th_ru/level1.luc th_ru/level1.lua

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
