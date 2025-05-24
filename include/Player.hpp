#pragma once
#include "Entity.hpp"
#include "common.hpp"
#include "config.hpp"

class Projectile;

class Player : public Entity {
private:
  const float move_speed = Config::PLAYER_MOVE_SPEED;
  const float jump_speed = Config::PLAYER_JUMP_SPEED;
  const unsigned int max_jumps = Config::PLAYER_MAX_JUMPS;
  unsigned int current_jumps = 0;

public:
  Player(int id, const sf::Texture &texture, const sf::Vector2f &position,
         const sf::Vector2f &velocity, unsigned int lives);
  int lives;
  PlayerStatus status;
  float health;

  float hit_stun_timer;
  float attack_cooldown_timer;
  float block_cooldown_timer;

  void handle_input(const PlayerInputState &input_state, float delta_time);
  void update(float delta_time) override;
  EntityType get_entity_type() const override { return EntityType::PLAYER; };

  // player movement
  // direction: -1 left, 1 right, 0 none
  void move(float direction);
  void jump();
  void apply_gravity(float delta_time);
  // player attacks
  void attack_melee(Player& enemy, float dir_x);
  void attack_ranged(std::vector<Projectile> &projectiles,
                     sf::Vector2f &direction);
  void block(bool start_blocking);
  // player interactions
  void take_damage(float damage);
  void on_block_attack();
  bool is_alive();
  void reset_player();

  // getters
  int get_player_id() const { return id; }
};
