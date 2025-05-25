#include "Game.hpp"
#include "Map.hpp"
#include "Stage.hpp"
#include "Tile.hpp"
#include "common.hpp"
#include "config.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

Game::Game()
    : window(sf::RenderWindow(
          sf::VideoMode({Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}),
          Config::WINDOW_TITLE)),
      m_map(), stage({2, 2}) {
  window.setFramerateLimit(Config::FPS_LIMIT);
  stage.add_player(1, {350.f, 350.f});
  PlayerInputState input_state;
};
void Game::run() {
  sf::Clock clock;
  while (window.isOpen()) {
    sf::Time delta_time = clock.restart();
    handle_events();
    stage.handle_player_input(1, input_state, delta_time.asSeconds());
    stage.update(delta_time.asSeconds());
    stage.render(window);
  }
}
void Game::handle_events() {
  while (const std::optional event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window.close();
    }
    input_state.move_left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    input_state.move_right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
    input_state.jump = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    input_state.attack_melee = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K);
    input_state.attack_ranged =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J);
    input_state.block = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L);
  }
}
