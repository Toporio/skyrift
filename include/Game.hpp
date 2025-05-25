#pragma once
#include "Stage.hpp"
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
  Stage stage;
  sf::Texture rock1;
  PlayerInputState input_state;
  Map m_map;
  GameSettings game_settings;
  sf::RenderWindow window;
};
