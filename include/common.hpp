#pragma once
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
