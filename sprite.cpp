#include "sprite.hpp"


CSpriteSheet::CSpriteSheet(char* filename){
  std::string full_filename = "images/";
  full_filename += filename;
  if (LoadTexture(full_filename.c_str(), texture_handle,  sdl_texture)){
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
  //vint::iterator current_frame;
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
  bool comment = false;
  while (!props_file.eof()){
    symbol = props_file.get();
    if (!comment){
      if (symbol == '#') {
	//начало строчного комментария
	comment = true;
      }
      else if (symbol == '-' && element.empty())
	//встретили минус, предположительно в начале числа
	element.push_back(symbol);
      else if (symbol >= '0' && symbol <= '9') //встретили цифру
	element.push_back(symbol);
      else if (symbol == ';')//встретили ограничитель анимации,
	//смотрим, отправлен ли кадр в последовательность
	if (element.size()){
	  if (current_animation == result->end()){
	    result->push_back(*(new vint()));
	    current_animation = (result->end()) - 1;
	  }
	  (*current_animation).push_back(std::atoi(element.c_str()));
	  current_animation++;
	  element.clear();
	}else{
	  if (current_animation < result->end())
	    current_animation++;
	}
      else if (element.size() == 1 && element[0] == '-')
	//встретили непонятно что, а в элементе нет цифр, а только один минус
	element.clear();
      else if (element.size()>0){
	//встретили непонятно что, в элементе есть цифры -> надо записать номер кадра
	if (current_animation == result->end()){
	  result->push_back(*(new vint()));
	  current_animation=(result->end())-1;
	}
	(*current_animation).push_back(std::atoi(element.c_str()));
	element.clear();
      }
    } else if (symbol == '\n') {
      comment = false;
    }
  }
#ifdef DEBUG
  std::cerr << ".done!" << std::endl;
#endif
  return result;
}

void CSpriteSheet::draw(GLuint animation, GLuint state, GLfloat x, GLfloat y, GLfloat rotation){
  draw((*animations)[animation][state*2],x,y,rotation);
}

void CSpriteSheet::draw(GLuint frame, GLfloat x, GLfloat y, GLfloat rotation){
  if (frame >= rectangle.x * rectangle.y){
    std::cerr << "incorrect frame!" << std::endl;
    SDL_Quit();
  }
  //вычисляем координаты кадра
  SDL_Rect frame_dimensions = rectangle;
  frame_dimensions.x = rectangle.w * (frame % rectangle.x);
  frame_dimensions.y = rectangle.h * (frame / rectangle.x);
  //сохраняем матрицу преобразования
  glPushMatrix();
  //устанавливаем координаты центра спрайта
  glTranslatef(x,y,0.0f);
  //вращаем спрайт
  glRotatef(rotation,0.0f,0.0f,1.0f);
  //биндим текстуру
  glBindTexture(GL_TEXTURE_2D ,texture_handle);
  glBegin( GL_QUADS );{//фигурные скобки добавлены чтоб были отступы
    glTexCoord2i( frame_dimensions.x, frame_dimensions.y+frame_dimensions.h );
    glVertex2f( -rectangle.w/2,-rectangle.h/2 );
    glTexCoord2i( frame_dimensions.x+frame_dimensions.w, frame_dimensions.y+frame_dimensions.h );	
    glVertex2f( rectangle.w/2, -rectangle.h/2 );
    glTexCoord2i( frame_dimensions.x+frame_dimensions.w, frame_dimensions.y );	
    glVertex2f( rectangle.w/2, rectangle.h/2 );
    glTexCoord2i( frame_dimensions.x, frame_dimensions.y );		
    glVertex2f( -rectangle.w/2, rectangle.h/2 );}
  glEnd();
  glPopMatrix();
}
void CSpriteSheet::draw_int(GLuint frame, GLint x, GLint y){
  if (frame >= rectangle.x * rectangle.y){
    std::cerr << "incorrect frame!" << std::endl;
    SDL_Quit();
  }
  //вычисляем координаты кадра
  SDL_Rect frame_dimensions = rectangle;
  frame_dimensions.x = rectangle.w * (frame % rectangle.x);
  frame_dimensions.y = rectangle.h * (frame / rectangle.x);
  //биндим текстуру
  glBindTexture(GL_TEXTURE_2D, texture_handle);
  glBegin( GL_QUADS );{//фигурные скобки добавлены, чтоб были отступы
    glTexCoord2i( frame_dimensions.x, frame_dimensions.y+frame_dimensions.h );
    glVertex2i( x,y );
    glTexCoord2i( frame_dimensions.x+frame_dimensions.w, frame_dimensions.y+frame_dimensions.h );	
    glVertex2i( x+rectangle.w, y );
    glTexCoord2i( frame_dimensions.x+frame_dimensions.w, frame_dimensions.y );	
    glVertex2i( x+rectangle.w, y+rectangle.h );
    glTexCoord2i( frame_dimensions.x, frame_dimensions.y );		
    glVertex2i( x, y+rectangle.h );
  }
  glEnd();
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

void CSprite::set_position(GLfloat new_x, GLfloat new_y, GLfloat new_rotation){
  x = new_x;
  y = new_y;
  rotation = new_rotation;
}
