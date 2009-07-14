#include "main.hpp"

namespace game{
  CSpriteSheetManager* ssmanager=new CSpriteSheetManager;
  CSpriteManager* smanager=new CSpriteManager;
  CScript* script=new CScript;
  CEngine* engine=new CEngine;
}

int main(int argc, char* argv[]){
  game::engine->loop(); 
  delete game::engine;
  delete game::script;
  delete game::smanager;
  delete game::ssmanager;
  return 0;
}
