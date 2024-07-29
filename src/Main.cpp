#include "Game.hpp"

int main()
{
  Game game;
  game.initBeatmaps("beatmaps");
  game.initWindow();

  while (!game.frame());
}

// #include <fstream>
// #include <sstream>
// std::ifstream t("scratchmap/404649 yuikonnu - Yume Chizu/yuikonnu - Yume Chizu (Tari) [Hard].osu");
// std::stringstream buffer;
// buffer << t.rdbuf();
// BeatmapInfo beatmap(buffer.str());
// Beatmap beatmap("scratchmap/404649 yuikonnu - Yume Chizu [no video].osz");
