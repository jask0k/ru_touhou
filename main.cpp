#include "main.hpp"

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
