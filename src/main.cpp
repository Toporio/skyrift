#include "Game.hpp"
#include "Map.hpp"
#include "Tile.hpp"
#include <SFML/Graphics.hpp>
#include <cerrno>
#include <cstdlib>
#include <exception>

int main() {
  try {
    Game game;
    game.run();
    sf::Texture rock1("Rock1.png");
    Tile tile1(1, rock1, sf::Vector2f(100.0f, 100.0f));
    sf::Texture rock2("Rock2.png");
    Tile tile2(2, rock2, sf::Vector2f(230.0f, 280.0f));
    Map map1();
    map1.add_tile(tile1);
    map1.add_tile(tile2);
    map1.draw(game.window);
    game.window.display();

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
