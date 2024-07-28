#include "Game.hpp"

int main()
{
  Game game;
  game.initBeatmaps("beatmaps");
  game.initWindow();

  while (!game.frame());
}
