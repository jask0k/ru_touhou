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
  SDL_RWops* props_file=SDL_RWFromZZIP(filename, "r");
  std::istringstream parse_numbers;
#ifdef DEBUG
  std::cerr << "Parsing " << filename << ".";
#endif
  std::string element;
  char symbol;
  while (SDL_RWread(props_file,&symbol,sizeof(char),1), symbol!='\n'){
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
  while (SDL_RWread(props_file,&symbol,sizeof(char),1)==1){
    //    symbol = props_file.get();
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
  SDL_RWclose(props_file);
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
    glVertex2f( (GLfloat)-rectangle.w/2*invertk,(GLfloat)-rectangle.h/2 );
    glTexCoord2f( (frame_dimensions.x+frame_dimensions.w)/kx, (frame_dimensions.y+frame_dimensions.h)/ky );	
    glVertex2f( (GLfloat)rectangle.w/2*invertk, (GLfloat)-rectangle.h/2 );
    glTexCoord2f( (frame_dimensions.x+frame_dimensions.w)/kx, frame_dimensions.y/ky );	
    glVertex2f( (GLfloat)rectangle.w/2*invertk, (GLfloat)rectangle.h/2 );
    glTexCoord2f( frame_dimensions.x/kx, frame_dimensions.y/ky );		
    glVertex2f( (GLfloat)-rectangle.w/2*invertk, (GLfloat)rectangle.h/2 );}
  glEnd();
  glPopMatrix();
}
void CSpriteSheet::draw_int(GLuint frame, GLint x, GLint y){
  if (frame >= (GLuint)rectangle.x * rectangle.y){
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
  collection.insert(std::pair<std::string,CSpriteSheet*>(sheetname,spritesheet));
  return spritesheet;
}

CSpriteSheet* CSpriteSheetManager::dispatch(std::string sheetname){
  return collection[sheetname];
}

CSprite::CSprite(CSpriteSheet* ssheet, Layer layer):
  ssheet(ssheet),x(0),y(0),rotation(0),alpha(1.f),
  tint_r(1.f),tint_g(1.f),tint_b(1.f),v_alpha(0.f),
  min_alpha(0.f),max_alpha(1.f),
  v_x(0.f),v_y(0.f),v_r(0.f),v_scale(0.f),
  min_scale(0.f),max_scale(1000.f),frame(0),
  animation(0),state(0),animation_timer(0),
  next_animation(0),decay_timer(0),
  animation_active(false),decay_active(false),
  scale(1.f),blur(false),layer(layer),follow(0){}

void CSprite::draw(){
#ifdef DEBUG
  if (scale<=0)
    std::cerr << "DIVISION BY ZERO!" << std::endl;
#endif
  glPushAttrib (GL_CURRENT_BIT);
  glColor4f(tint_r, tint_g, tint_b, alpha);
  if (animation_active){
      ssheet -> draw (animation, state, x, y, rotation, scale);
    if (blur){
      glColor4f(tint_r,tint_g,tint_b,.50);
      ssheet -> draw (animation, state, x, y, rotation, scale*1.05f);
      glColor4f(tint_r,tint_g,tint_b,.30);
      ssheet -> draw (animation, state, x, y, rotation, scale*1.10f);
      glColor4f(tint_r,tint_g,tint_b,.20);
      ssheet -> draw (animation, state, x, y, rotation, scale*1.25f);
    }
  }
  else{
      ssheet -> draw (frame, x, y, rotation, scale);
    if (blur){
      glColor4f(tint_r,tint_g,tint_b,.50);
      ssheet -> draw (frame, x, y, rotation, scale*1.05f);
      glColor4f(tint_r,tint_g,tint_b,.30);
      ssheet -> draw (frame, x, y, rotation, scale*1.10f);
      glColor4f(tint_r,tint_g,tint_b,.20);
      ssheet -> draw (frame, x, y, rotation, scale*1.15f);
    }
  }
  glPopAttrib ();
}

int CSprite::start_animation(GLint animation, GLint next_animation){
  if (animation > ssheet -> get_animations()||next_animation > ssheet -> get_animations())
    return -1;
  this -> animation = animation;
  this -> state = 0;
  this -> animation_timer = ssheet -> get_pause(animation,state);
  if (next_animation >= 0)
    this -> next_animation = next_animation;
  else
    this -> next_animation = animation;
  return 0;
}

void CSprite::set_position(GLfloat new_x, GLfloat new_y, GLfloat new_rotation){
  x = new_x;
  y = new_y;
  rotation = new_rotation;
}

void CSprite::set_scale(GLfloat scale, GLfloat v_scale){
  if (scale>0)
    this -> scale = scale;
  this -> v_scale = v_scale;
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
  if (v_r !=0)
    rotation += v_r;
  if (v_alpha !=0)
    alpha += v_alpha;
  if (alpha > max_alpha){
    v_alpha = 0;
    alpha = max_alpha;
  }
  else if (alpha < min_alpha){
    v_alpha = 0;
    alpha = min_alpha;
  }
  if (v_scale != 0)
    scale += v_scale;
  if (scale > max_scale){
    v_scale = 0;
    scale = max_scale;
  }
  else if (alpha < min_scale){
    v_scale = 0;
    scale = min_scale;
  }
  if (scale <= 0.f)
    return DECOMPOSED;
  x += v_x;
  if (x < -GAME_FIELD_WIDTH || x > 2 * GAME_FIELD_WIDTH)
    return DECOMPOSED;
  y += v_y;
  if (y < -GAME_FIELD_HEIGHT || y > 2 * GAME_FIELD_HEIGHT)
    return DECOMPOSED;

  if (animation_active)
    if (--animation_timer == 0){
      if (++state == ssheet -> get_frames(animation)){
	animation=next_animation;
	state = 0;
      }
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
  this -> v_x = cosf((float)M_PI*angle/180)*v;
  this -> v_y = sinf((float)M_PI*angle/180)*v;
  this -> rotation = angle-90.f;
}

CSpriteManager::CSpriteManager():free_handle(1){}

GLuint CSpriteManager::create_sprite(std::string spritesheet, Layer layer){
  CSprite* sprite = new CSprite(game::ssmanager->dispatch(spritesheet), layer);
  GLuint result = free_handle;
  collection.insert(std::pair<GLuint, CSprite*>(result,sprite));
  while(collection.count(free_handle))
    ++free_handle;
  return result;
}

CSprite* CSpriteManager::get_sprite(GLuint handle){
  if (collection.count(handle) == 1)
    return collection[handle];
  else
    return NULL;
}

void  CSpriteManager::draw(Layer layer){
  std::map<GLuint,CSprite*>::iterator i;
  for (i = collection.begin(); i != collection.end(); ++i)
    if (i->second->layer == layer)
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
  for (i = collection.begin();i != collection.end();++i)
    if (i -> second -> follow > 0){
      CSprite* master_sprite = get_sprite(i -> second -> follow);
      if (master_sprite != NULL){
	(get_sprite(i -> first)) -> set_position(master_sprite -> x, master_sprite -> y, 
						 (get_sprite(i->first)) -> rotation);
      }
      else
	i -> second -> follow = 0;
    }
}

GLuint CSpriteManager::destroy_sprite(GLuint handle){
  if (collection.count(handle) == 0)
    return 0;
  collection.erase(handle);
  if (free_handle>handle)
    free_handle=handle;
  return free_handle;
}
