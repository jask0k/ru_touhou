#include "text.hpp"

CLabel::CLabel(GLint x, GLint y, std::string text, CSpriteSheet* font, GLuint decay):
  x(x),y(y),align(ALIGN_LEFT),text(text),font(font){
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

CText::CText(CSpriteSheetManager* manager): ssmanager(manager){}

CText::~CText(){
  if (labels.size()>0){
    std::vector<CLabel*>::iterator i;
    for (i = labels.begin();i < labels.end();++i)
      delete *i;
  }
    
}

CLabel* CText::text_add(GLint x, GLint y, std::string text, GLuint font_n, GLuint decay){
  if (font_n < fonts.size()){
    CLabel* label = new CLabel(x, y, text, fonts[font_n], decay);
    labels.push_back(label);
    return label;
  }
  else{
    std::cerr << "Incorrect font!" << std::endl;
    return NULL;
  }
}

void CText::think(){
  if (labels.size() > 0){
    std::vector<CLabel*>::iterator curr_label;
    for (curr_label = labels.begin(); curr_label < labels.end();)//итерируем надписи
      if ((*curr_label) -> think() == DECOMPOSED){//если надпись сгнила, то
	delete *curr_label;//удаляем её из памяти
	curr_label = labels.erase(curr_label);//и из списка, переходя на следующую запись
      } else {//иначе просто переходим на следующую запись
	++curr_label;
      }
      
  }
}

int CText::font_load(std::string fontname){
  CSpriteSheet* font_sheet = ssmanager -> dispatch(fontname);
  fonts.push_back(font_sheet);
  return 0;
}

void CText::draw(){
  if (labels.size()>0){
    std::vector<CLabel*>::iterator curr_label;
    for (curr_label=labels.begin();curr_label<labels.end();++curr_label){
      (*curr_label)->draw();
    }
  }
}
