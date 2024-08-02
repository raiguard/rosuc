#include "Game.hpp"

int main()
{
  Game game;
  game.initBeatmaps("beatmaps");
  game.init();

  while (!game.frame());
}
