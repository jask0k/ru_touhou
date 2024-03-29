#include "main.hpp"

namespace game{
  CSpriteSheetManager* ssmanager;
  CSpriteManager* smanager;
  CLabelManager* lmanager;
  CScript* script;
  CEngine* engine;
  CBack* background;
  CHero* hero;
  CHeroBulletManager* hbmanager;
  CEnemyBulletManager* ebmanager;
  CVariableManager* vmanager;
  CEnemyManager* enmanager;
  CParticleManager* pmanager;
  CBoomBox* boom_box;
  CController* controller;
}

int main(int argc, char* argv[]){
  PHYSFS_init(argv[0]);
  game::ssmanager = new CSpriteSheetManager;
  game::smanager = new CSpriteManager;
  game::lmanager = new CLabelManager;
  game::vmanager = new CVariableManager;
  game::script = new CScript;
  game::engine = new CEngine;
  game::background = new CBack;
  game::ebmanager = new CEnemyBulletManager;
  game::enmanager = new CEnemyManager;
  game::hbmanager = new CHeroBulletManager;
  game::pmanager = new CParticleManager;
  game::boom_box = new CBoomBox;
  for (int i = 1;i<argc;i++)
    if (strncmp("-nosound",argv[i],8) == 0){
#ifdef DEBUG
      std::cerr << "Disabling sound!\n";
#endif
      game::boom_box -> sound_disable();
    }
  game::engine->loop(); 
  delete game::boom_box;
  delete game::pmanager;
  delete game::hbmanager;
  delete game::vmanager;
  delete game::enmanager;
  delete game::ebmanager;
  delete game::background;
  delete game::engine;
  delete game::script;
  delete game::smanager;
  delete game::ssmanager;
  PHYSFS_deinit();
  return 0;
}
