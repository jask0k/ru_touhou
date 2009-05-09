#include "game.hpp"

#include "SDL.h"
#include "SDL_opengl.h" 
#include "SDL_image.h"
#include "math.h"
#include <iostream>

CEngine::CEngine(){
  read_config();
#ifdef DEBUG
  std::cerr << "Initializing video.";
#endif
  //инициализация видео SDL
  if ( SDL_Init(SDL_INIT_VIDEO||SDL_INIT_TIMER) != 0 ) {
    std::cerr << "Init error: " << SDL_GetError() << "!" << std::endl;
  }
  screen = SDL_SetVideoMode( xres, yres, colour, SDL_OPENGL | (SDL_FULLSCREEN * fullscreen) );
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  //начало установки 2d-режима
  glEnable( GL_ARB_texture_rectangle );
  glViewport( 0, 0, 640, 480 );
 
  glClear( GL_COLOR_BUFFER_BIT );
 
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
 
  //  glOrtho(0.0f, 640, 0.0f, 480, -1.0f, 1.0f);
	
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  glEnable(GL_TEXTURE_RECTANGLE_ARB);
  //конец установки 2d-режима
  SDL_WM_SetCaption("ru.touhou.project ru_touhou@conference.jabber.ru","ru.danmaku");
#ifdef DEBUG
  std::cerr << ".done!" << std::endl;
#endif
  ssmanager = new CSpriteSheetManager;
  ssmanager -> load("aya.png");
  hero = new CHero("aya.png", ssmanager);
}

CEngine::~CEngine(){
#ifdef DEBUG
  std::cerr << "Quitting.";
#endif
  SDL_Quit();
#ifdef DEBUG
  std::cerr << ".done!" << std::endl;
#endif
}

void CEngine::think(){
}

int CEngine::read_config(){
#ifdef DEBUG
  std::cerr << "Reading config.";
#endif
  xres = DEFAULT_X;
  yres = DEFAULT_Y;
  colour = DEFAULT_COLOUR;
  fullscreen = WINDOW_DEFAULT;
#ifdef DEBUG
  std::cerr << ".done!" << std::endl;
#endif
  return 0;
}

int CEngine::write_config(){
#ifdef DEBUG
  std::cerr << "Writing config.";
#endif
  //Стаб!
#ifdef DEBUG
  std::cerr << ".done!" << std::endl;
#endif
  return 0;
}

void CEngine::new_game(){
  state = ENGINE_STATE_GAME;
  frames = 0;
}

void CEngine::handle_events(){
  SDL_Event *event = new SDL_Event;
  while (SDL_PollEvent(event)){
    switch (event -> type){  
    case SDL_KEYDOWN:	//обработка нажатий клавиш
      switch (event -> key.keysym.sym){
      case SDLK_ESCAPE:
	state = ENGINE_STATE_QUIT;
	break;
      default:
#ifdef DEBUG
	std::cerr << "Don't know dis baton!"<< std::endl;
#endif
	break;
      }
      break;
	
    case SDL_QUIT://обработка закрытия окна
      state = ENGINE_STATE_QUIT;
      break;
	
    case SDL_ACTIVEEVENT://обработка сворачивания/разворачивания окна
      if (event -> active.gain)
	state = last_state;
      else{
	last_state = state;
	state = ENGINE_STATE_MINIMIZED;
	
	std::cerr << "minimizing!" << std::endl;
      }
    default:
      break;
    }
  }
  
}

void CEngine::loop(){
#ifdef DEBUG
  std::cerr << "Starting game loop!" << std::endl;
#endif
  int frame_begin;
  new_game();
  while (state!=ENGINE_STATE_QUIT){
    frame_begin=SDL_GetTicks();
    handle_events();
    if (state == ENGINE_STATE_GAME){
      frames++;
      think();//а что тут думать, тут писать надо!
    }
    draw();
    SDL_Delay(1000/60-(SDL_GetTicks()-frame_begin));
// #ifdef DEBUG
//     std::cerr << 1000/(SDL_GetTicks()-frame_begin)<< "FPS" << std::endl;
// #endif
  }
 
  write_config();
}

void CEngine::draw_game(){
  glEnable2D();
  hero -> draw();
  glDisable2D();
}

void CEngine::draw(){
  glClear(GL_COLOR_BUFFER_BIT);
  switch (state){
  case ENGINE_STATE_GAME:
    draw_game();
    break;
  case ENGINE_STATE_MENU:
    break;
  case ENGINE_STATE_PAUSED:
    draw_game();
    break;
  default:
    return;
  }
  SDL_GL_SwapBuffers();
}
