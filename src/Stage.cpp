#pragma once

#include "Player.hpp"
#include "Stage.hpp"
#include "common.hpp"
#include "config.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <memory>
#include <set>

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
    : game_settings(settings), new_player_id(0) {
  try {
    resource_manager.loadTexture("blue_player", Config::BLUE_PLAYER_SPRITES);
    resource_manager.loadTexture("projectile_tex", Config::PROJECTILE_SPRITE);
    resource_manager.loadTexture("terrain_tex", Config::TERRAIN_SPRITE);
    resource_manager.loadTexture("all_sprites", Config::PINK_PLAYER_SPRITES);
    resource_manager.loadTexture("background_texturka", Config::BACKGROUND_TEX);

  } catch (const std::runtime_error &e) {
    std::cerr << "loading texture error" << e.what() << std::endl;
  }
  add_tiles({300.f, 400.f});

  // background_sprite.setTexture(resource_manager.getTexture("background_texturka"));
}

void Stage::add_player(int player_id, const sf::Vector2f &spawn_position) {
  try {
    auto new_player_p = std::make_unique<Player>(
        player_id, resource_manager.getTexture("all_sprites"), spawn_position,
        sf::Vector2f(0.f, 0.f), game_settings.player_lives, *this,
        resource_manager.getTexture("all_sprites"));
    new_player_p->sprite.setScale({3.f, 3.f});
    players.emplace(player_id, std::move(new_player_p));

  } catch (const std::runtime_error &e) {
    std::cerr << "Stage: Failed to add player " << new_player_id
              << ". Reason: " << e.what() << std::endl;
  }
}
int Stage::get_new_projectile_id() {
  int first_valid_id = 0;
  for (auto &projectile : projectiles) {
    if (first_valid_id == projectile->id)
      first_valid_id++;
  }
  return first_valid_id;
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
        std::make_unique<Tile>(i, resource_manager.getTexture("terrain_tex"),
                               pos, sf::Vector2f(0.0f, 0.0f));
    tile->sprite.setTextureRect(temp);
    tile->sprite.setScale(scale);
    x_offset += temp.size.x * scale.x;

    tiles.push_back(std::move(tile));
  }
}
void Stage::update_interpolation(sf::Time timestamp, int client_id) {
  for (auto &pair : players) {
    if (pair.second && pair.first != client_id) {
      pair.second->update_interpolation(timestamp);
    }
  }
  for (auto &projectile : projectiles) {
    if (projectile)
      projectile->update_interpolation(timestamp);
  }
}
void Stage::handle_player_input(int player_id, const PlayerInputState &input,
                                float delta_time) {
  for (const auto &pair : players) {
    if (player_id == pair.first) {
      auto a = pair.second->handle_input(input, delta_time);
      bool b = 0;
      if (a && !pair.second->is_grounded) {
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

    player.position.x = 400.0f;
    player.position.y = 100.0f;
    player.sprite.setPosition(sf::Vector2f(640.0f, -100.0f));
    player.lives--;
    player.health = 0;
    player.velocity.y = 0;
    player.velocity.x = 0;
    player.is_grounded = 0;
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
  check_projectile_map_collision();
  check_player_projectile_collision();
  projectiles.erase(
      std::remove_if(projectiles.begin(), projectiles.end(),
                     [](const std::unique_ptr<Projectile> &p_ptr) {
                       if (!p_ptr) {
                         return true;
                       }
                       return p_ptr->hp <= 0 /* || p_ptr->is_expired() */;
                     }),
      projectiles.end());
}
void Stage::draw_background(sf::RenderWindow &window) {
  sf::Texture back_tex = resource_manager.getTexture("background_texturka");
  sf::Sprite background(back_tex);
  sf::Vector2u textureSize = back_tex.getSize();
  float scaleX = static_cast<float>(Config::WINDOW_WIDTH) / textureSize.x;
  float scaleY = static_cast<float>(Config::WINDOW_HEIGHT) / textureSize.y;
  background.setScale({scaleX, scaleY});
  window.draw(background);
}
void Stage::draw(sf::RenderWindow &window) {
  sf::Sprite background(resource_manager.getTexture("background_texturka"));
  draw_background(window);
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
  window.clear();
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
      if (player_p->get_player_id() == projectile_p->get_owner_id()) {
        continue;
      }
      if (player_p->check_collision(*projectile_p)) {
        player_p->take_damage(projectile_p->dir_x);
        projectile_p->hp--;
      }
    }
  }
}
void Stage::check_projectile_map_collision() {
  for (auto &projectile : projectiles) {
    for (auto &tile : tiles) {
      if (projectile->check_collision(*tile)) {
        projectile->hp--;
      }
    }
  }
}
void Stage::check_player_map_collision(Player &player, float delta_time) {
  sf::FloatRect player_bounds = player.sprite.getGlobalBounds();
  // player_bounds.position.x = player.position.x + 3;
  //  player_bounds.position.y = player.position.y;
  // player_bounds.size.x = 30;
  player.is_grounded = false;
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
        player.is_grounded = true;
        if (overlap_left < overlap_right) {
          player.position.x -= penetration_x;
          player.velocity.x = player.velocity.x < 0 ? 0.f : player.velocity.x;
        } else {
          player.position.x += penetration_x;
          player.velocity.x = player.velocity.x > 0 ? 0.f : player.velocity.x;
        }

        // player.velocity.x = 0.f;
      } else {
        if (overlap_top < overlap_bottom) {
          player.position.y -= penetration_y;
          player.velocity.y = player.velocity.y > 0 ? 0.f : player.velocity.y;
          player.is_grounded = true;
        } else {
          player.position.y += penetration_y;
          player.velocity.y = 0.f;
        }
        player.sprite.setPosition(player.position);
      }
    }
  }
}
StageSnapshot Stage::get_stage_snapshot() const {
  StageSnapshot current_stage_snapshot;
  current_stage_snapshot.game_tick = 0;
  for (auto &pair : players) {
    current_stage_snapshot.players.push_back(
        pair.second->get_player_snapshot());
  }
  for (auto &projectile : projectiles) {
    current_stage_snapshot.projectiles.push_back(
        projectile->get_projectile_snapshot());
  }
  return current_stage_snapshot;
}
void Stage::apply_stage_snapshot(const StageSnapshot &snapshot,
                                 sf::Time timestamp, int player_id) {
  std::set<int> players_in_snapshot;
  for (auto player_snapshot : snapshot.players) {
    players_in_snapshot.insert(player_snapshot.player_id);
    auto it = players.find(player_snapshot.player_id);
    if (it != players.end() && it->second) {
      if (player_id == it->first) {
        it->second->position = player_snapshot.position;
        it->second->velocity = player_snapshot.velocity;
        it->second->status = player_snapshot.status;
        it->second->is_grounded = player_snapshot.is_grounded;
        it->second->dir_x = player_snapshot.dir_x;
        it->second->health = player_snapshot.health;
        it->second->lives = player_snapshot.lives;
      } else
        it->second->add_snapshot(player_snapshot, timestamp);
    }
  }

  std::vector<int> players_to_remove;
  for (const auto &pair : players) {
    if (players_in_snapshot.find(pair.first) == players_in_snapshot.end()) {
      players_to_remove.push_back(pair.first);
    }
  }
  for (int id : players_to_remove)
    remove_player(id);

  for (auto &player_snapshot : snapshot.players) {
    if (players.find(player_snapshot.player_id) == players.end()) {
      add_player(player_snapshot.player_id, player_snapshot.position);
      if (players.count(player_snapshot.player_id)) {
        players.at(player_snapshot.player_id)
            ->add_snapshot(player_snapshot, timestamp);
      }
    }
  }
  std::set<int> projectiles_in_snapshot;
  for (const auto &projectile_snapshot : snapshot.projectiles) {
    projectiles_in_snapshot.insert(projectile_snapshot.projectile_id);
    bool old_projectile = false;
    for (auto &projectile : projectiles) {
      if (projectile && projectile->id == projectile_snapshot.projectile_id) {
        projectile->sprite.setScale({2.f, 2.f});
        projectile->add_snapshot(projectile_snapshot, timestamp);
        old_projectile = true;
      }
    }
    if (!old_projectile)
      add_projectile_from_snapshot(projectile_snapshot);
  }
  projectiles.erase(
      std::remove_if(projectiles.begin(), projectiles.end(),
                     [&](const std::unique_ptr<Projectile> &p_ptr) {
                       if (!p_ptr)
                         return true;
                       return projectiles_in_snapshot.find(p_ptr->id) ==
                              projectiles_in_snapshot.end();
                     }),
      projectiles.end());
}
void Stage::add_projectile_from_snapshot(const ProjectileSnapshot &snapshot) {
  sf::Texture &projectile_texture =
      resource_manager.getTexture("projectile_tex");
  const float damage = Config::PROJECTILE_DAMAGE;
  float dir_x = snapshot.velocity.x > 0 ? 1.f : -1.f;
  const float lifes = 1;
  auto new_projectile = std::make_unique<Projectile>(
      snapshot.projectile_id, snapshot.owner_id, projectile_texture, dir_x,
      snapshot.position);
  new_projectile->sprite.setScale({2.f, 2.f});
  projectiles.push_back(std::move(new_projectile));
}
void Stage::remove_player(int player_id) { players.erase(player_id); }
