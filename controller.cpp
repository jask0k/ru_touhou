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
	return axismove(event -> jaxis.axis, event -> jaxis.value);
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

//пока что пашет не так, как от него требуется. ПОПРАВИТЬ!!!
//всё ещё хуже, чем я думал. это реализация порочна. ПЕРЕПИСАТЬ!!!
int CController::axismove(Uint8 axis, Sint16 value){
  EButton control;
  //считаем, что при таких значениях ось отпущена иначе её теребят
  bool key_state = !((value > -300) && (value < 300));
  switch (axis){
  case JOY_AXIS_LR:
    if ( value <= 0 )
		control = B_LEFT;
	else if ( value > 0 )
		control = B_RIGHT;
    break;
  case JOY_AXIS_UD:
    if ( value <= 0 )
		control = B_UP;
	else if ( value > 0 )
		control = B_DOWN;
    break;
  default:
    return 0;
  }
#ifdef DEBUG
  std::cerr << "value: "<< value << " axis: " << (int)axis << " control: " << control << " key_state: " << key_state << std::endl;
#endif
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
      angle += 2*M_PI;
  analog_state.dir = angle;
  //#ifdef DEBUG
  //std::cerr << "angle:" << angle << std::endl;
  //#endif

}

struct controller_state CController::get_state(){
  controller_state result;
  result.direction = analog_state.dir;
  result.strength = analog_state.pow;
  result.focus = (bool)state -> count(B_FOCUS);
  result.attack = (bool)state -> count(B_ATTACK);
  result.skip = (bool)state -> count(B_SKIP);
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
