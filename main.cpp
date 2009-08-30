#include "main.hpp"

namespace game{
  CSpriteSheetManager* ssmanager = new CSpriteSheetManager;
  CSpriteManager* smanager = new CSpriteManager;
  CScript* script = new CScript;
  CEngine* engine = new CEngine;
  CBack* background = new CBack;
  CHero* hero;
  CEnemyBulletManager* ebmanager = new CEnemyBulletManager(std::string("bullets.png"));
  CParticleManager* pmanager = new CParticleManager;
}

int main(int argc, char* argv[]){
  game::engine->loop(); 
  delete game::pmanager;
  delete game::ebmanager;
  delete game::background;
  delete game::engine;
  delete game::script;
  delete game::smanager;
  delete game::ssmanager;
  return 0;
}
