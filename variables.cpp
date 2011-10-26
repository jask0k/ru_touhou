#include "variables.hpp"

CVariable::CVariable(std::string name, GLuint value, GLuint default_speed, int font, GLint x, GLint y, text_layer layer){
  std::ostringstream str_value;
  this->value = value;
  str_value << value;
  this->default_speed = default_speed;
  game::vmanager -> add_variable(this, name);
  if (font != -1)
    display = new CLabel(x, y, str_value.str(), font, layer);
  else
    display = NULL;
}
CVariable::~CVariable(){
  game::vmanager -> del_variable(this);
}
GLuint CVariable::inc(int increment){
  this -> value = this -> value + increment;
  this -> update_label();
  return this -> value;
}
GLuint CVariable::dec(int decrement){
  this -> value = this -> value - decrement;
  this -> update_label();
  return this -> value;
}
GLuint CVariable::set(GLuint value){
  this -> value = value;
  this -> update_label();
  return value;
}
GLuint CVariable::get(){
  return value;
}
GLuint CVariable::slowly_inc(int increment, int speed, int frame_pause){
  return 0;
}
GLuint CVariable::slowly_dec(int increment, int speed, int frame_pause){
  return 0;
}
void CVariable::think(){
}
void CVariable::update_label(){
  std::ostringstream str_value;
  if (display !=NULL){
    str_value << value;
    display -> change_text(str_value.str());
  }
}
void CVariableManager::add_variable(CVariable* pointer, std::string& name){
  collection.insert(std::pair<std::string,CVariable*>(name,pointer));
}
void CVariableManager::del_variable(CVariable* pointer){
  
}
void CVariableManager::think(){
}

