#include "game.hpp"

#include "SDL.h"
#include "SDL_opengl.h" 
#include "SDL_image.h"
#include <cmath>
#include <iostream>


CFrameManager::CFrameManager(){
  frames = 0;
  FPS = 0;
  averageFPS = 0;
}

void CFrameManager::begin_frame(){
  begin_time = SDL_GetTicks();  
}

void CFrameManager::end_frame(){
  int delay = 1000/60-(SDL_GetTicks()-begin_time);
  if (delay>0)
    SDL_Delay(delay);
  ++frames;
  FPS = 1000/(SDL_GetTicks()-begin_time);
  averageFPS += (FPS-averageFPS)/frames;
}

GLfloat CFrameManager::get_FPS(){
  return FPS;
}

GLfloat CFrameManager::get_aFPS(){
  return averageFPS;
}

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
  
  SDL_ShowCursor(SDL_DISABLE);

  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  //начало установки 2d-режима
  glViewport( 0, 0, 640, 480 );
 
  glClear( GL_COLOR_BUFFER_BIT );
 
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
 
  glOrtho(0.0f, xres, 0.0f, yres, -1.0f, 1.0f);
	
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
  fps_manager = new CFrameManager;
  ssmanager = new CSpriteSheetManager;
  controller = new CController;
  ssmanager -> load("aya.png");
  hero = new CHero("aya.png", ssmanager);
  ui_background = LoadTexture_simple("images/ui.png");
}

CEngine::~CEngine(){
  delete hero;
  delete ssmanager;
  delete fps_manager;
  delete controller;
#ifdef DEBUG
  std::cerr << "Quitting.";
#endif
  SDL_Quit();
#ifdef DEBUG
  std::cerr << ".done!" << std::endl;
#endif
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
  state.main_state = ENGINE_STATE_GAME;
  state.active = true;
  frames = 0;
}

void CEngine::think(){
  controller_state c_state = controller -> get_state();
  GLfloat speed;
  speed = (c_state.focus)?0.5f:1.0f;
  hero -> set_speed_angle(c_state.strength * speed, c_state.direction);
  hero ->think();
}

void CEngine::handle_events(){
  SDL_Event *event = new SDL_Event;
  while (SDL_PollEvent(event)){
    if (controller -> handle_event(event))
      continue;
    switch (event -> type){  
    case SDL_KEYDOWN:	//обработка нажатий клавиш
      switch (event -> key.keysym.sym){
      case SDLK_ESCAPE:
	state.main_state = ENGINE_STATE_QUIT;
	break;
      default:
#ifdef DEBUG
	std::cerr << "Don't know dis baton!"<< std::endl;
#endif
	break;
      }
      break;
	
    case SDL_QUIT://обработка закрытия окна
      state.main_state = ENGINE_STATE_QUIT;
      break;
	
    case SDL_ACTIVEEVENT://обработка сворачивания/разворачивания окна
      if (event -> active.state == SDL_APPACTIVE)
	if (event -> active.gain)
	  state.active = true;
	else{
	  state.active = false;	
#ifdef DEBUG
	  std::cerr << "minimizing!" << std::endl;
#endif
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

  new_game();
  while (state.main_state!=ENGINE_STATE_QUIT){
    fps_manager -> begin_frame();
    handle_events();
    if (state.main_state == ENGINE_STATE_GAME){
      frames++;
      think();//а что тут думать, тут писать надо!
    }
    draw();
    fps_manager -> end_frame();
    //    std::cerr << (fps_manager -> get_aFPS()) << "fps" <<std::endl;
  }
 
  write_config();
}

void CEngine::draw_game(){
  
  glViewport(0,0,640,480);
  glLoadIdentity();
  glEnable2D();
  //рисуем панельку со статами здесь
  glBindTexture(GL_TEXTURE_RECTANGLE_ARB , ui_background);
  glBegin( GL_QUADS );{
    glTexCoord2i( 0  , 480 ); glVertex2i( 0,    0 );
    glTexCoord2i( 640, 480 ); glVertex2i( xres, 0 );
    glTexCoord2i( 640, 0   ); glVertex2i( xres, yres );
    glTexCoord2i( 0  , 0   ); glVertex2i( 0,    yres );}
  glEnd();

  glDisable2D();
  
  glViewport(32,16,GAME_FIELD_WIDTH,GAME_FIELD_HEIGHT);
  glScissor(32,16,GAME_FIELD_WIDTH,GAME_FIELD_HEIGHT);
  glLoadIdentity();

  glEnable(GL_SCISSOR_TEST);
  glClear(GL_COLOR_BUFFER_BIT);

  //рисуем фон

  glEnable2D();

  //рисуем спрайты
  hero -> draw();

  glDisable2D();
  glDisable(GL_SCISSOR_TEST);
}

void CEngine::draw(){
  glClear(GL_COLOR_BUFFER_BIT);
  switch (state.main_state){
  case ENGINE_STATE_GAME:
    draw_game();
    break;
  case ENGINE_STATE_MAIN_MENU:
    break;
  case ENGINE_STATE_PAUSED:
    draw_game();
    break;
  default:
    break;
  }
  if (state.active)
  SDL_GL_SwapBuffers();
}
