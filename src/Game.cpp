#include "Game.hpp"
#include "config.hpp"
Game::Game()
    : window(sf::RenderWindow(
          sf::VideoMode({Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}),
          Config::WINDOW_TITLE)) {
  window.setFramerateLimit(Config::FPS_LIMIT);
};
void Game::run() {
  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    window.clear();
    window.display();
  }
}
