#include "main.hpp"

int main(int argc, char* argv[]){
  game::engine->loop(); 
  delete game::engine;
  return 0;
}
