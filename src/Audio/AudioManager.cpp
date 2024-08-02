#include "Audio/AudioManager.hpp"
#include "Util.hpp"
#include <SDL.h>
#include <SDL_mixer.h>

AudioManager::AudioManager()
{
  if (SDL_Init(SDL_INIT_AUDIO))
    Util::panic("Failed to initialize SDL: {}", SDL_GetError());

  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096))
    Util::panic("Failed to open audio device: {}", SDL_GetError());
}

AudioManager::~AudioManager()
{
  if (this->music)
    Mix_FreeMusic(music);
  Mix_CloseAudio();
}

void AudioManager::playSong(const std::filesystem::path& path)
{
  Mix_Music* music = Mix_LoadMUS(path.c_str());
  if (!music)
    Util::panic("Could not open music: {}", SDL_GetError());

  Mix_PlayMusic(music, 0);

  this->music = music;
}
