#pragma once
#include "Map.hpp"
#include "Player.hpp"
#include "Stage.hpp"
#include "config.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
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
    resource_manager.loadTexture("pocisk_w_orka", Config::PROJECTILE_SPRITE);
    resource_manager.loadTexture("orka_zwisa", Config::TERRAIN_SPRITE);
  } catch (const std::runtime_error &e) {
    std::cerr << "loading texture error" << e.what() << std::endl;
  }
}

void Stage::add_player(int player_id, const sf::Vector2f &spawn_position) {
  try {
    auto new_player_p = std::make_unique<Player>(
        player_id, resource_manager.getTexture("jebac_kurwe_disa"),
        spawn_position, sf::Vector2f(0.f, 0.f), game_settings.player_lives,
        *this);
    new_player_p->sprite.setScale({3.f, 3.f});
    players.emplace(player_id, std::move(new_player_p));
  } catch (const std::runtime_error &e) {
    std::cerr << "Stage: Failed to add player " << player_id
              << ". Reason: " << e.what() << std::endl;
  }
}

void Stage::add_tiles(const sf::Vector2f &start_position) {
  sf::IntRect ending_frame(sf::Vector2i{0, 0}, sf::Vector2i{95, 95});
  sf::IntRect middle_frame(sf::Vector2i{288, 96}, sf::Vector2i{64, 64});
  sf::IntRect temp;
  sf::Vector2f scale{1.f, 1.f};
  for (int i = 0; i < 5; ++i) {
    if (i == 0 || i == 4) {
      temp = ending_frame;
    } else {
      temp = middle_frame;
      scale = {1.5f, 1.5f};
    }
    sf::Vector2f pos = start_position + sf::Vector2f(i * temp.size.x, 400.f);
    auto tile =
        std::make_unique<Tile>(i, resource_manager.getTexture("orka_zwisa"),
                               pos, sf::Vector2f(0.0f, 0.0f));
    tile->sprite.setTextureRect(temp);
    tile->sprite.setScale(scale);
    std::cout << tile->sprite.getPosition().x << std::endl;
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
  for (auto it = projectiles.begin(); it != projectiles.end();) {
    if (!(*it)) {
      it = projectiles.erase(it);
      continue;
    }
    (*it)->update(delta_time);
    if ((*it)->hp == 0) {
      projectiles.erase(it);
    } else {
      ++it;
    }
  }
}
void Stage::draw(sf::RenderWindow &window) {
  for (const auto &tile : tiles) {
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
  window.clear(sf::Color::Yellow);
  draw(window);
  window.display();
}
