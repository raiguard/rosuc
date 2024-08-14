#include "Audio/AudioManager.hpp"
#include "Util.hpp"
#include <SDL.h>
#include <SDL_mixer.h>

AudioManager::AudioManager()
{
  if (SDL_Init(SDL_INIT_AUDIO))
    Util::panic("Failed to initialize SDL audio: {}", SDL_GetError());

  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096))
    Util::panic("Failed to open audio device: {}", SDL_GetError());

  Mix_VolumeMusic(10);
}

AudioManager::~AudioManager()
{
  if (this->music)
    Mix_FreeMusic(music);
  Mix_CloseAudio();
}

void AudioManager::update()
{
  if (this->repeat)
    this->play();
}

void AudioManager::playSong(const std::filesystem::path& path, int32_t offset, Repeat repeat)
{
  if (path == this->path)
    return;

  if (this->music)
    Mix_FreeMusic(this->music);
  this->music = Mix_LoadMUS(path.c_str());
  if (!this->music)
    Util::panic("Could not open music: {}", SDL_GetError());
  this->offset = offset;
  this->repeat = repeat;
  this->path = path;
  this->play();
}

void AudioManager::play()
{
  if (Mix_PlayingMusic())
    return;

  Mix_PlayMusic(this->music, 0);
  if (offset > 0)
    if (Mix_SetMusicPosition(double(offset) / 1000.0))
      Util::panic("Failed to set music position");
}

void AudioManager::setVolume(int volume)
{
  Mix_VolumeMusic(volume);
}
