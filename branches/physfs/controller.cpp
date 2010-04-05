#include "controller.hpp"

CController::CController(): state(new std::set<EButton>){
  //Открываем джойстик
#ifdef DEBUG
  std::cerr << "Initializing joystick.";
#endif
  if(SDL_NumJoysticks() > 0){
    stick = SDL_JoystickOpen(0);
#ifdef DEBUG
    if(stick == NULL)
      std::cerr << ".failed to open joystick!" << std::endl;
    else
      std::cerr << ".done!" << std::endl << "Joystick name: " << SDL_JoystickName(0) << std::endl;
#endif
  }else{
#ifdef DEBUG
    std::cerr << ".there is no useable joystick!" << std::endl; 
#endif
    stick = NULL;
  }
  analog_state.dir=0;
  analog_state.pow=0;
  state->insert(B_NIL);//я не понимаю, зачем я это написал, но пусть будет
}

CController::~CController(){
  if(stick != NULL)
    SDL_JoystickClose(stick);
  delete state;
}

int CController::save_old(){
  old_focus = ((state -> count(B_FOCUS))!=0);
  old_attack = ((state -> count(B_ATTACK))!=0);
  return 0;
}

int CController::handle_event(SDL_Event* event){
  switch (event -> type){
  case SDL_KEYDOWN: //нажата кнопка на клаве
    return button(event -> key.keysym.sym, STATE_DOWN);
  case SDL_KEYUP: //отпущена кнопка на клаве
    return button(event -> key.keysym.sym, STATE_UP);
  case SDL_JOYBUTTONDOWN: //нажата кнопка на джойстике
    return jbutton(event -> jbutton.button, STATE_DOWN);
  case SDL_JOYBUTTONUP: //отпущена кнопка на джойстике
    return jbutton(event -> jbutton.button, STATE_UP);
  case SDL_JOYAXISMOTION: //управление джойстиком
    return axismove();
  default:
    return 0;
  }
}

//обработчик нажатия кнопок клавы
int CController::button(SDLKey key, bool key_state){
  EButton control;
  //Ахтунг! Увага! БЫДЛОКОД!
  switch (key){
  case SDLK_DOWN:
    control = B_DOWN;
    break;
  case SDLK_UP:
    control = B_UP;
    break;
  case SDLK_LEFT:
    control = B_LEFT;
    break;
  case SDLK_RIGHT:
    control = B_RIGHT;
    break;
  case SDLK_z:
    control = B_ATTACK;
    break;
  case SDLK_x:
    control = B_BOMB;
    break;
  case SDLK_LSHIFT:
    control = B_FOCUS;
    break;
  case SDLK_LCTRL:
    control = B_SKIP;
    break;
  default:
    return 0;
  }
  //быдлокод енд
  button(control, key_state);
  return 1;
}

//обработчик нажатия кнопок джойстика
int CController::jbutton(Uint8 key, bool key_state){
  EButton control;
  switch (key){
  case JOY_BUTTON_A:
    control = B_ATTACK;
    break;
  case JOY_BUTTON_B:
    control = B_BOMB;
    break;
  case JOY_BUTTON_C:
    control = B_FOCUS;
    break;
  case JOY_BUTTON_D:
    control = B_SKIP;
    break;
  default:
    return 0;
  }
  button(control, key_state);
  return 1;
}

//обработчик дёргания осей джойстика
int CController::axismove(){
  Sint16 vx, vy;
  //Вместо того, чтобы полагаться на значения полученные во время события,
  //будем каждый раз получать их самостоятельно для обоих осей.
  //Так не понадобится запоминать их значения между вызовами.
  vx = SDL_JoystickGetAxis(stick, JOY_AXIS_LR);
  vy = SDL_JoystickGetAxis(stick, JOY_AXIS_UD);
  //Обнуляем все значения попавшие в мёртвую зону
  if ((vx >= -JOY_DEAD_ZONE) && (vx <= JOY_DEAD_ZONE))
    vx = 0;
  if ((vy >= -JOY_DEAD_ZONE) && (vy <= JOY_DEAD_ZONE))
    vy = 0;
  
  analog_state.dir = atan2((float)-vy,(float)vx);
  
  analog_state.pow = sqrt(((float)vx*vx + (float)vy*vy))/32768/(fabs(sin(analog_state.dir))+fabs(cos(analog_state.dir)));

  return 1;
}

void CController::button(EButton key, bool key_state){
  if (key_state)
    state -> insert(key);
  else
    state -> erase(key);
  if (key>=B_UP && key<=B_RIGHT)
    analog_sync();
}

void CController::analog_sync(){
  GLint vx,vy;
  GLfloat angle;
  vx = (state -> count(B_RIGHT)) - (state -> count(B_LEFT));
  vy = (state -> count(B_UP)) - (state -> count(B_DOWN));
  //#ifdef DEBUG
  //std::cerr << "vx:"<<vx << " vy:" << vy<<std::endl;
  //#endif
  if (vx==0 && vy==0){
    analog_state.pow = 0;
    angle = 0;}
  else{
    analog_state.pow = 1;
    angle = atan2((float)vy,(float)vx);}
    if (angle < 0)
      angle += 2*(float)M_PI;
  analog_state.dir = angle;
  //#ifdef DEBUG
  //std::cerr << "angle:" << angle << std::endl;
  //#endif

}

struct controller_state CController::get_state(){
  controller_state result;
  result.direction = analog_state.dir;
  result.strength = analog_state.pow;
  if (old_focus){
    if ((state -> count(B_FOCUS))!=0)
      result.focus = LONG_DOWN;
    else
      result.focus = JUST_UP;
  }
  else{
    if ((state -> count(B_FOCUS))!=0)
      result.focus = JUST_DOWN;
    else
      result.focus = LONG_UP;
  }

  if (old_attack){
    if ((state -> count(B_ATTACK))!=0)
      result.attack = LONG_DOWN;
    else
      result.attack = JUST_UP;
  }
  else{
    if ((state -> count(B_ATTACK))!=0)
      result.attack = JUST_DOWN;
    else
      result.attack = LONG_UP;
  }
  result.skip = (state -> count(B_SKIP))!=0;
  return result;
}

EButton CController::get_confirm(){
  if (state -> count(B_ATTACK)){
    state -> erase(B_ATTACK);
    state -> erase(B_BOMB);
    return B_ATTACK;}
  else if (state -> count(B_BOMB)){
    state -> erase(B_BOMB);
    return B_BOMB;}
  return B_NIL;
}

GLfloat CController::get_kx(){
  return cos(analog_state.dir)*analog_state.pow;
}

GLfloat CController::get_ky(){
  return sin(analog_state.dir)*analog_state.pow;
}
