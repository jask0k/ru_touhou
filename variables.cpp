#include "variables.hpp"
CVariable::CVariable(std::string name, GLuint value, GLint default_speed){
}
CVariable::~CVariable(){
}
GLuint CVariable::inc(int increment){
}
GLuint CVariable::dec(int decrement){
}
GLuint CVariable::set(GLuint value){
}
GLuint CVariable::get(){
}
GLuint CVariable::slowly_inc(int increment, int speed, int frame_pause){
}
GLuint CVariable::slowly_dec(int increment, int speed, int frame_pause){
}
void CVariable::think(){
}
void CVariableManager::add_variable(CVariable*){
}
void CVariableManager::think(){
}

