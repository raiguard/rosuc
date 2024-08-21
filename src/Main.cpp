#include "Game.hpp"

int main()
{
  auto game = std::make_unique<Game>();
  game->init();
  while (!game->frame());
}
