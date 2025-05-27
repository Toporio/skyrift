#pragma once
#include "Player.hpp"
#include "Stage.hpp"
#include "config.hpp"
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <memory>

Player::Player(int id, const sf::Texture &texture, const sf::Vector2f &position,
               const sf::Vector2f &velocity, unsigned int lives,
               Stage &stage_data)
    : Entity(id, texture, position, velocity), lives(lives),
      status(PlayerStatus::IDLE), current_jumps(0), dir_x(1),
      stage_data(stage_data),
      attack_range_cooldown(Config::PLAYER_ATTACK_RANGE__COOLDOWN) {}
void Player::jump() { velocity.y = -jump_speed; };
void Player::apply_gravity(float delta_time) {
  velocity.y += Config::GRAVITY * delta_time;
}

void Player::move(float direction) {
  velocity.x = direction * move_speed;
  if (direction != 0.0f)
    status = PlayerStatus::RUNNING;
  else
    status = PlayerStatus::IDLE;
}

void Player::block(bool start_blocking) { return; }

void Player::attack_ranged(
    std::vector<std::unique_ptr<Projectile>> &projectiles) {
  sf::Vector2f projectile_position = {
      sprite.getPosition().x,
      sprite.getPosition().y + sprite.getTexture().getSize().y / 2.f};
  auto new_projectile_p = std::make_unique<Projectile>(
      id, stage_data.get_resource_manager().getTexture("pocisk_w_orka"), dir_x,
      projectile_position);
  new_projectile_p->sprite.setScale({5.f, 5.f});
  projectiles.push_back(std::move(new_projectile_p));
  attack_range_cooldown = Config::PLAYER_ATTACK_RANGE__COOLDOWN;
  std::cout << "atak range" << dir_x << std::endl;
}

void Player::attack_melee(Player &enemy, float dir_x) {
  this->sprite.move(sf::Vector2f((dir_x > 0 ? 10.f : -10.f), 0.f));
  if (this->check_collision(enemy)) {
    enemy.health--;
  }
  this->sprite.move(sf::Vector2f((dir_x > 0 ? -10.f : 10.f), 0.f));
  std::cout << "atak mele" << std::endl;
}
void Player::update(float delta_time) {
  if (attack_range_cooldown > 0)
    attack_range_cooldown -= delta_time;
  position.x += velocity.x * delta_time;
  position.y += velocity.y * delta_time;
  apply_gravity(delta_time);
  sprite.setPosition(position);
}
void Player::draw(sf::RenderWindow &window) const { window.draw(sprite); }

bool Player::handle_input(const PlayerInputState &input_state,
                          float delta_time) {
    bool lr = 0;
  float move_direction = 0.0f;
  if (input_state.move_left) {
    move_direction -= 1.0f;
    lr = 1;
  }
  if (input_state.move_right) {
    move_direction += 1.0f;
    lr = 1;
  }
  if (move_direction != 0.f)
    dir_x = move_direction;
  move(move_direction);
  if (input_state.attack_melee) {
    std::map<int, std::unique_ptr<Player>> &players = stage_data.get_players();
    for (const auto &pair : players) {
      attack_melee(*pair.second, dir_x);
    }
  }
  if (input_state.attack_ranged) {
    if (attack_range_cooldown <= 0) {
      std::vector<std::unique_ptr<Projectile>> &projectiles =
          stage_data.get_projectiles();
      attack_ranged(projectiles);
    }
  }
  if (input_state.jump) {
    jump();
  }
  if (input_state.block) {
    block(true);
  }
  return lr;
}
