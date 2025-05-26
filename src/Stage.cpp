#pragma once
#include "Map.hpp"
#include "Player.hpp"
#include "Stage.hpp"
#include "config.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>
#include <memory>

sf::Texture &ResourceManager::getTexture(const std::string &name) {
  try {
    return textures.at(name);
  } catch (const std::out_of_range &e) {
    std::cerr << "resource manager error: texture" << name << "not found."
              << std::endl;
    throw std::runtime_error("Texture net found" + name);
  }
}
void ResourceManager::loadTexture(const std::string &name,
                                  const std::string &filename) {
  sf::Texture texture;
  if (!texture.loadFromFile(filename)) {
    std::cerr << "ResourceManager Error: Failed to load texture '" << name
              << "' from file: " << filename << std::endl;
    throw std::runtime_error("Failed to load texture: " + name);
  }
  textures[name] = texture;
}
Stage::Stage(const GameSettings &settings)
    : game_settings(settings), game_map() {
  try {
    resource_manager.loadTexture("jebac_kurwe_disa",
                                 Config::BLUE_PLAYER_SPRITES);
  } catch (const std::runtime_error &e) {
    std::cerr << "loading texture error" << e.what() << std::endl;
  }
  try {
      resource_manager.loadTexture("orka_zwisa",
          Config::ROCK);
  }
  catch (const std::runtime_error& e) {
      std::cerr << "loading texture error" << e.what() << std::endl;
  }
}

void Stage::add_player(int player_id, const sf::Vector2f &spawn_position) {
  try {
    auto new_player_p = std::make_unique<Player>(
        player_id, resource_manager.getTexture("jebac_kurwe_disa"),
        spawn_position, sf::Vector2f(0.f, 0.f), game_settings.player_lives);
    players.emplace(player_id, std::move(new_player_p));
  } catch (const std::runtime_error &e) {
    std::cerr << "Stage: Failed to add player " << player_id
              << ". Reason: " << e.what() << std::endl;
  }
}

void Stage::add_tiles(const sf::Vector2f& start_position) {
    float tile_width = resource_manager.getTexture("orka_zwisa").getSize().x;
    for (int i = 0; i < 4; ++i) {
        sf::Vector2f pos = start_position + sf::Vector2f(i * tile_width, 40.f);
        auto tile = std::make_unique<Tile>(
            i,
            resource_manager.getTexture("orka_zwisa"),
            pos,
            sf::Vector2f(0.f, 0.f)
        );
        tiles.push_back(std::move(tile));
    }
}

void Stage::handle_player_input(int player_id, const PlayerInputState &input,
                                float delta_time) {
  for (const auto &pair : players) {
    if (player_id == pair.first) {
      pair.second->handle_input(input, delta_time);
    }
  }
}
void Stage::update(float delta_time) {
  for (const auto &pair : players) {
    pair.second->update(delta_time);
  }
  for (auto &projectile : projectiles) {
    projectile->update(delta_time);
  }
}
void Stage::draw(sf::RenderWindow &window) {
  game_map.draw(window);
  for (const auto& tile : tiles) {
      tile->draw(window);
  }
  for (const auto &pair : players) {
    pair.second->draw(window);
  }
  for (auto &projectile : projectiles) {
    projectile->draw(window);
  }
}
void Stage::render(sf::RenderWindow &window) {
  window.clear();
  draw(window);
  window.display();
}
