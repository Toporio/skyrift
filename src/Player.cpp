#pragma once
#include "Player.hpp"
#include "Stage.hpp"
#include "config.hpp"
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <memory>

Player::Player(int id, const sf::Texture &texture, const sf::Vector2f &position,
               const sf::Vector2f &velocity, unsigned int lives,
               Stage &stage_data, const sf::Texture &all_texture)
    : Entity(id, texture, position, velocity), lives(lives),
      status(PlayerStatus::IDLE), current_jumps(0), dir_x(1),
      stage_data(stage_data),
      attack_range_cooldown(Config::PLAYER_ATTACK_RANGE__COOLDOWN),
      all_texxt(all_texture), basic_texture(texture) {}
void Player::jump() {
  if (jump_cooldown_timer <= 0 && current_jumps < max_jumps) {
    jump_cooldown_timer = Config::PLAYER_JUMP_COOLDOWN;
    current_jumps += 1;
    velocity.y = -jump_speed;
  }
};
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

void Player::block() {
  std::cout << "block dmg" << std::endl;
  block_cooldown_timer = Config::PLAYER_BLOCK_COOLDOWN;
  block_timer = Config::PLAYER_BLOCK_TIMER;
}

bool Player::attack_animation() {
  auto mele_att_cd = (Config::PLAYER_ATTACK_MELEE__COOLDOWN / 4);
  int frame = int(attack_melee_cooldown / mele_att_cd);
  int off_set = 32 * frame;
  sf::Vector2i offset(off_set, 0);
  sf::Vector2i size(32, 32);
  sprite.setTexture(all_texxt);
  sprite.setTextureRect(sf::IntRect(offset, size));
  if (attack_melee_cooldown <= 0) {
    this->IsAttacking = 0;
    attack_melee_cooldown = 0;
    sprite.setTexture(basic_texture);
    return 1;
  }
  return 0;
}
void Player::attack_ranged(
    std::vector<std::unique_ptr<Projectile>> &projectiles) {
  sf::Vector2f projectile_position = {
      sprite.getPosition().x + sprite.getTexture().getSize().x,
      sprite.getPosition().y + sprite.getTexture().getSize().y};
  auto new_projectile_p = std::make_unique<Projectile>(
      id, stage_data.get_resource_manager().getTexture("pocisk_w_orka"), dir_x,
      projectile_position);
  new_projectile_p->sprite.setScale({2.f, 2.f});
  projectiles.push_back(std::move(new_projectile_p));
  attack_range_cooldown = Config::PLAYER_ATTACK_RANGE__COOLDOWN;
  std::cout << "atak range" << dir_x << std::endl;
}

void Player::attack_melee(Player &enemy) {
  this->sprite.move(sf::Vector2f((this->dir_x > 0 ? 10.f : -10.f), 0.f));
  if (this->check_collision(enemy)) {
    enemy.take_damage(this->dir_x);
  }

  this->sprite.move(sf::Vector2f((this->dir_x > 0 ? -10.f : 10.f), 0.f));
  std::cout << "atak mele" << std::endl;
}
void Player::update(float delta_time) {
  if (attack_range_cooldown > 0)
    attack_range_cooldown -= delta_time;
  if (attack_melee_cooldown > 0)
    attack_melee_cooldown -= delta_time;
  if (block_cooldown_timer > 0)
    block_cooldown_timer -= delta_time;
  if (jump_cooldown_timer > 0)
    jump_cooldown_timer -= delta_time;
  if (dmg_timer > 0)
    dmg_timer -= delta_time;
  position.x += velocity.x * delta_time;
  position.y += velocity.y * delta_time;
  if (is_grounded)
    current_jumps = 0;
  std::cout << is_grounded << std::endl;
  apply_gravity(delta_time);
  sprite.setPosition(position);
  if (IsAttacking)
    if (attack_animation()) {
      for (auto &player_pair : stage_data.get_players())
        if (player_pair.first != id)
          attack_melee(*player_pair.second);
    }
}
void Player::draw(sf::RenderWindow &window) const {
  sf::Sprite sprite_to_draw = sprite;
  sf::IntRect current_frame = sprite_to_draw.getTextureRect();
  if (dir_x == -1.f) {
    current_frame.position.x += current_frame.size.x;
    current_frame.size.x *= -1;
  }
  sprite_to_draw.setTextureRect(current_frame);
  window.draw(sprite_to_draw);
}
void Player::take_damage(float dir) {
  if (block_timer > 0 || dmg_timer > 0) {
    return;
  }
  this->health++;
  this->velocity.x = 5.f * (this->health) * dir;
  this->velocity.y = -10.f * (this->health);
  this->stun_timer = 0.05f * (this->health);
  dmg_timer = Config::PLAYER_DMG_TIMER;
}
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
  if (input_state.attack_melee && !IsAttacking) {
    IsAttacking = 1;
    attack_melee_cooldown = Config::PLAYER_ATTACK_MELEE__COOLDOWN;
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
  if (input_state.block && block_timer <= 0) {
    block();
  }
  return lr;
}
