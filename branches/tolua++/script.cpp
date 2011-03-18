#include "script.hpp"

#include "danmaku_api.hpp"

#define declare_number(name) lua_pushnumber(level_state, name);lua_setglobal(level_state, #name)
#define declare_function(name) int name(lua_State* L)
#define bind_function(name) lua_register(level_state, #name, bind::name)
#define get_bullet(name) (game::ebmanager -> get_bullet(name))
#define get_label(name) (game::lmanager -> get_label(name))


namespace script{
  //формат вида "ifs", где каждый символ - тип переменной
  int parameters_parse(lua_State* L, std::string format, ...);
}

namespace bind{
//бинды
  declare_function(log);
  declare_function(wait_time);//Пауза на несколько кадров
  declare_function(wait_cond);
  declare_function(wait_time_cond);
  declare_function(thread_start);

}

int CScript::do_binds(){
  bind_function(wait_time);
  bind_function(wait_cond);
  bind_function(wait_time_cond);
  bind_function(log);
  bind_function(thread_start);

  return 0;
}

int CScript::do_globals(){
  declare_number(GAME_FIELD_WIDTH);
  declare_number(GAME_FIELD_HEIGHT);
  // declare_number(LAYER_BACKGROUND);
  // declare_number(LAYER_ENEMY_BULLET);
  // declare_number(LAYER_ENEMY);
  // declare_number(LAYER_HERO_BULLET);
  // declare_number(LAYER_HERO);
  // declare_number(LAYER_EMBLEM);

  // declare_number(LAYER_GAME);
  // declare_number(LAYER_PANEL);


  //  declare_number(CONTROL_BULLET);
  //  declare_number(CONTROL_SPRITE);
  return 0;
}

CScript::CScript():level_state(luaL_newstate()),cond(""){
  luaL_openlibs(level_state);

  //биндим классы
  tolua_danmaku_api_open(level_state);

  do_globals();
  do_binds();
  lua_newtable(level_state);
  lua_setglobal(level_state,"AI_table");
}

CScript::~CScript(){
  lua_close(level_state);
}

int CScript::load_script(std::string scriptname){
  std::string full_path=scriptname+std::string(".luc");
  SDL_RWops *fp;
  long len=0;
  char *buf=NULL;
  fp=PHYSFSRWOPS_openRead(full_path.c_str());
  if (fp!=NULL)
  {
    SDL_RWseek(fp, 0, SEEK_END);
    len=SDL_RWtell(fp); //get position at end (length)
    SDL_RWseek(fp, 0, SEEK_SET);
    buf=(char *)malloc(len); //malloc buffer
    SDL_RWread(fp,buf,len,1); //read into buffer
    SDL_RWclose(fp);
  }
  else
  {
    printf("Script load failed: %s\n", SDL_GetError());
  }
  return luaL_loadbuffer(level_state,buf,len,scriptname.c_str());
}

void CScript::print_error() const
{
	std::string err_message(luaL_checklstring(level_state,-1,NULL));
	luaL_where(level_state,0);
	std::cerr <<"lua error: "<< lua_tolstring(level_state,-1,NULL) << err_message << std::endl;
}

int CScript::run_script(std::string scriptname){
  int ret=load_script(scriptname);
  if(ret) {
 #ifdef DEBUG
    std::string err_str(luaL_checklstring(level_state,1,NULL));
    std::cerr <<"lua error: "<< err_str << std::endl;
#endif
    return -1;
  }

  if (lua_pcall(level_state,0,0,0) == 0){
    return 0;
  } else {
#ifdef DEBUG
    print_error();
#endif
    return 1;
  }
}

int CScript::run_function(std::string funcname){
  lua_getfield(level_state, LUA_GLOBALSINDEX, funcname.c_str());
  if (lua_isfunction(level_state,-1))
    if (lua_pcall(level_state,0,0,0) != 0){
#ifdef DEBUG
      print_error();
#endif
      return 1;
    }
  return 0;
}

int CScript::run_function(std::string funcname, GLuint parameter){
  lua_getfield(level_state, LUA_GLOBALSINDEX, funcname.c_str());
  if (lua_isfunction(level_state,-1)){
    lua_pushinteger(level_state,parameter);
    if (lua_pcall(level_state,1,0,0) != 0){
#ifdef DEBUG
      print_error();
#endif
      return 1;
    }
  }
  return 0;
}
int CScript::run_function(std::string funcname, void* ptr, const char* type){
  lua_getfield(level_state, LUA_GLOBALSINDEX, funcname.c_str());
  if (lua_isfunction(level_state,-1)){
    tolua_pushusertype(level_state, ptr, type);
    if (lua_pcall(level_state,1,0,0) != 0){
#ifdef DEBUG
      print_error();
#endif
      return 1;
    }
  }
  return 0;
}

GLboolean CScript::check_cond(lua_State* L, std::string cond){
  std::string query = std::string("return (") + cond + std::string(")");
  luaL_dostring(L,query.c_str());
  GLboolean result = lua_toboolean(L,-1);
  lua_pop(L,1);
  return result;
}

lua_State* CScript::create_AI_state(lua_State* L){
  if (lua_isfunction(L, 1) == 1 && lua_isstring(L, 2) == 1){
    //создаём тред АИ
    lua_State* state = lua_newthread(L);
    //переносим указатель на тред на дно стека, он нам пока не нужен
    lua_insert(L,1);
    
    lua_pushvalue(L,2);
    lua_remove(L,2);
    //переносим указатель на функцию в стек треда АИ
    lua_xmove(L,state,1);

    //забираем условие выхода из стека
    std::string cond = std::string(luaL_checkstring(L,2));
    //    std::cerr << cond << std::endl;
    //и удаляем его оттудова
    lua_remove(L,2);
    if (cond.compare("")!=0 && check_cond(L,cond))
      return NULL;
    //смотрим, сколько лишних параметров у нас есть
    GLuint par_num = lua_gettop(L)-1;
    //    std::cerr << par_num << std::endl;
    //и суём их все в параметры треда
    lua_xmove(L,state,par_num);
    //создаём структуру, содержащую инфу про тред АИ
    AI_state st = {0,cond,std::string("")};
    //и засовываем её в коллекцию
    AI_states.insert(std::pair<lua_State*,AI_state>(state,st));
    lua_getglobal(L,"AI_table");
    lua_insert(L,1);
    std::ostringstream reader;
    reader << state; 
    //    std::cerr << state_name << std::endl;
    lua_setfield(L,1,reader.str().c_str());
    lua_resume(state,1);
    return state;
  }
  return NULL;
}

int CScript::destroy_AI_state(std::map<lua_State*,AI_state>::iterator position){
  std::ostringstream reader;
  reader << position->first; 
  lua_getglobal(level_state,"AI_table");
  lua_pushnil(level_state);
  lua_setfield(level_state,-2,reader.str().c_str());
  //  std::cerr << "destroing " << reader.str() << std::endl;
  AI_states.erase(position);
  return 0;
}

GLint CScript::get_integer(std::string var_name){
  lua_getglobal(level_state, var_name.c_str());
  return luaL_checkint(level_state,-1);
}
GLint CScript::get_integer(const char* var_name){
  lua_getglobal(level_state, var_name);
  return luaL_checkint(level_state,-1);
}
GLfloat CScript::get_number(std::string var_name){
  lua_getglobal(level_state, var_name.c_str());
  return luaL_checknumber(level_state,-1);
}
GLfloat CScript::get_number(const char* var_name){
  lua_getglobal(level_state, var_name);
  return luaL_checknumber(level_state,-1);
}
std::string CScript::get_string(std::string var_name){
  const char* value;
  lua_getglobal(level_state, var_name.c_str());
  value = luaL_checklstring(level_state,-1,NULL);
  return std::string(value);
}
std::string CScript::get_string(const char* var_name){
  const char* value;
  lua_getglobal(level_state, var_name);
  value = luaL_checklstring(level_state,-1,NULL);
  return std::string(value);
}

void* CScript::get_class(const char* var_name){
  lua_getglobal(level_state, var_name);
  return tolua_tousertype(level_state,-1,NULL);
}

GLint CScript::set_integer(std::string var_name, GLint value){
  lua_pushinteger(level_state,value);
  lua_setglobal(level_state,var_name.c_str());
  return value;
}

GLint CScript::set_integer(const char* var_name, GLint value){
  lua_pushinteger(level_state,value);
  lua_setglobal(level_state,var_name);
  return value;
}

GLfloat CScript::set_number(std::string var_name, GLfloat value){
  lua_pushnumber(level_state,value);
  lua_setglobal(level_state,var_name.c_str());
  return value;
}

GLfloat CScript::set_number(const char* var_name, GLfloat value){
  lua_pushnumber(level_state,value);
  lua_setglobal(level_state,var_name);
  return value;
}

std::string CScript::set_string(std::string var_name, std::string value){
  lua_pushlstring(level_state, value.c_str(), value.size());
  lua_setglobal(level_state, var_name.c_str());
  return value;
}

std::string CScript::set_string(const char* var_name, std::string value){
  lua_pushlstring(level_state, value.c_str(), value.size());
  lua_setglobal(level_state, var_name);
  return value;
}


int CScript::think(){
  std::map<lua_State*,AI_state>::iterator i;
  std::map<lua_State*,AI_state>::iterator bad_handle;
  for (i = AI_states.begin(); i != AI_states.end();){
    //    bool cleanup=false;
    if (i -> second.timer > 0)
      --(i->second.timer);
    if (i->second.quit_condition.compare("")!=0 && 
	check_cond(i->first, i->second.quit_condition)){
      //      cleanup = true;
      //    }
      //    if (cleanup){
      bad_handle = i;
      ++i;
      if (destroy_AI_state(bad_handle) == -1){
#ifdef DEBUG
	std::cerr << "fixme:Could not destroy AI state!" << std::endl;
#endif
      }
      //      cleanup = false;
    }
    else if (i -> second.timer == 0 || 
	     ((i -> second.wait_condition.compare("")!=0) && 
	      (check_cond(i->first,i -> second.wait_condition)))){
      int result = lua_resume(i->first,0);
      if (result!=LUA_YIELD){
	//завершились с ошибкой или скрипт закончил выполнение
	bad_handle = i;
	++i;
	if (destroy_AI_state(bad_handle) == -1){
#ifdef DEBUG
	  std::cerr << "fixme:Could not destroy AI state!" << std::endl;
#endif
	}
#ifdef DEBUG
	if (result!=0){
	  std::cerr << "stack content '"<<i->first<<"'"<<std::endl;
	  int it;
	  for (it=1;it<=lua_gettop(i->first);++it)
	    std::cerr << lua_typename(i->first, lua_type(i->first, it)) << std::endl;
	  std::cerr << "end!"<<std::endl;
	  std::cerr << "error code:" << result<< std::endl;
	  const char* err_string = luaL_checklstring(i->first,-1,NULL);
	  std::cerr << "script error:" << err_string << std::endl;
	}
#endif
	
      } 
      else
	++i;
    } else ++i;
  }
  if (state.resume){
    lua_resume(level_state, 0);
    state.resume=false;
  }
  if ((timer_active && --timer == 0)||
      (cond.compare("")!=0 && check_cond(level_state, cond))){
    timer_active = false;
    if (lua_resume(level_state, 0) == 0)
      return 1;
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

int CScript::set_timer(lua_State* state, GLuint timer){
  if (state == level_state){
    this -> timer = timer;
    this -> timer_active = true;
  }
  else
    AI_states[state].timer = timer; 
  return 0;
}

int CScript::set_cond(lua_State* state, std::string cond){
  if (state == level_state)
    this -> cond = cond;
  else
    AI_states[state].wait_condition = cond; 
  return 0;
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
    luaL_where(L,0);
    std::string err_pos = lua_tolstring(L,-1,NULL);
    std::cerr << err_pos << "arguments error!" << std::endl;
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
      const char** var = va_arg(vl,const char**);
      *var = (luaL_checkstring(L,cur_var));
    }
      break;
    }
  }
  va_end(vl);
  return stack_size;
}

int bind::wait_time(lua_State* L){
  int timer;
  script::parameters_parse(L,"i",&timer);
  game::script->set_timer(L, timer);
  return lua_yield(L, 0);
}

int bind::wait_cond(lua_State* L){
  const char* cond;
  script::parameters_parse(L,"s",&cond);
  game::script->set_cond(L, cond);
  return lua_yield(L, 0);
}

int bind::wait_time_cond(lua_State* L){
  int timer;
  const char* cond;
  script::parameters_parse(L,"is",&timer, &cond);
  game::script->set_cond(L, cond);
  game::script->set_timer(L, timer);
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

//параметры: целое - хендл врага, и функция - функция вида func(handle)
//не обращайте внимания, я просто сосу хуйцы, ололо
int bind::thread_start(lua_State* L){
  lua_State* thread = game::script -> create_AI_state(L);
  if (thread!=NULL){
    lua_pushthread(thread);
    lua_xmove(thread,L,1);
    return 1;
  }
  else{
#ifdef DEBUG
    std::cerr << "Could not create AI state!" << std::endl;
#endif
    return 0;
  }
}

