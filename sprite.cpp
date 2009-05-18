#include "sprite.hpp"


CSpriteSheet::CSpriteSheet(char* filename){
  std::string full_filename = "images/";
  full_filename += filename;
  if (LoadTexture(full_filename.c_str(), &texture_handle,  &sdl_texture)){
#ifdef DEBUG
    std::cerr << "Loaded spritesheet '" << filename << "'!" << std::endl;
#endif
    
    full_filename+=".spr";
    animations = parse_props((char*)full_filename.c_str());
  }
  else{
    std::cerr << "could not load spritesheet " << filename << std::endl;
    SDL_Quit();
  }
}

vvint* CSpriteSheet::parse_props(char* filename){
  vvint* result = new vvint;
  vvint::iterator current_animation = result->begin();
  vint::iterator current_frame;
  std::ifstream props_file(filename);
  std::istringstream parse_numbers;
#ifdef DEBUG
  std::cerr << "Parsing " << filename << ".";
#endif
  std::string element;
  char symbol;
  while ((symbol = props_file.get())!='\n'){
    if (symbol >= '0' && symbol <= '9')
      element.push_back(symbol);
    else if (symbol == 'x' && !element.empty()){
      parse_numbers.str(element);
      parse_numbers >> rectangle.w;
      parse_numbers.sync();
      element.clear();
      parse_numbers.clear();
    }
    else if (symbol == ';' && !element.empty()){
      parse_numbers.str(element);
      parse_numbers >> rectangle.h;
      element.clear();
    }
    else {
      std::cerr << "char:" << symbol << " string:" << element << std::endl;
      std::cerr << ".Parsing error!";
    }
  }
  rectangle.x = (sdl_texture -> w) / rectangle.w;
  rectangle.y = (sdl_texture -> h) / rectangle.h;
  while (!props_file.eof()){
    symbol = props_file.get();
    if (symbol == '-' && element.empty())
      //встретили минус, предположительно в начале числа
      element.push_back(symbol);
    else if (symbol >= '0' && symbol <= '9') //встретили цифру
      element.push_back(symbol);
    else if (symbol == ';')//встретили ограничитель анимации,
      //смотрим, отправлен ли кадр в последовательность
      if (element.size()){
      }
      else{
      }
    else if (element.size() == 1 && element[0] == '-')
      //встретили непонятно что, а в элементе нет цифр, а только один минус
      element.clear();
    else if (element.size()>0){
      //встретили непонятно что, в элементе есть цифры -> надо записать номер кадра
      
    }
  }
#ifdef DEBUG
  std::cerr << ".done!" << std::endl;
#endif
  return result;
}

void CSpriteSheet::draw(GLuint frame, GLfloat x, GLfloat y){
  //вычисляем координаты кадра
  SDL_Rect frame_dimensions = rectangle;
  frame_dimensions.x = rectangle.w * frame % rectangle.x;
  frame_dimensions.y = rectangle.h * frame / rectangle.x;
  //биндим текстуру
  glBindTexture(GL_TEXTURE_RECTANGLE_ARB ,texture_handle);
  glBegin( GL_QUADS );{
    glTexCoord2i( frame_dimensions.x, frame_dimensions.y+frame_dimensions.h );
    glVertex2f( x-rectangle.w/2,y-rectangle.h/2 );
    glTexCoord2i( frame_dimensions.x+frame_dimensions.w, frame_dimensions.y+frame_dimensions.h );	
    glVertex2f( x+rectangle.w/2, y-rectangle.h/2 );
    glTexCoord2i( frame_dimensions.x+frame_dimensions.w, frame_dimensions.y );	
    glVertex2f( x+rectangle.w/2, y+rectangle.h/2 );
    glTexCoord2i( frame_dimensions.x, frame_dimensions.y );		
    glVertex2f( x-rectangle.w/2, y+rectangle.h/2 );}
  glEnd();
}
void CSpriteSheet::draw(GLuint frame, GLfloat x, GLfloat y, GLfloat rotation){
  //сохраняем матрицу преобразования
  glPushMatrix();
  //вращаем спрайт
  glTranslatef(x,y,0.0f);
  glRotatef(rotation,0.0f,0.0f,1.0f);
  //рисуем его
  draw(frame,x,y);
  //возвращаем матрицу состояния
  glPopMatrix();
}

CSpriteSheet* CSpriteSheetManager::load(char* filename){
  CSpriteSheet* spritesheet = new CSpriteSheet(filename);
  std::string sheetname(filename);
  collection[sheetname] = spritesheet;
  return spritesheet;
}

CSpriteSheet* CSpriteSheetManager::dispatch(std::string sheetname){
  return collection[sheetname];
}

CSprite::CSprite(std::string sheetname, CSpriteSheetManager* manager, GLuint frame_no){
  ssheet = manager -> dispatch(sheetname);
  frame = frame_no;
  rotation = 0;
}

void CSprite::draw(){
  ssheet -> draw (frame,x,y,rotation);
}

void CSprite::set_position(GLfloat new_x, GLfloat new_y){
  x = new_x;
  y = new_y;
}

void CSprite::set_position(GLfloat new_x, GLfloat new_y, GLfloat new_rotation){
  set_position(new_x,new_y);
  rotation = new_rotation;
}
