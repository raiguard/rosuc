#pragma once
#include "NamedBool.hpp"
#include <filesystem>
#include <SDL_mixer.h>

typedef struct _Mix_Music Mix_Music;

class AudioManager
{
public:
  using Repeat = NamedBool<class RepeatTag>;

  AudioManager();
  ~AudioManager();
  void update();

  void playSong(const std::filesystem::path& path, int32_t offset = -1, Repeat repeat = Repeat::False);
  bool playingSong() const { return bool(this->music); }
  void setVolume(int volume);

  static constexpr auto MAX_VOLUME = MIX_MAX_VOLUME;

private:
  void play();

  Mix_Music* music = nullptr;
  int32_t offset = -1;
  Repeat repeat;
  std::filesystem::path path;
};
