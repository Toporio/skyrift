#pragma once
#include "common.hpp"
#include "config.hpp"
#include <SFML/Graphics.hpp>

class Game {
public:
  Game()
      : window(sf::RenderWindow(
            sf::VideoMode({Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}),
            Config::WINDOW_TITLE)) {
    window.setFramerateLimit(Config::FPS_LIMIT);
  };
  void run();

private:
  void handle_events();
  void update(sf::Time delta_time);
  void render();

  sf::RenderWindow window;
};
