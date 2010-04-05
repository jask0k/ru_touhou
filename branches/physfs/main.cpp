#include "main.hpp"

#include <cstring>

namespace game{
  CSpriteSheetManager* ssmanager = new CSpriteSheetManager;
  CSpriteManager* smanager = new CSpriteManager;
  CLabelManager* lmanager = new CLabelManager;
  CScript* script = new CScript;
  CEngine* engine = new CEngine;
  CBack* background = new CBack;
  CHero* hero;
  CEnemyBulletManager* ebmanager = new CEnemyBulletManager;
  CParticleManager* pmanager = new CParticleManager;
  CBoomBox *boom_box = new CBoomBox;
}

int main(int argc, char* argv[]){
  int i;
  for (i = 1;i<argc;i++)
    if (strncmp("-nosound",argv[i],8) == 0){
#ifdef DEBUG
      std::cerr << "Disabling sound!\n";
#endif
      game::boom_box -> sound_disable();
    }
  game::engine->loop(); 
  delete game::boom_box;
  delete game::pmanager;
  delete game::ebmanager;
  delete game::background;
  delete game::engine;
  delete game::script;
  delete game::smanager;
  delete game::ssmanager;
  return 0;
}
