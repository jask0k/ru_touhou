#include "sprite.hpp"


CSpriteSheet::CSpriteSheet(char* filename){
  std::string full_filename = "images/";
  full_filename += filename;
  if (LoadTexture(full_filename.c_str(), &texture_handle, sdl_texture)){
#ifdef DEBUG
    std::cerr << "Loaded spritesheet '" << filename << "'!" << std::endl;
#endif
    
    full_filename+=".spr";
    animations = parse_props((char*)full_filename.c_str());
  }
  else{
    std::cerr << "could not load spritesheet " << filename << std::endl;
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
    bool width_submitted = false;
    if (symbol >= '0' && symbol <= '9')
      element.push_back(symbol);
    else if (symbol == 'x' && !element.empty() && !width_submitted){
      width_submitted = true;
      parse_numbers.str(element);
      parse_numbers >> rectangle.w;
      element.clear();
    }
    else if (symbol == ';' && !element.empty() && width_submitted){
      parse_numbers.str(element);
      parse_numbers >> rectangle.h;
    }
    else {
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
  draw(frame,x,y,0);
}
void CSpriteSheet::draw(GLuint frame, GLfloat x, GLfloat y, GLfloat rotation){
  //биндим текстуру
  glBindTexture(GL_TEXTURE_RECTANGLE_ARB ,texture_handle);
  glPushMatrix();
  //вращаем спрайт
  glTranslatef(x,y,0.0f);
  glRotatef(rotation,0.0f,0.0f,1.0f);
  //вычисляем координаты кадра
  SDL_Rect frame_dimensions = rectangle;
  frame_dimensions.x = rectangle.w * frame % rectangle.x;
  frame_dimensions.y = rectangle.h * frame / rectangle.x;
  glBegin( GL_QUADS );
    glTexCoord2i( frame_dimensions.x, frame_dimensions.y+frame_dimensions.h );
    glVertex2f( x-rectangle.w,y-rectangle.h );
    glTexCoord2i( frame_dimensions.x+frame_dimensions.w, frame_dimensions.y+frame_dimensions.h );	
    glVertex2f( x+rectangle.w, y-rectangle.h );
    glTexCoord2i( frame_dimensions.x+frame_dimensions.w, frame_dimensions.y );	
    glVertex2f( x+rectangle.w, y+rectangle.h );
    glTexCoord2i( frame_dimensions.x, frame_dimensions.y );		
    glVertex2f( x-rectangle.w, y+rectangle.h );
  glEnd();
}
