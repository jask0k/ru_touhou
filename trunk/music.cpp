#include "music.hpp"

////////////////////////////////////////////////
// CSound
////////////////////////////////////////////////

class CSound {
  Mix_Chunk *chunk;
public:
  CSound(const char *file) : chunk(0) {
    Mix_SetError("");
    if(file) {
      chunk = Mix_LoadWAV(file);
    }
    if(!chunk) {
      std::cerr << Mix_GetError() << std::endl;
      std::cerr << "Error: failed to load sound from file " << file << std::endl;
    }
  }
  CSound(SDL_RWops *src) : chunk(0) {
    Mix_SetError("");
    if(src) {
      chunk = Mix_LoadWAV_RW(src, 0);
    }
    if(!chunk) {
      std::cerr << Mix_GetError() << std::endl;
      std::cerr << "Error: failed to load sound from SDL_RWops " << src << std::endl;
    }
  }
  ~CSound() {
    if(chunk) {
      Mix_FreeChunk(chunk);
    }
  }
  int play() {
    int ret = -1;
    if(chunk) {
      ret = Mix_PlayChannel(-1, chunk, 0);
      if(ret < 0) {
        std::cerr << Mix_GetError() << std::endl;
        std::cerr << "Error: failed to play sound" << std::endl;
      }
    }
    return ret;
  }
};

////////////////////////////////////////////////
// CMusic
////////////////////////////////////////////////

class CMusic {
  Mix_Music *music;
public:
  CMusic(const char *file) : music(0) {
    Mix_SetError("");
    if(file) {
      music = Mix_LoadMUS(file);
    }
    if(!music) {
      std::cerr << Mix_GetError() << std::endl;
      std::cerr << "Error: failed to load music from file " << file << std::endl;
    }
  }
  CMusic(SDL_RWops *src) : music(0) {
    Mix_SetError("");
    if(src) {
      music = Mix_LoadMUS_RW(src);
    }
    if(!music) {
      std::cerr << Mix_GetError() << std::endl;
      std::cerr << "Error: failed to load music from SDL_RWops " << src << std::endl;
    }
  }
  ~CMusic() {
    if(music) {
      Mix_FreeMusic(music);
    }
  }
  int play() {
    int ret = -1;
    if(music) {
      ret = Mix_PlayMusic(music, -1);
      if(ret < 0) {
        std::cerr << Mix_GetError() << std::endl;
        std::cerr << "Error: failed to play music" << std::endl;
      }
    }
    return ret;
  }
};

////////////////////////////////////////////////
// CBoomBox
////////////////////////////////////////////////

CBoomBox::CBoomBox() : no_sound(true) {
  bool sdl_audio_preinited = SDL_INIT_AUDIO & SDL_WasInit(SDL_INIT_AUDIO);
  if(!sdl_audio_preinited) {
    if( SDL_InitSubSystem(SDL_INIT_AUDIO) < 0 ) {
      std::cerr << SDL_GetError() << std::endl;
      std::cerr << "Error: failed SDL_INIT_AUDIO" << std::endl;
      return;
    }
  }

  if( (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0) && (!no_sound)) {
    std::cerr << Mix_GetError() << std::endl;
    std::cerr << "Error: failed Mix_OpenAudio" << std::endl;
    return;
  }

  channels.resize(Mix_AllocateChannels(-1));

  no_sound = false;
}

CBoomBox::~CBoomBox() {
  if (no_sound)
    return;

  music.reset(); // destroy CMusic instance (this automatically stops music playback)

  // request for all playback to halt
  Mix_HaltChannel(-1);

  while(Mix_Playing(-1) > 0) {
    std::cerr << "Panic: failed to halt all channels" << std::endl;
    abort();
  }

  sounds.clear(); // destroy all CSound instances BEFORE calling Mix_CloseAudio
  channels.clear(); // destroy all CSound instances BEFORE calling Mix_CloseAudio

  Mix_CloseAudio();
}

size_t CBoomBox::create_sound(const char *file) {
  if (no_sound)
    return 0;
  CSoundP sound(new CSound(file));
  for(size_t i = 0, size = sounds.size(); i < size; ++i) {
    if(!sounds[i]) {
      sounds[i] = sound;
      return i;
    }
  }
  sounds.push_back(sound);
  return sounds.size() - 1;
}

size_t CBoomBox::create_sound(SDL_RWops *src) {
  if (no_sound)
    return 0;
  CSoundP sound(new CSound(src));
  for(size_t i = 0, size = sounds.size(); i < size; ++i) {
    if(!sounds[i]) {
      sounds[i] = sound;
      return i;
    }
  }
  sounds.push_back(sound);
  return sounds.size() - 1;
}

void CBoomBox::destroy_sound(size_t i) {
  if (no_sound)
    return;
  if(i < sounds.size()) {
    sounds[i].reset();
  }
}

void CBoomBox::play_sound(size_t i) {
  if (no_sound)
    return;
  if(i < sounds.size()) {
    CSoundP sound(sounds[i]);
    if(sound) {
      int channel = sound->play();
      if(channel >= 0) {
        if(channel < (int) channels.size()) {
          channels[channel] = sound;
        } else {
          std::cerr << "Panic: unexpected channel number " << channel << std::endl;
          abort();
        }
      }
    }
  }
}

void CBoomBox::play_music(const char *file) {
  if (no_sound)
    return;
  music.reset(new CMusic(file));
  music->play();
}

void CBoomBox::play_music(SDL_RWops *src) {
  if (no_sound)
    return;
  music.reset(new CMusic(src));
  music->play();
}
