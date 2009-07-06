#include "script.hpp"

namespace game{
  CScript* script = new CScript;
}

namespace bind{
//бинды
  int wait(lua_State* L);
  int sprite_create (lua_State* L);
  int sprite_set_position(lua_State* L);
  int sprite_set_speed(lua_State* L);
  int sprite_set_angle(lua_State* L);
}

CScript::CScript():level_state(lua_open()){
  luaL_openlibs(level_state);
  do_binds();
}

CScript::~CScript(){
  lua_close(level_state);
}

int CScript::load_script(std::string scriptname){
  //пока это так, но попозже здесь будет чтение из дата
  return luaL_loadfile(level_state,(std::string("th_ru/")+scriptname+std::string(".luc")).c_str());
}

int CScript::do_binds(){
  lua_register(level_state, "wait", bind::wait);
  lua_register(level_state, "sprite_create", bind::sprite_create);
  lua_register(level_state, "sprite_set_position", bind::sprite_set_position);
  lua_register(level_state, "sprite_set_angle", bind::sprite_set_angle);
  return 0;
}

int CScript::think(){
  if (state.resume){
    lua_resume(level_state, 0);
    state.resume=false;
  }
  if (timer_active){
    if (--timer == 0){
      timer_active = false;
      if (lua_resume(level_state, 0) == 0)
	return 1;
    }
  }
  return 0;
}

int CScript::init_level(int level){
  std::ostringstream s;
  s << "level" << level;
  load_script(s.str());
  state.resume = true;
  return 0;
}

int CScript::set_timer(GLuint timer){
  this -> timer = timer;
  this -> timer_active = true;
  return 0;
}

int bind::wait(lua_State* L){
  int narg = lua_gettop(L);
  if (narg !=1){
    lua_pushstring(L, "wait: wrong number of arguments");
    lua_error(L);
  }
  else{
    int timer = lua_tointeger(L,1);
    game::script->set_timer(timer);
    return lua_yield(L, 0);
  }
  return 0;
}

int bind::sprite_create(lua_State* L){
  int narg = lua_gettop(L);
  std::string ssname("aya.png");
  int frame = 0;
  if (narg > 0){
    ssname = lua_tolstring(L,1,NULL);
    if (narg > 1)
      frame = lua_tointeger(L,2);
  }
  
  GLuint sprite_handle = game::smanager->create_sprite(ssname, frame);
  lua_pushinteger(L,sprite_handle);
  return 1;
}

int bind::sprite_set_position(lua_State* L){
  int narg = lua_gettop(L);
  GLfloat x=0.f;
  GLfloat y=0.f;
  GLuint sprite = 0;
  if (narg>0)
    sprite = lua_tointeger(L,1);
  if (narg>2){
    x = (GLfloat)lua_tonumber(L,2);
    y = (GLfloat)lua_tonumber(L,3);
  }
  CSprite* sp = game::smanager -> get_sprite(sprite);
  sp -> set_position(x,y);
  return 0;
}

int bind::sprite_set_angle(lua_State* L){
  int narg = lua_gettop(L);
  GLfloat r=0.f;
  GLfloat a=0.f;
  GLuint sprite = 0;
  if (narg>0)
    sprite = lua_tointeger(L,1);
  if (narg>2){
    r = (GLfloat)lua_tonumber(L,2);
    a = (GLfloat)lua_tonumber(L,3);
  }
  CSprite* sp = game::smanager -> get_sprite(sprite);
  sp -> set_angle(r,a);
  return 0;
}
