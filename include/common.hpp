#pragma once
#include "common.hpp"
#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>

const unsigned int MAX_PLAYERS = 4;
enum class EntityType { PLAYER, PROJECTILE, TILE };

enum class PlayerStatus {
  IDLE,
  RUNNING,
  JUMPING,
  FALLING,
  ATTACKING_MELEE,
  ATTACKING_RANGED,
  BLOCKING,
  HIT_STUN,
  DEAD
};
struct PlayerSnapshot {
  int id;
  sf::Vector2f position;
  sf::Vector2f velocity;
  PlayerStatus status;
  float dir_x;
  bool is_grounded;
  int lives;
  float health;
};
struct PlayerInputState {
  bool move_left = false;
  bool move_right = false;
  bool jump = false;
  bool attack_melee = false;
  bool attack_ranged = false;
  bool block = false;
  sf::Vector2f aimDirection;
  int sequence_number = 0;
};
struct GameSettings {
  unsigned int player_lives = 3;
  float player_health = 0.1f;
};
