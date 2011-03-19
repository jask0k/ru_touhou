#include "text.hpp"

CLabel::CLabel(GLint x, GLint y, std::string text, CSpriteSheet* font, text_layer layer, GLuint decay):
  x(x),y(y),align(ALIGN_LEFT),text(text),font(font),layer(layer){
  if (decay>0){
    this->decay_timer = decay;
    this->decay_active = true;
  } else
    this->decay_active = false;
}

CLabel::~CLabel(){
}

decay_state CLabel::think(){
  if (decay_active)
    if (--decay_timer == 0)
      return DECOMPOSED;
  return STILL_ALIVE;
}

void CLabel::change_text(std::string text){
  this->text = text;
}

void CLabel::change_text(GLfloat number){
  std::ostringstream reader;
  text.clear();
  reader << std::fixed << number;
  text = reader.str();
}

void CLabel::draw(){
  std::string::iterator c;
  GLint label_x;
  GLuint char_width = font -> get_width();
  for (c = text.begin(), label_x = x; c < text.end(); ++c, label_x+=(char_width-1)){
    if ((GLuint)*c >= 32 && (GLuint)*c <= 127){ //английские буквы
      font -> draw_int(((GLuint)*c-32),label_x,y);
    } else{ //юникод
    }
  }
}

CLabelManager::CLabelManager():min_handle(1){}

CLabelManager::~CLabelManager(){
  if (labels.size()>0){
    std::map<GLuint,CLabel*>::iterator i;
    for (i = labels.begin();i != labels.end();++i)
      destroy_label (i->first);
  }
    
}

GLuint CLabelManager::text_add(GLint x, GLint y, std::string text, GLuint font_n, text_layer layer, GLuint decay){
  if (font_n < fonts.size()){
    CLabel* label = new CLabel(x, y, text, fonts[font_n], layer, decay);
    GLuint handle = min_handle;
    labels.insert(std::pair<GLuint,CLabel*>(handle,label));
    while (labels.count(min_handle))
      ++min_handle;
    return handle;
  }
  else{
    std::cerr << "Incorrect font!" << std::endl;
    return 0;
  }
}

void CLabelManager::think(){
  if (labels.size() > 0){
    std::map<GLuint,CLabel*>::iterator curr_label;
    for (curr_label = labels.begin(); curr_label != labels.end();)//итерируем надписи
      if (curr_label->second -> think() == DECOMPOSED){//если надпись сгнила, то
	GLuint bad_handle = curr_label -> first;
	++curr_label;
	destroy_label(bad_handle);
      } else {//иначе просто переходим на следующую запись
	++curr_label;
      }
      
  }
}

int CLabelManager::font_load(CSpriteSheet* fontsheet){
  fonts.push_back(fontsheet);
  return fonts.size()-1;
}

void CLabelManager::draw(text_layer layer){
  if (labels.size()>0){
    std::map<GLuint,CLabel*>::iterator curr_label;
    for (curr_label=labels.begin();curr_label!=labels.end();++curr_label){
      if (curr_label->second->layer == layer)
	(curr_label->second)->draw();
    }
  }
}

void CLabelManager::destroy_label(GLuint handle){
  if (labels.count(handle)>0){
    delete labels[handle];
    labels.erase(handle);
    if (min_handle>handle)
      min_handle = handle;
  }
}

CLabel* CLabelManager::get_label(GLuint handle){
  return labels[handle];
}
