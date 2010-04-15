#ifndef _VARIABLES_HPP
#define _VARIABLES_HPP

#include "text.hpp" 

class CVariable{
public:
  CVariable();
  int inc();
private:
  CLabel display;
  const char* format;
  int value;
  int maxvalue;
  int minvalue;
};
#endif
