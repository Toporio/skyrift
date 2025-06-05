#pragma once
#include "Player.hpp"
#include "Stage.hpp"
#include "common.hpp"
#include "config.hpp"
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <memory>

Player::Player(int id, const sf::Texture &texture, const sf::Vector2f &position,
               const sf::Vector2f &velocity, unsigned int lives,
               Stage &stage_data, const sf::Texture &all_texture)
    : Entity(id, texture, position, velocity), lives(lives),
      status(PlayerStatus::IDLE), current_jumps(0), dir_x(1),
      stage_data(stage_data), animation_timer(0.f),
      attack_range_cooldown(Config::PLAYER_ATTACK_RANGE__COOLDOWN),
      all_texxt(all_texture), basic_texture(texture) {

  attack_melee_cooldown = 0.f;
  block_cooldown_timer = 0.f;
  jump_cooldown_timer = 0.f;
  dmg_timer = 0.f;
  stun_timer = 0.f;
  block_timer = 0.f;
  sprite.setTexture(all_texxt);
}
void Player::jump() {
  if (status == PlayerStatus::HIT_STUN)
    return;
  if (jump_cooldown_timer <= 0 && current_jumps < max_jumps) {
    jump_cooldown_timer = Config::PLAYER_JUMP_COOLDOWN;
    current_jumps += 1;
    velocity.y = -jump_speed;
  }
};
void Player::apply_gravity(float delta_time) {
  velocity.y += Config::GRAVITY * delta_time;
}

void Player::move(float direction) { velocity.x = direction * move_speed; }

void Player::block() {
  std::cout << "block dmg" << std::endl;
  block_cooldown_timer = Config::PLAYER_BLOCK_COOLDOWN;
  block_timer = Config::PLAYER_BLOCK_TIMER;
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

  PlayerStatus previous_status = status;
  switch (status) {
  case PlayerStatus::HIT_STUN: {
    if (stun_timer <= 0.f) {
      status = PlayerStatus::IDLE;
    }
    break;
  }

  case PlayerStatus::ATTACKING_MELEE: {
    const float ATTACK_ANIMATION = Config::PLAYER_ATTACK_MELEE__COOLDOWN;
    const int ATTACK_FRAMES = 6;
    const float FRAME_DURATION = ATTACK_ANIMATION / ATTACK_FRAMES;
    animation_timer += delta_time;
    int frame = static_cast<int>(animation_timer / FRAME_DURATION);
    if (frame >= ATTACK_FRAMES) {
      frame = 0;
      status = is_grounded ? PlayerStatus::IDLE : PlayerStatus::FALLING;
    }

    std::cout << is_grounded << std::endl;
    // sprite.setTexture(all_texxt);
    sprite.setTextureRect(sf::IntRect({230 + frame * 32, 162}, {30, 28}));

    for (auto &pair : stage_data.get_players()) {
      if (pair.first != id) {
        attack_melee(*pair.second);
      }
    }
    break;
  }
  case PlayerStatus::ATTACKING_RANGED: {
    auto &projectiles = stage_data.get_projectiles();
    attack_ranged(projectiles);
    status = PlayerStatus::IDLE;
    break;
  }
  case PlayerStatus::BLOCKING: {
    if (block_timer <= 0.f) {
      status = PlayerStatus::IDLE;
    }
    break;
  }
  default: {
    if (is_grounded) {
      current_jumps = 0;
      if (std::abs(velocity.x) > 0.1f) {
        status = PlayerStatus::RUNNING;
      } else {
        status = PlayerStatus::IDLE;
      }
    } else {
      if (velocity.y < 0) {
        status = PlayerStatus::JUMPING;
      } else {
        status = PlayerStatus::FALLING;
      }
    }
    if (previous_status != status)
      animation_timer = 0.f;
  }
  }

  apply_gravity(delta_time);
  position += velocity * delta_time;
  sprite.setPosition(position);
}
void Player::set_animation(int frame_num, int x_pos, int y_pos, int frame_width,
                           int frame_height, float delta_time,
                           bool loop = false) {
  const float ATTACK_ANIMATION = Config::PLAYER_ATTACK_MELEE__COOLDOWN;
  const int ATTACK_FRAMES = frame_num;
  const float FRAME_DURATION = ATTACK_ANIMATION / ATTACK_FRAMES;
  animation_timer += delta_time;
  int frame = static_cast<int>(animation_timer / FRAME_DURATION);
  if (frame >= ATTACK_FRAMES) {
    frame = loop ? 0 : ATTACK_FRAMES - 1;
  }
  sprite.setTextureRect(sf::IntRect({x_pos + frame * frame_width, y_pos},
                                    {frame_width, frame_height}));
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
  health++;
  velocity.x = 5.f * (health)*dir;
  velocity.y = -10.f * (health);
  stun_timer = 0.05f * (health);
  dmg_timer = Config::PLAYER_DMG_TIMER;
  status = PlayerStatus::HIT_STUN;
}
bool Player::handle_input(const PlayerInputState &input_state,
                          float delta_time) {
  if (status == PlayerStatus::HIT_STUN) {
    move(0.f);
    return false;
  }
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
  bool can_act =
      (status == PlayerStatus::IDLE || status == PlayerStatus::RUNNING ||
       status == PlayerStatus::JUMPING || status == PlayerStatus::FALLING);
  if (input_state.attack_melee && can_act && attack_melee_cooldown <= 0) {
    status = PlayerStatus::ATTACKING_MELEE;
    attack_melee_cooldown = Config::PLAYER_ATTACK_MELEE__COOLDOWN;
  }
  if (can_act && input_state.attack_ranged && attack_range_cooldown <= 0) {
    status = PlayerStatus::ATTACKING_RANGED;
    attack_range_cooldown = Config::PLAYER_ATTACK_RANGE__COOLDOWN;
  }
  if (input_state.jump) {
    jump();
  }
  if (input_state.block && block_timer <= 0) {
    block();
  }
  return lr;
}
