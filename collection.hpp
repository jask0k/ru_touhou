#ifndef _COLLECTION_HPP
#define _COLLECTION_HPP
#include <GL/gl.h>
#include <map>
#include <string>
#include <set>
//class CScript;
//namespace game{
//  extern CScript* script;
//}
//#include "script.hpp"

//класс коллекции, нужен для присваивания интовых хендлов всяким объектам
//для удобства работы со скриптами
//вобще-то надо бы было сделать метатаблицы и всё такое...
template <class T> class CCollection{
private:
  std::map<GLuint, T*> collection;
  // std::map<GLuint,std::string> destruction_hooks;
  std::set<GLuint> destroyed_collection;
  std::set<GLuint> destroy_tracking;
  GLuint free_handle;
public:
  CCollection();
  GLuint add(T*);
  GLuint operator+(T* what);
  //  T* get(GLuint what);
  T* operator[](GLuint what);
  GLuint count(GLuint what);
  GLuint destroy(GLuint what);
  GLboolean destroy_check(GLuint what);
  //  GLuint add_hook(GLuint handle, std::string funct);
  void wipe();
  typename std::map<GLuint, T*>::iterator begin();
  typename std::map<GLuint, T*>::iterator end();
  ~CCollection();
};

template <class T> CCollection<T>::CCollection():free_handle(1){}

template <class T> GLuint CCollection<T>::add(T* what){
  GLuint result = free_handle;
  collection.insert(std::pair<GLuint, T*>(free_handle,what));
  while(collection.count(free_handle)||destroyed_collection.count(free_handle))
    ++free_handle;
  return result;
}

template <class T> GLuint CCollection<T>::operator+(T* what){
  return add(what);
}


template <class T> T* CCollection<T>::operator[](GLuint what){
  if (collection.count(what)>0)
    return collection[what];
  else
    return NULL;
}

template <class T> GLuint CCollection<T>::count(GLuint what){
    return collection.count(what);
}

template <class T> GLuint CCollection<T>::destroy(GLuint what){
  // if (destruction_hooks.count(what) == 1){
  //   game::script -> run_function(destruction_hooks[what],what);
  //   destruction_hooks.erase(what);
  // }
  if (collection.count(what) == 0)
    return 0;
  if (destroy_tracking.count(what) == 1)
    destroyed_collection.insert(what);
  delete collection[what];
  collection.erase(what);
  if (free_handle>what)
    free_handle=what;
  return free_handle;
}

template <class T> 
GLboolean CCollection<T>::destroy_check(GLuint what){
  if (destroyed_collection.count(what)>0){
    destroyed_collection.erase(what);
    return true;
  }
  if (destroy_tracking.count(what) == 0)
    destroy_tracking.insert(what);
  return false;

}

template <class T> 
typename std::map<GLuint, T*>::iterator CCollection<T>::begin(){
  return collection.begin();
}

template <class T> 
typename std::map<GLuint, T*>::iterator CCollection<T>::end(){
  return collection.end();
}

// template <class T> GLuint CCollection<T>::add_hook(GLuint handle, std::string funct){
//   if (collection.count(handle) == 0)
//     return 0;
//   destruction_hooks.insert(std::pair<GLuint,std::string>(handle,funct));
//   return handle;
// }

template <class T> void CCollection<T>::wipe(){
  free_handle = 1;
  typename std::map<GLuint,T*>::iterator i;
  for (i=collection.begin();i!=collection.end();++i){
    delete i->second;
  }
  //  destruction_hooks.clear();
  collection.clear();
  destroy_tracking.clear();
  destroyed_collection.clear();
}

template <class T> CCollection<T>::~CCollection(){
  wipe();
}
#endif
