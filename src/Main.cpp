#include "Game.hpp"
#include <fmt/format.h>
#include <SDL3/SDL.h>

int main()
{
  Game game;
  game.initBeatmaps("beatmaps");
  game.initWindow();

  while (!game.frame());
}
