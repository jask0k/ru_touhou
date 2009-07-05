#include "script.hpp"

CScript::CScript():level_state(lua_open()){
  luaL_openlibs(level_state);
  do_binds();
}

CScript::~CScript(){
  lua_close(level_state);
}

int CScript::load_script(std::string scriptname){
  //пока это так, но попозже здесь будет чтение из дата
  return luaL_loadfile(level_state,(std::string("th_ru/")+scriptname).c_str());
}

int CScript::do_binds(){
  //  lua_register(level_state, "wait", wait);
  return 0;
}

int CScript::think(){
  if (state.resume){
    lua_resume(level_state, 0);
    state.resume=false;
  }
  if (timer_active){
    if (--timer == 0){
      lua_resume(level_state, 0);
    }
  }
  return 0;
}

int CScript::init_level(int level){
  std::ostringstream s;
  s << "level" << level << ".lua";
  load_script(s.str());
  state.resume = true;
  return 0;
}

int CScript::wait(lua_State* L){
  int narg = lua_gettop(L);
  if (narg !=1){
    lua_pushstring(L, "wait: wrong number of arguments");
    lua_error(L);
  }
  else{
    timer_active=true;
    timer = lua_tointeger(L,1);
    return lua_yield(L, 0);
  }
  return 0;
}

int CScript::sprite_create(lua_State* L){
  int narg = lua_gettop(L);
  std::string ssname("aya.png");
  if (narg > 0){
    //    ssname = lua_tolstring(L,1);
  }
  GLuint sprite_handle = game::smanager->create_sprite(ssname, 0);
  lua_pushinteger(L,sprite_handle);
  return 1;
}
