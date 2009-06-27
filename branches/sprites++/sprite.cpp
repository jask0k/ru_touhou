#include "sprite.hpp"


CSpriteSheet::CSpriteSheet(char* filename){
  std::string full_filename = "th_ru/";
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

void CSpriteSheet::draw(GLuint animation, GLuint state, GLfloat x, GLfloat y, GLfloat rotation, GLfloat scale){
  draw((*animations)[animation][state*2],x,y,rotation,scale);
}

void CSpriteSheet::draw(GLint frame, GLfloat x, GLfloat y, GLfloat rotation, GLfloat scale){
#ifdef DEBUG
  if (frame >= rectangle.x * rectangle.y ||
      -1-frame >= rectangle.x * rectangle.y ){
    std::cerr << "incorrect frame!" << std::endl;
    SDL_Quit();
  }
#endif
  GLint invertk=1;
  if (frame < 0){
    invertk = -1;
    frame = -frame-1;
  }
  //вычисляем координаты кадра
  SDL_Rect frame_dimensions = rectangle;
  frame_dimensions.x = rectangle.w * (frame % rectangle.x);
  frame_dimensions.y = rectangle.h * (frame / rectangle.x);
  GLfloat kx = (GLfloat)(sdl_texture->w);
  GLfloat ky = (GLfloat)(sdl_texture->h);

  //сохраняем матрицу преобразования
  glPushMatrix();
  //устанавливаем координаты центра спрайта
  glTranslatef(x,y,0.0f);
  //вращаем спрайт
  glRotatef(rotation,0.0f,0.0f,1.0f);
  glScalef(scale,scale,1.0f);
  //биндим текстуру
  glBindTexture(GL_TEXTURE_2D ,texture_handle);
  glBegin( GL_QUADS );{//фигурные скобки добавлены чтоб были отступы
    glTexCoord2f( (GLfloat)frame_dimensions.x/kx, (frame_dimensions.y+frame_dimensions.h)/ky );
    glVertex2f( -rectangle.w/2*invertk,-rectangle.h/2 );
    glTexCoord2f( (frame_dimensions.x+frame_dimensions.w)/kx, (frame_dimensions.y+frame_dimensions.h)/ky );	
    glVertex2f( rectangle.w/2*invertk, -rectangle.h/2 );
    glTexCoord2f( (frame_dimensions.x+frame_dimensions.w)/kx, frame_dimensions.y/ky );	
    glVertex2f( rectangle.w/2*invertk, rectangle.h/2 );
    glTexCoord2f( frame_dimensions.x/kx, frame_dimensions.y/ky );		
    glVertex2f( -rectangle.w/2*invertk, rectangle.h/2 );}
  glEnd();
  glPopMatrix();
}
void CSpriteSheet::draw_int(GLuint frame, GLint x, GLint y){
  if (frame >= rectangle.x * rectangle.y){
    std::cerr << "incorrect frame!" << std::endl;
    SDL_Quit();
  }
  GLfloat kx = (GLfloat)(sdl_texture->w);
  GLfloat ky = (GLfloat)(sdl_texture->h);
  //вычисляем координаты кадра
  SDL_Rect frame_dimensions = rectangle;
  frame_dimensions.x = rectangle.w * (frame % rectangle.x);
  frame_dimensions.y = rectangle.h * (frame / rectangle.x);
  //биндим текстуру
  glBindTexture(GL_TEXTURE_2D, texture_handle);
  glBegin( GL_QUADS );{//фигурные скобки добавлены, чтоб были отступы
    glTexCoord2f( frame_dimensions.x/kx, (frame_dimensions.y+frame_dimensions.h)/ky );
    glVertex2i( x,y );
    glTexCoord2f( (frame_dimensions.x+frame_dimensions.w)/kx, (frame_dimensions.y+frame_dimensions.h)/ky );	
    glVertex2i( x+rectangle.w, y );
    glTexCoord2f( (frame_dimensions.x+frame_dimensions.w)/kx, frame_dimensions.y/ky );	
    glVertex2i( x+rectangle.w, y+rectangle.h );
    glTexCoord2f( frame_dimensions.x/kx, frame_dimensions.y/ky );		
    glVertex2i( x, y+rectangle.h );
  }
  glEnd();
}

GLuint CSpriteSheet::get_frames(GLuint animation){
  if (animation < animations -> size())
    return (GLuint)((*animations)[animation].size()/2);
  else
    return 0;
}

GLuint CSpriteSheet::get_pause(GLuint animation, GLuint state){
  if (animation < animations -> size())
    if (state < ((*animations)[animation].size()/2))
      return (*animations)[animation][state*2+1];
    else
      return 0;
  else
    return 0;
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

CSprite::CSprite(CSpriteSheet* ssheet, GLint frame_no):
  rotation(0),ssheet(ssheet),frame(frame_no),
  alpha(1.f),tint_r(1.f),tint_g(1.f),tint_b(1.f),
  animation_active(false),decay_active(false),
  v_x(0.f),v_y(0.f),v_r(0.f),v_alpha(0.f){}

CSprite::CSprite(CSpriteSheet* ssheet, GLuint anim_no):
  rotation(0),ssheet(ssheet),animation(anim_no),
  alpha(1.f),tint_r(1.f),tint_g(1.f),tint_b(1.f),
  state(0),animation_active(true),decay_active(false),
  v_x(0.f),v_y(0.f),v_r(0.f),v_alpha(0.f),
  animation_timer(ssheet->get_pause(anim_no,0)){}

void CSprite::draw(){
  glPushAttrib (GL_CURRENT_BIT);
  glColor4f(tint_r, tint_g, tint_b, alpha);
  if (animation_active)
    ssheet -> draw (animation, state, x, y, rotation, scale);
  else
    ssheet -> draw (frame, x, y, rotation, scale);
  glPopAttrib ();
}

void CSprite::set_position(GLfloat new_x, GLfloat new_y, GLfloat new_rotation){
  x = new_x;
  y = new_y;
  rotation = new_rotation;
}

void CSprite::set_scale(GLfloat scale){
  this -> scale = scale;
}

void CSprite::set_tint(GLfloat red, GLfloat green, GLfloat blue){
  this->tint_r = red;
  this->tint_g = green;
  this->tint_b = blue;
}

void CSprite::clear_tint(){
  this->tint_r = 1.f;
  this->tint_g = 1.f;
  this->tint_b = 1.f;
}

decay_state CSprite::think(){
  rotation += v_r;
  alpha += v_alpha;
  if (alpha > 1.f)
    alpha = 1.f;
  else if (alpha < 0.f)
    alpha = 0.f;
  x += v_x;
  if (x < -GAME_FIELD_WIDTH || x > 2 * GAME_FIELD_WIDTH)
    return DECOMPOSED;
  y += v_y;
  if (y < -GAME_FIELD_HEIGHT || y > 2 * GAME_FIELD_HEIGHT)
    return DECOMPOSED;

  if (animation_active)
    if (--animation_timer == 0){
      if (++state == ssheet -> get_frames(animation))
	state = 0;
      animation_timer = ssheet -> get_pause(animation,state);
    }
  if (decay_active)
    if (--decay_timer == 0)
      return DECOMPOSED;
    else
      return STILL_ALIVE;
  else
    return STILL_ALIVE;
}

void CSprite::set_alpha(GLfloat amount){
  this -> alpha = amount;
}

void CSprite::set_alpha_speed(GLfloat amount){
  this -> v_alpha = amount;
}

void CSprite::set_speed(GLfloat v_x, GLfloat v_y, GLfloat v_r){
  this -> v_x = v_x;
  this -> v_y = v_y;
  this -> v_r = v_r;
}

void CSprite::set_angle(GLfloat v, GLfloat angle){
  this -> v_r = 0.f;
  this -> v_x = cos(M_PI*angle/180)*v;
  this -> v_y = sin(M_PI*angle/180)*v;
  this -> rotation = angle;
}

CSpriteManager::CSpriteManager(CSpriteSheetManager* ssmanager):
  ssmanager(ssmanager),free_handle(0){}

GLuint CSpriteManager::create_sprite(std::string spritesheet, GLint frame_no){
  CSprite* sprite = new CSprite(ssmanager->dispatch(spritesheet), frame_no);
  GLuint result = free_handle;
  collection.insert(std::pair<GLuint, CSprite*>(result,sprite));
  while(collection.count(free_handle))
    ++free_handle;
  return result;
}

GLuint CSpriteManager::create_sprite(std::string spritesheet, GLuint animation){
  CSprite* sprite = new CSprite(ssmanager->dispatch(spritesheet), animation);
  GLuint result = free_handle;
  collection.insert(std::pair<GLuint, CSprite*>(result,sprite));
  while(collection.count(free_handle))
    ++free_handle;
  return result;
}

CSprite* CSpriteManager::get_sprite(GLuint handle){
  return collection[handle];
}

void  CSpriteManager::draw(){
  std::map<GLuint,CSprite*>::iterator i;
  for (i = collection.begin(); i != collection.end(); ++i)
    i -> second -> draw();
}

void CSpriteManager::think(){
  std::map<GLuint,CSprite*>::iterator i;
  for (i = collection.begin();i != collection.end();)
    if (i -> second -> think() == DECOMPOSED){
      GLuint bad_handle = i -> first;
      ++i;
      destroy_sprite(bad_handle);
    }
    else
      ++i;
}

GLuint CSpriteManager::destroy_sprite(GLuint handle){
  if (collection.count(handle) == 0)
    return 0;
  collection.erase(handle);
  if (free_handle>handle)
    free_handle=handle;
  return free_handle;
}
