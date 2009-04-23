#include "main.hpp"
#include "game.hpp"

int main(int argc, char argv[]){
  CEngine *engine = new CEngine();
  engine->loop(); 
  delete engine;
}
