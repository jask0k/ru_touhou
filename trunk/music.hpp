#ifndef _MUSIC_HPP
#define _MUSIC_HPP

#include "SDL.h"
#include <tr1/memory>
#include <vector>

class CSound;
typedef std::tr1::shared_ptr<CSound> CSoundP;

class CBoomBox {
  std::vector<CSoundP> sounds; // keeps sounds alive while the user needs them
  std::vector<CSoundP> channels; // keeps sounds alive while the playback needs them
public:
  CBoomBox();
  ~CBoomBox();

  size_t create_sound(const char *file);
  size_t create_sound(SDL_RWops *src);
  void destroy_sound(size_t);
  void play_sound(size_t);
};

namespace game {
  extern CBoomBox *boom_box;
}

#endif
