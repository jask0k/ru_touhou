#include "controller.hpp"

CController::CController(){
  state = new std::set<EButton>;
  analog_state.dir=0;
  analog_state.pow=0;
  state->insert(B_NIL);
}

CController::~CController(){
  delete state;
}

int CController::handle_event(SDL_Event* event){
  switch (event -> type){
  case SDL_KEYDOWN:
    return button(event -> key.keysym.sym, STATE_DOWN);
  case SDL_KEYUP:
    return button(event -> key.keysym.sym, STATE_UP);
  default:
    return 0;
  }
}

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
#ifdef DEBUG
  std::cerr << "vx:"<<vx << " vy:" << vy<<std::endl;
#endif
  if (vx==0 && vy==0){
    analog_state.pow = 0;
    angle = 0;}
  else{
    analog_state.pow = 1;
    angle = atan2((float)vy,(float)vx);}
    if (angle < 0)
      angle = 2*M_PI+angle;
  analog_state.dir = angle;
#ifdef DEBUG
  std::cerr << "angle:" << angle << std::endl;
#endif

}

struct controller_state CController::get_state(){
  controller_state result;
  result.direction = analog_state.dir;
  result.strength = analog_state.pow;
  result.focus = state -> count(B_FOCUS);
  result.attack = state -> count(B_ATTACK);
  result.skip = state -> count(B_SKIP);
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
