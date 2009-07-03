#include "script.hpp"

CScript::CScript(CEngine* engine):engine(engine),level_state(lua_open()){
  luaL_openlibs(level_state);
  do_binds();
}

CScript::~CScript(){
  lua_close(level_state);
}

int CScript::load_script(char* scriptname){
  //пока это так, но попозже здесь будет чтение из дата
  luaL_loadfile(level_state,scriptname);
}

int CScript::do_binds(){
}
