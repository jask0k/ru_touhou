#include "text.hpp"

CLabel::CLabel(GLint x, GLint y, std::string text, int font_no, text_layer layer, GLuint decay):
  x(x),y(y),align(ALIGN_LEFT),text(text),font(game::lmanager->font_get(font_no)),layer(layer){
  if (decay>0){
    this->decay_timer = decay;
    this->decay_active = true;
  } else
    this->decay_active = false;
  game::lmanager -> add_label(this);
}

CLabel::~CLabel(){
  game::lmanager -> del_label(this);
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

CLabelManager::CLabelManager(){}

CLabelManager::~CLabelManager(){
  if (collection.size()>0){
    std::set<CLabel*>::iterator i;
    for (i = collection.begin();i != collection.end();)
      delete *(i++);
  }
    
}

// GLuint CLabelManager::text_add(GLint x, GLint y, std::string text, GLuint font_n, text_layer layer, GLuint decay){
//   if (font_n < fonts.size()){
//     CLabel* label = new CLabel(x, y, text, fonts[font_n], layer, decay);
//     GLuint handle = min_handle;
//     labels.insert(std::pair<GLuint,CLabel*>(handle,label));
//     while (labels.count(min_handle))
//       ++min_handle;
//     return handle;
//   }
//   else{
//     std::cerr << "Incorrect font!" << std::endl;
//     return 0;
//   }
// }

void CLabelManager::add_label(CLabel* label){
  collection.insert(label);
}

void CLabelManager::del_label(CLabel* label){
  collection.erase(label);
}

void CLabelManager::think(){
  if (collection.size() > 0){
    std::set<CLabel*>::iterator curr_label;
    for (curr_label = collection.begin(); curr_label != collection.end();)//итерируем надписи
      if ((*curr_label) -> think() == DECOMPOSED){//если надпись сгнила, то
	//	GLuint bad_handle = curr_label -> first;
	//	++curr_label;
	delete *(curr_label++);
      } else //иначе просто переходим на следующую запись
	++curr_label;
      
  }
}

int CLabelManager::font_load(CSpriteSheet* fontsheet){
  fonts.push_back(fontsheet);
  return fonts.size()-1;
}

CSpriteSheet* CLabelManager::font_get(int no){
  return fonts[no];
}


void CLabelManager::draw(text_layer layer){
  if (collection.size()>0){
    std::set<CLabel*>::iterator curr_label;
    for (curr_label=collection.begin();curr_label!=collection.end();++curr_label){
      if ((*curr_label)->layer == layer)
	(*curr_label)->draw();
    }
  }
}

// void CLabelManager::destroy_label(GLuint handle){
//   if (labels.count(handle)>0){
//     delete labels[handle];
//     labels.erase(handle);
//     if (min_handle>handle)
//       min_handle = handle;
//   }
// }

// CLabel* CLabelManager::get_label(GLuint handle){
//   return labels[handle];
// }
