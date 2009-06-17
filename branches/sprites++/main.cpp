#include "main.hpp"

int main(int argc, char* argv[]){
  CEngine *engine = new CEngine();
  engine->loop(); 
  delete engine;
  return 0;
}
