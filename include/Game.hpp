#pragma once
#include "common.hpp"
#include "config.hpp"
#include <Map.hpp>
#include <SFML/Graphics.hpp>

class Game {
public:
  Game();
  void run();

private:
  void handle_events();
  void update(sf::Time delta_time);
  void render();
  sf::Texture rock1;
  sf::Texture rock2;
  Map m_map;
  GameSettings game_settings;
  sf::RenderWindow window;
};
