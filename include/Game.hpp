#pragma once
#include "common.hpp"
#include "config.hpp"
#include <SFML/Graphics.hpp>

class Game {
public:
  Game();
  void run();

private:
  void handle_events();
  void update(sf::Time delta_time);
  void render();

  sf::RenderWindow window;
};
