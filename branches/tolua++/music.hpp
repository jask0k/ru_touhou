#ifndef _MUSIC_HPP
#define _MUSIC_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <tr1/memory>
#include <vector>
#include <iostream>
#include <cstdlib>

class CSound;
typedef std::tr1::shared_ptr<CSound> CSoundP;

class CMusic;
typedef std::tr1::shared_ptr<CMusic> CMusicP;

class CBoomBox { //tolua_export
  std::vector<CSoundP> sounds; // keeps sounds alive while the user needs them
  std::vector<CSoundP> channels; // keeps sounds alive while the playback needs them
  CMusicP music;
  bool no_sound;
public:
  //tolua_begin
  CBoomBox();
  ~CBoomBox();

  size_t create_sound(const char *file);
  size_t create_sound(SDL_RWops *src);
  void destroy_sound(size_t);
  void play_sound(size_t);

  void play_music(const char *file);
  void play_music(SDL_RWops *src);

  void sound_disable(){no_sound = true;};
};
//tolua_end

namespace game {
  extern CBoomBox *boom_box;//tolua_export
}

#endif
