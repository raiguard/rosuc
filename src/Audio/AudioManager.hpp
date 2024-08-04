#pragma once
#include <filesystem>

typedef struct _Mix_Music Mix_Music;

class AudioManager
{
public:
  AudioManager();
  ~AudioManager();
  void update();

  void playSong(const std::filesystem::path& path, int32_t offset = -1, bool repeat = false);
  bool playingSong() const { return bool(this->music); }

private:
  void play();

  Mix_Music* music = nullptr;
  int32_t offset = -1;
  bool repeat = false;
};
