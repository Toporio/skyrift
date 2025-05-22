#include "Game.hpp"
#include "Map.hpp"
#include "Tile.hpp"
#include "config.hpp"

Game::Game()
    : window(sf::RenderWindow(
          sf::VideoMode({Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}),
          Config::WINDOW_TITLE)),
      m_map() {
  window.setFramerateLimit(Config::FPS_LIMIT);
  rock1.loadFromFile("../assets/2_Owlet_Monster/Owlet_Monster.png");
  rock2.loadFromFile("../assets/2_Owlet_Monster/Rock1.png");
  Tile tile1(1, rock1, sf::Vector2f(100.0f, 100.0f), sf::Vector2f(0.0f, 0.0f));
  Tile tile2(2, rock2, sf::Vector2f(230.0f, 280.0f), sf::Vector2f(0.0f, 0.0f));
  m_map.add_tile(tile1);
  m_map.add_tile(tile2);
  m_map.draw(window);
};
void Game::run() {
  sf::Clock clock;
  while (window.isOpen()) {
    sf::Time delta_time = clock.restart();
    handle_events();
    update(delta_time);
    render();
  }
}
void Game::handle_events() {
  while (const std::optional event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window.close();
    }
  }
}
void Game::update(sf::Time delta_time) {
  // Update game logic here
}
void Game::render() {
  window.clear();
  m_map.draw(window);
  window.display();
}
