#include "Game.hpp"
#include <SFML/Graphics.hpp>
#include <cerrno>
#include <cstdlib>
#include <exception>

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
