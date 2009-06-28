#include "game.hpp"

#include "SDL.h"
#include "SDL_opengl.h" 
#include "SDL_image.h"
#include <cmath>
#include <iostream>


CFrameManager::CFrameManager(CLabel* label): FPS(0),averageFPS(0),frames(0),fps_label(label){}

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
  fps_label -> change_text(FPS);
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
  //инициализация видео, джойстика и таймера в SDL
  if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_JOYSTICK) != 0) {
    std::cerr << "Init error: " << SDL_GetError() << "!" << std::endl;
  }
  screen = SDL_SetVideoMode(xres, yres, colour, SDL_OPENGL | (SDL_FULLSCREEN * fullscreen));
  
  //убираем курсор с экрана
  SDL_ShowCursor(SDL_DISABLE);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  //начало установки 2d-режима
  glViewport(0, 0, 640, 480);
 
  glClear(GL_COLOR_BUFFER_BIT);
 
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
 
  glOrtho(0.0f, xres, 0.0f, yres, -1.0f, 1.0f);
	
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //^это для работы (полу-)прозрачности

  glScissor(32, 16, GAME_FIELD_WIDTH, GAME_FIELD_HEIGHT);
  glDisable(GL_SCISSOR_TEST);
  //^это чтобы спрайты игрока, фона и прочего не вылезали

  //  glEnable(GL_TEXTURE_RECTANGLE_ARB);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_ARB_texture_non_power_of_two);
  
  SDL_WM_SetCaption("ru.touhou.project ru_touhou@conference.jabber.ru","ru.danmaku");
#ifdef DEBUG
  std::cerr << ".done!" << std::endl;
#endif

  ssmanager = new CSpriteSheetManager;
  controller = new CController;
  smanager = new CSpriteManager(ssmanager);
  text = new CText(ssmanager);
  ssmanager -> load("fontg.png");
  text -> font_load (std::string("fontg.png"));
  text -> text_add(9, 18, std::string("fps:"), 0);
  fps_manager = new CFrameManager(text -> text_add(45, 18, std::string("0"), 0));
  ssmanager -> load("aya_2.png");
  hero = new CHero("aya_2.png", ssmanager);
  ui_background = LoadTexture_simple("th_ru/ui.png");
  background = new CBack;
  ssmanager -> load("bullets.png");
}

CEngine::~CEngine(){
  delete hero;
  delete ssmanager;
  delete fps_manager;
  delete controller;
  delete background;
  delete smanager;
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
  background -> init("th_ru/grnd03.jpg");
  hero -> set_position(GAME_FIELD_WIDTH/2, 100);
}

void CEngine::think(){
  controller_state c_state = controller -> get_state();
  GLfloat speed;
  speed = (c_state.focus)?0.5f:1.0f;
  if (c_state.focus)
    hero->sprite->set_alpha_speed(-.005f);
  else{
    hero->sprite->set_alpha_speed(0.005f);
  }
  if (c_state.attack){
    int i = smanager -> create_sprite("bullets.png", (GLuint)0);
    CSprite* bull_sprite = smanager -> get_sprite(i);
    bull_sprite -> set_position(hero -> x, hero -> y);
    //    bull_sprite -> set_decay(100);
    bull_sprite -> set_speed(0.f,20.f);
  }
  hero -> set_speed_angle(c_state.strength*speed, c_state.direction);
  hero ->think();
  hero ->sprite->think();
  text ->think();
  background -> think();
  smanager -> think();
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
        case SDLK_RETURN:
          if (SDL_WM_ToggleFullScreen(screen)==0)
            std::cerr << "Failure!" << std::endl;
          break;
        case SDLK_PRINT:
          state.screenshot=true;
          break;
        default:
#ifdef DEBUG
          std::cerr << "Don't know dis baton!"<< std::endl;
#endif
          break;
      }
      break;
      
    case SDL_QUIT: //обработка закрытия окна
      state.main_state = ENGINE_STATE_QUIT;
      break;
      
    case SDL_ACTIVEEVENT: //обработка сворачивания/разворачивания окна
      if (event -> active.state == SDL_APPACTIVE)
	    if (event -> active.gain)
           state.active = true;
	    else{
           state.active = false;	
#ifdef DEBUG
	       std::cerr << "minimizing!" << std::endl;
#endif
		}
      break;
      
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
  while (state.main_state != ENGINE_STATE_QUIT){
    fps_manager -> begin_frame();
    handle_events();
    if (state.main_state == ENGINE_STATE_GAME){
      frames++;
      think(); //а что тут думать, тут писать надо!
    }
    draw();
    fps_manager -> end_frame();
    //std::cerr << (fps_manager -> get_aFPS()) << "fps" <<std::endl;
  }
 
  write_config();
}

void CEngine::draw_game(){
  
  glViewport(0, 0, 640, 480);
  glLoadIdentity();
  glEnable2D();
  //рисуем панельку со статами здесь
  glBindTexture(GL_TEXTURE_2D, ui_background);
  glBegin( GL_QUADS );{
    //    glTexCoord2i( 0  , 480 ); glVertex2i( 0,    0 );
    //    glTexCoord2i( 640, 480 ); glVertex2i( xres, 0 );
    //    glTexCoord2i( 640, 0   ); glVertex2i( xres, yres );
    //    glTexCoord2i( 0  , 0   ); glVertex2i( 0,    yres );}
    glTexCoord2i(0, 1); glVertex2i(0, 0);
    glTexCoord2i(1, 1); glVertex2i(xres, 0);
    glTexCoord2i(1, 0); glVertex2i(xres, yres);
    glTexCoord2i(0, 0); glVertex2i(0, yres);}
  glEnd();

  glDisable2D();

  glViewport(32, 16, GAME_FIELD_WIDTH, GAME_FIELD_HEIGHT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_SCISSOR_TEST);
  glClear(GL_COLOR_BUFFER_BIT);

  background -> draw();

  glEnable2D();

  //рисуем спрайты

  hero -> draw();

  smanager -> draw();
  
  text -> draw();

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
  if (state.active){
    SDL_GL_SwapBuffers();
    if (state.screenshot){
      state.screenshot = false;
      save_screenshot();
    }
  }
}

void CEngine::save_screenshot(){
  //сохранение скриншота идёт сюда.
}