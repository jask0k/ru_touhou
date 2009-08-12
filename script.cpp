#include "script.hpp"

#include <cstdarg>

#define declare_number(name) lua_pushnumber(level_state, name);lua_setglobal(level_state, #name)
#define declare_function(name) int name(lua_State* L)
#define bind_function(name) lua_register(level_state, #name, bind::name)


namespace script{
  const char* reader(lua_State* L, void* filename, size_t* size);//функция чтения скриптов из dat
  int parameters_parse(lua_State* L, std::string format, ...);//формат вида "ns", где каждый символ - тип переменной
}

namespace bind{
//бинды
  declare_function(wait);//Пауза на несколько кадров
  declare_function(spritesheet_load);//загрузка спрайтового листа менеджером
  declare_function(sprite_create);//создание спрайтов
  declare_function(sprite_set_position);//установка координат спрайта
  declare_function(sprite_set_speed);
  declare_function(sprite_set_angle);
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
  std::string full_path=std::string("th_ru/")+scriptname+std::string(".luc");
  return lua_load(level_state,script::reader, (void*)full_path.c_str(), scriptname.c_str());
}

int CScript::run_script(std::string scriptname){
  load_script(scriptname);
  lua_call(level_state,0,0);
  return 0;
}

int CScript::run_function(std::string funcname){
  lua_State* L=lua_newthread(level_state);
  lua_getfield(L, LUA_GLOBALSINDEX, funcname.c_str());
  if (!lua_pcall(L,0,0,0)){
    free(L);
    return 0;
  } else {
#ifdef DEBUG
    std::string err_message(luaL_checklstring(L,1,NULL));
    std::cerr << err_message;
#endif
    free(L);
    return 1;
  }
}

int CScript::do_globals(){
  declare_number(GAME_FIELD_WIDTH);
  declare_number(GAME_FIELD_HEIGHT);
  declare_number(LAYER_BACKGROUND);
  declare_number(LAYER_ENEMY_BULLET);
  declare_number(LAYER_ENEMY);
  declare_number(LAYER_HERO_BULLET);
  declare_number(LAYER_HERO);
  declare_number(LAYER_EMBLEM);
  return 0;
}

int CScript::do_binds(){
  bind_function(wait);
  bind_function(spritesheet_load);
  bind_function(sprite_create);
  bind_function(sprite_set_position);
  bind_function(sprite_set_angle);
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

const char* script::reader(lua_State* L, void* filename, size_t* size){
  SDL_RWops* rwops;
  char* buf = (char*)calloc(1024*1024,sizeof(char));
  rwops=SDL_RWFromZZIP((char*)filename,"r");
  *size = SDL_RWread(rwops, buf, sizeof(char), 1024*1024);
  buf = (char*)realloc(buf,*size);
  return buf;
}

int script::parameters_parse(lua_State* L, std::string format, ...){
  enum {
    //    TYPE_VOID = ' ',
    TYPE_INTEGER = 'i',
    TYPE_FLOAT = 'f',
    TYPE_STRING = 's'
  };
  GLuint stack_size = lua_gettop(L);
  if (stack_size != format.size()){
    lua_pushstring(L,"Wrong arguments number!");
    lua_error(L);
  }
  va_list vl;
  va_start(vl,format);
  std::string::iterator it;
  unsigned int cur_var;
  for (cur_var=1,it=format.begin();it!=format.end();++it,++cur_var){
    switch(*it){
    case TYPE_INTEGER:{
      int* var = va_arg(vl,int*);
      *var = luaL_checkint(L,cur_var);
    }
      break;
    case TYPE_FLOAT:{
      float* var = va_arg(vl,float*);
      *var = (float)luaL_checknumber(L,cur_var);
    }
      break;
    case TYPE_STRING:{
      char** var = va_arg(vl,char**);
      *var = const_cast<char*>(luaL_checklstring(L,cur_var,NULL));
    }
      break;
    }
  }
  va_end(vl);
  return stack_size;
}

int bind::wait(lua_State* L){
  int timer;
  script::parameters_parse(L,"i",&timer);
  game::script->set_timer(timer);
  return lua_yield(L, 0);
}

int bind::spritesheet_load(lua_State* L){
  char* ssname;
  script::parameters_parse(L,"s",&ssname);

  game::ssmanager->load(ssname);
  return 0;
};

int bind::sprite_create(lua_State* L){
  char* ssname;
  Layer layer;
  script::parameters_parse(L,"si",&ssname,&layer);
  
  GLuint sprite_handle = game::smanager->create_sprite(std::string(ssname), layer);
  lua_pushinteger(L,sprite_handle);
  return 1;
}

int bind::sprite_set_position(lua_State* L){
  GLfloat x,y;
  GLuint sprite;
  script::parameters_parse(L,"iff",&sprite,&x,&y);

  CSprite* sp = game::smanager -> get_sprite(sprite);
  sp -> set_position(x,y);
  return 0;
}

int bind::sprite_set_angle(lua_State* L){
  GLfloat r,a;
  GLuint sprite;
  script::parameters_parse(L,"iff",&sprite,&r,&a);
  CSprite* sp = game::smanager -> get_sprite(sprite);
  sp -> set_angle(r,a);
  return 0;
}
