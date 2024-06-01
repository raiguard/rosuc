#include "Game.hpp"
#include <fmt/format.h>
#include <SDL3/SDL.h>

int main()
{
  Game game;
  game.readBeatmap("beatmaps/1973808 Neko Hacker - Setsuna no Chikai (feat. Yuzaki Tsukasa (CV Kito Akari)).osz");
  game.initWindow();

  while (!game.frame());
}
