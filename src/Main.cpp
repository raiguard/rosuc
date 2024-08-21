#include "Game.hpp"

int main()
{
  auto game = std::make_unique<Game>();
  while (!game->frame());
}
