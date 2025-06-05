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
    resource_manager.loadTexture("all_sprites", Config::PLAYER_ATTACK);
  } catch (const std::runtime_error &e) {
    std::cerr << "loading texture error" << e.what() << std::endl;
  }
}

void Stage::add_player(int player_id, const sf::Vector2f &spawn_position) {
  try {
    auto new_player_p = std::make_unique<Player>(
        player_id, resource_manager.getTexture("jebac_kurwe_disa"),
        spawn_position, sf::Vector2f(0.f, 0.f), game_settings.player_lives,
        *this, resource_manager.getTexture("all_sprites"));
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
  float x_offset = 0.f;
  float y_offset = 0.f;
  for (int i = 0; i < 6; ++i) {
    if (i == 0 || i == 5) {
      temp = ending_frame;
      if (i == 5) {
        y_offset = -100.f;
        x_offset += 150.f;
      }
      scale = {1.f, 1.f};
    } else {
      temp = middle_frame;
      scale = {1.5f, 1.5f};
    }
    sf::Vector2f pos = start_position + sf::Vector2f(x_offset, y_offset);
    auto tile =
        std::make_unique<Tile>(i, resource_manager.getTexture("orka_zwisa"),
                               pos, sf::Vector2f(0.0f, 0.0f));
    tile->sprite.setTextureRect(temp);
    tile->sprite.setScale(scale);
    x_offset += temp.size.x * scale.x;
    std::cout << tile->sprite.getPosition().x << std::endl;
    tiles.push_back(std::move(tile));
  }
}

void Stage::handle_player_input(int player_id, const PlayerInputState &input,
                                float delta_time) {
  for (const auto &pair : players) {
    if (player_id == pair.first) {
      auto a = pair.second->handle_input(input, delta_time);
      bool b = 0;
      if (a && !pair.second->IsGrounded) {
        pair.second->sprite.setPosition(sf::Vector2f(
            pair.second->position.x + pair.second->velocity.x * delta_time,
            pair.second->position.y - 1.0f));
        for (const auto &tile : tiles) {
          if (pair.second->check_collision(*tile)) {
            b = true;
          }
        }
        pair.second->sprite.setPosition(sf::Vector2f(
            pair.second->position.x - pair.second->velocity.x * delta_time,
            pair.second->position.y + 1.0f));
        if (b) {
          pair.second->velocity.x = 0.0f;
        }
      }
    }
  }
}
void Stage::check_player_out_of_map(Player &player) {
  auto x_side = player.sprite.getGlobalBounds().getCenter().x;
  auto y_side = player.sprite.getGlobalBounds().getCenter().y;
  if (x_side < -500 || x_side > Config::WINDOW_WIDTH + 500 ||
      y_side > Config::WINDOW_HEIGHT + 500) {
    std::cout << x_side << y_side;
    player.position.x = 400.0f;
    player.position.y = 100.0f;
    player.sprite.setPosition(sf::Vector2f(640.0f, -100.0f));
    player.lives--;
    player.health = 0;
    player.velocity.y = 0;
    player.velocity.x = 0;
    player.IsGrounded = 0;
  }
}
void Stage::update(float delta_time) {
  for (const auto &pair : players) {
    pair.second->update(delta_time);
    check_player_map_collision(*pair.second, delta_time);
    check_player_out_of_map(*pair.second);
  }
  for (const auto &projectile : projectiles) {
    projectile->update(delta_time);
  }
  check_player_projectile_collision();
  projectiles.erase(
      std::remove_if(projectiles.begin(), projectiles.end(),
                     [](const std::unique_ptr<Projectile> &p_ptr) {
                       if (!p_ptr) {
                         return true; // Usuń puste wskaźniki (zabezpieczenie)
                       }
                       // Pocisk jest usuwany, jeśli jest oznaczony LUB jego
                       // czas życia minął (is_expired)
                       return p_ptr->hp == 0 /* || p_ptr->is_expired() */;
                     }),
      projectiles.end());
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
void Stage::check_player_projectile_collision() {
  for (auto &player_pair : players) {
    std::unique_ptr<Player> &player_p = player_pair.second;

    if (!player_p) {
      continue;
    }
    for (auto &projectile_p : projectiles) {

      if (!projectile_p) {
        continue;
      }
      if (player_p->get_player_id() == projectile_p->get_id()) {
        continue;
      }
      if (player_p->check_collision(*projectile_p)) {
        player_p->take_damage(projectile_p->dir_x);
        projectile_p->hp--;
      }
    }
  }
}
void Stage::check_player_map_collision(Player &player, float delta_time) {
  sf::FloatRect player_bounds = player.sprite.getGlobalBounds();
  player.IsGrounded = false;
  //  player_bounds.size.x -= 40;
  // player_bounds.position.x += 20;
  for (const auto &tile_p : tiles) {
    if (!tile_p) {
      continue;
    }
    if (player.check_collision(*tile_p)) {
      sf::FloatRect tile_bounds = tile_p->sprite.getGlobalBounds();
      float overlap_left = (player_bounds.position.x + player_bounds.size.x) -
                           tile_bounds.position.x;
      float overlap_right = (tile_bounds.position.x + tile_bounds.size.x) -
                            player_bounds.position.x;
      float overlap_top = (player_bounds.position.y + player_bounds.size.y) -
                          tile_bounds.position.y;
      float overlap_bottom = (tile_bounds.position.y + tile_bounds.size.y) -
                             player_bounds.position.y;
      float penetration_x =
          (overlap_left < overlap_right) ? overlap_left : overlap_right;
      float penetration_y =
          (overlap_top < overlap_bottom) ? overlap_top : overlap_bottom;
      if (penetration_x < penetration_y) {
        if (overlap_left < overlap_right) {
          player.position.x -= penetration_x;
        } else {
          player.position.x += penetration_x;
        }

        player.velocity.x = 0.f;
      } else {
        if (overlap_top < overlap_bottom) {
          player.position.y -= penetration_y;
          player.velocity.y = 0.f;
          player.IsGrounded = true;
        } else {
          player.position.y += penetration_y;
          player.velocity.y = 0.f;
        }
        player.sprite.setPosition(player.position);
      }
    }
  }
}
