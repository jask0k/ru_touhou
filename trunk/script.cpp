#include "script.hpp"

#include <cstdarg>

#define declare_number(name) lua_pushnumber(level_state, name);lua_setglobal(level_state, #name)
#define declare_function(name) int name(lua_State* L)
#define bind_function(name) lua_register(level_state, #name, bind::name)
#define get_sprite(name) (game::smanager -> get_sprite(name))


namespace script{
  const char* reader(lua_State* L, void* filename, size_t* size);//функция чтения скриптов из dat
  int parameters_parse(lua_State* L, std::string format, ...);//формат вида "ns", где каждый символ - тип переменной
}

namespace bind{
//бинды
  declare_function(log);
  declare_function(wait);//Пауза на несколько кадров

  declare_function(engine_get_frame);

  declare_function(hero_x);
  declare_function(hero_y);

  declare_function(spritesheet_load);//загрузка спрайтового листа менеджером

  declare_function(sprite_create);//создание спрайтов
  declare_function(sprite_set_position);//установка координат спрайта
  declare_function(sprite_set_speed);
  declare_function(sprite_set_angle);
  declare_function(sprite_set_tint);
  declare_function(sprite_set_alpha);
  declare_function(sprite_set_alpha_speed);
  declare_function(sprite_set_scale);
  declare_function(sprite_set_scale_speed);
  declare_function(sprite_set_decay);
  declare_function(sprite_set_blur);
  declare_function(sprite_set_frame);
  declare_function(sprite_start_animation);
}

int CScript::do_binds(){
  bind_function(wait);
  bind_function(log);

  bind_function(engine_get_frame);

  bind_function(spritesheet_load);

  bind_function(hero_x);
  bind_function(hero_y);

  bind_function(sprite_create);
  bind_function(sprite_set_position);
  bind_function(sprite_set_speed);
  bind_function(sprite_set_angle);
  bind_function(sprite_set_tint);
  bind_function(sprite_set_alpha);
  bind_function(sprite_set_alpha_speed);
  bind_function(sprite_set_scale);
  bind_function(sprite_set_scale_speed);
  bind_function(sprite_set_decay);
  bind_function(sprite_set_blur);
  bind_function(sprite_set_frame);
  bind_function(sprite_start_animation);
  return 0;
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
  if (!lua_pcall(level_state,0,0,0)){
    return 0;
  } else {
#ifdef DEBUG
    std::string err_message(luaL_checklstring(level_state,1,NULL));
    std::cerr << err_message;
#endif
    return 1;
  }
}

int CScript::run_function(std::string funcname){
  lua_getfield(level_state, LUA_GLOBALSINDEX, funcname.c_str());
  if (lua_isfunction(level_state,-1))
    if (lua_pcall(level_state,0,0,0)){
#ifdef DEBUG
      std::string err_message(luaL_checklstring(level_state,1,NULL));
      std::cerr << err_message;
#endif
      return 1;
    }
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
#ifdef DEBUG
    std::cerr << "FFFFFFFFFUUUUU-" << std::endl;
#endif
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

int bind::log(lua_State* L){
  char* message;
  script::parameters_parse(L,"s",&message);
#ifdef DEBUG
  std::cerr << "Script says:" << message << std::endl;
#endif
  return 0;
}

int bind::engine_get_frame(lua_State* L){
  lua_Integer frame = game::engine->get_frame();
  lua_pushinteger(L,frame);
  return 1;
}
int bind::hero_x(lua_State* L){
  lua_Number x = game::hero->x;
  lua_pushnumber(L,x);
  return 1;
}

int bind::hero_y(lua_State* L){
  lua_Integer y = game::hero->y;
  lua_pushnumber(L,y);
  return 1;
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
  GLfloat x,y,rot;
  GLuint sprite;
  script::parameters_parse(L,"ifff",&sprite,&x,&y,&rot);

  get_sprite(sprite) -> set_position(x,y,rot);
  return 0;
}

int bind::sprite_set_angle(lua_State* L){
  GLfloat r,a;
  GLuint sprite;
  script::parameters_parse(L,"iff",&sprite,&r,&a);
  get_sprite(sprite) -> set_angle(r,a);
  return 0;
}

int bind::sprite_set_speed(lua_State* L){
  GLfloat vx,vy,rot;
  GLuint sprite;
  script::parameters_parse(L,"ifff",&sprite,&vx,&vy,&rot);
  get_sprite(sprite) -> set_speed(vx,vy,rot);
  return 0;
}
int bind::sprite_set_tint(lua_State* L){
  GLfloat red,green,blue;
  GLuint sprite;
  script::parameters_parse(L,"ifff",&sprite,&red,&green,&blue);
  get_sprite(sprite) -> set_tint(red,green,blue);
  return 0;
}
int bind::sprite_set_alpha(lua_State* L){
  GLfloat amount;
  GLuint sprite;
  script::parameters_parse(L,"if",&sprite,&amount);
  get_sprite(sprite) -> set_alpha(amount);
  return 0;
}
int bind::sprite_set_alpha_speed(lua_State* L){
  GLfloat amount;
  GLuint sprite;
  script::parameters_parse(L,"if",&sprite,&amount);
  get_sprite(sprite) -> set_alpha_speed(amount);
  return 0;
}
int bind::sprite_set_scale(lua_State* L){
  GLfloat scale;
  GLuint sprite;
  script::parameters_parse(L,"if",&sprite,&scale);
  get_sprite(sprite) -> set_scale(scale);
  return 0;
}
int bind::sprite_set_scale_speed(lua_State* L){
  GLfloat v_scale;
  GLuint sprite;
  script::parameters_parse(L,"if",&v_scale);
  get_sprite(sprite) -> set_scale_speed(v_scale);
  return 0;
}
int bind::sprite_set_decay(lua_State* L){
  GLint time;
  GLuint sprite;
  script::parameters_parse(L,"ii",&sprite,&time);
  get_sprite(sprite) -> set_decay(time);
  return 0;
}
int bind::sprite_set_blur(lua_State* L){
  GLint blur;
  GLuint sprite;
  script::parameters_parse(L,"ii",&sprite,&blur);
  get_sprite(sprite) -> set_blur( blur!=0 );
  return 0;
}
int bind::sprite_set_frame(lua_State* L){
  GLint frame;
  GLuint sprite;
  script::parameters_parse(L,"ii",&sprite,&frame);
  get_sprite(sprite) -> set_frame(frame);
  return 0;
}
int bind::sprite_start_animation(lua_State* L){
  GLint animation,next_animation;
  GLuint sprite;
  script::parameters_parse(L,"iii",&sprite,&animation,&next_animation);
  get_sprite(sprite) -> start_animation(animation,next_animation);
  return 0;
}
