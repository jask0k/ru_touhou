#include "enemy.hpp"

CEnemy::CEnemy(GLuint proto, GLuint bullet_proto, GLuint sprite, GLfloat max_hp):
  sprite_no(sprite),proto_no(proto),bullet_proto(bullet_proto),hp(max_hp){}

GLboolean CEnemy::think(){
  return (hp > 0.f);
}

GLboolean CEnemy::damage(GLfloat amount){
  hp -= amount;
  return (hp > 0.f);
}

//CEnemyManager::CEnemyManager():free_handle(0){}
CEnemyManager::CEnemyManager(){}

GLuint CEnemyManager::create_enemy_proto(std::string sheetname, GLuint animation, GLfloat max_hp, GLuint bullet_proto, std::string die_func){
  SEnemyProto new_proto = {sheetname, animation, max_hp, bullet_proto, die_func};
  proto_collection.push_back(new_proto);
  return proto_collection.size()-1;
}

GLuint CEnemyManager::create_enemy(GLuint proto, GLfloat x, GLfloat y){
  GLuint sprite_no = game::smanager -> create_sprite(proto_collection[proto].sheetname, LAYER_ENEMY);
  (game::smanager -> get_sprite(sprite_no)) -> set_position(x,y);
  CEnemy* new_enemy = new CEnemy(proto, proto_collection[proto].bullet_proto, 
				 sprite_no, proto_collection[proto].max_hp);
  //  GLuint result = free_handle;
  //  collection.insert(std::pair<GLuint, CEnemy*>(result,new_enemy));
  // while(collection.count(free_handle))
  //   ++free_handle;
  // return result;
  return collection + new_enemy;
}

GLuint CEnemyManager::destroy_enemy(GLuint handle){
  //  if (collection.count(handle) == 0)
  //    return 0;
  //если это интересно движку, сохраняем хэндл врага в сет уничтоженных
  // if (collection[handle]->managed){
  //   destroyed_collection.insert(handle);
  // }
  GLuint sprite_no = collection[handle] -> sprite_no;
  GLuint proto_no = collection[handle] -> proto_no;
  if (game::smanager -> get_sprite(sprite_no) != NULL){
    //Вызываем скрипт исчезновения врага с параметром номера спрайта
    game::script -> run_function(proto_collection[proto_no].die_func,sprite_no);
  }
  return collection.destroy(handle);
  //  collection.erase(handle);
  // if (free_handle>handle)
  //   free_handle=handle;
  // return free_handle;
}

GLboolean CEnemyManager::enemy_destroyed(GLuint handle){
  return collection.destroy_check(handle);
  // if (destroyed_collection.count(handle)>0){
  //   destroyed_collection.erase(handle);
  //   return true;
  // }
  // if (!collection[handle]->managed)
  //   collection[handle]->managed = true;
  // return false;
}
