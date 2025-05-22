#include "Game.hpp"
#include "Map.hpp"
#include "Tile.hpp"
#include <SFML/Graphics.hpp>
#include <cerrno>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <system_error>

int main() {
  try {
    Game game;
    game.run();

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
