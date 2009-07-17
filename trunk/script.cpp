#include "script.hpp"

namespace bind{
  const char* reader(lua_State* L, void* filename, size_t* size);
//бинды
  int wait(lua_State* L);
  int spritesheet_load(lua_State* L);
  int sprite_create (lua_State* L);
  int sprite_set_position(lua_State* L);
  int sprite_set_speed(lua_State* L);
  int sprite_set_angle(lua_State* L);
}

CScript::CScript():level_state(lua_open()){
  luaL_openlibs(level_state);
  do_globals();
  do_binds();
}

CScript::~CScript(){
  lua_close(level_state);
}

int CScript::load_script(std::string scriptname){
  ////пока это так, но попозже здесь будет чтение из дата
  //  return luaL_loadfile(level_state,(std::string("th_ru/")+scriptname+std::string(".luc")).c_str());
  return lua_load(level_state,bind::reader, 
		  (void*)(std::string("th_ru/")+scriptname+std::string(".luc")).c_str(), 
		  scriptname.c_str());
}

int CScript::run_script(std::string scriptname){
  load_script(scriptname);
  lua_call(level_state,0,0);
  return 0;
}

int CScript::do_globals(){
  lua_pushnumber(level_state, GAME_FIELD_WIDTH);
  lua_setglobal(level_state, "GAME_FIELD_WIDTH");
  lua_pushnumber(level_state, GAME_FIELD_HEIGHT);
  lua_setglobal(level_state, "GAME_FIELD_HEIGHT");
  lua_pushnumber(level_state, LAYER_BACKGROUND);
  lua_setglobal(level_state, "LAYER_BACKGROUND");
  lua_pushnumber(level_state, LAYER_ENEMY_BULLET);
  lua_setglobal(level_state, "LAYER_ENEMY_BULLET");
  lua_pushnumber(level_state, LAYER_ENEMY);
  lua_setglobal(level_state, "LAYER_ENEMY");
  lua_pushnumber(level_state, LAYER_HERO_BULLET);
  lua_setglobal(level_state, "LAYER_HERO_BULLET");
  lua_pushnumber(level_state, LAYER_HERO);
  lua_setglobal(level_state, "LAYER_HERO");
  lua_pushnumber(level_state, LAYER_EMBLEM);
  lua_setglobal(level_state, "LAYER_EMBLEM");
  return 0;
}

int CScript::do_binds(){
  lua_register(level_state, "wait", bind::wait);
  lua_register(level_state, "spritesheet_load", bind::spritesheet_load);
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

const char* bind::reader(lua_State* L, void* filename, size_t* size){
  SDL_RWops* rwops;
  char* buf = (char*)calloc(1024*1024,sizeof(char));
  rwops=SDL_RWFromZZIP((char*)filename,"r");
  *size = SDL_RWread(rwops, buf, sizeof(char), 1024*1024);
  buf = (char*)realloc(buf,*size);
  return buf;
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

int bind::spritesheet_load(lua_State* L){
  int narg = lua_gettop(L);
  std::string ssname;
  if (narg > 0){
    ssname.append(lua_tolstring(L,1,NULL));
  }
  
  game::ssmanager->load((char*)ssname.c_str());
  return 0;
};

int bind::sprite_create(lua_State* L){
  int narg = lua_gettop(L);
  std::string ssname("aya.png");
  Layer layer = LAYER_EMBLEM;
  if (narg > 0){
    ssname = lua_tolstring(L,1,NULL);
    if (narg > 1)
      layer = (Layer)lua_tointeger(L,2);
  }
  
  GLuint sprite_handle = game::smanager->create_sprite(ssname, layer);
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
