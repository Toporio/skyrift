#pragma once

#include "common.hpp"
#include <SFML/Network.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <sys/_types/_u_int16_t.h>
#include <sys/_types/_u_int32_t.h>
#include <sys/_types/_u_int8_t.h>
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
  int player_id;
  sf::Vector2f position;
  sf::Vector2f velocity;
  PlayerStatus status;
  int dir_x;
  bool is_grounded;
  int lives;
  float health;
};
inline sf::Packet &operator<<(sf::Packet &packet,
                              const PlayerSnapshot &snapshot) {
  return packet << snapshot.player_id << snapshot.position.x
                << snapshot.position.y << snapshot.velocity.x
                << snapshot.velocity.y << static_cast<u_int8_t>(snapshot.status)
                << snapshot.dir_x << snapshot.is_grounded << snapshot.lives
                << snapshot.health;
}
inline sf::Packet &operator>>(sf::Packet &packet, PlayerSnapshot &snapshot) {
  u_int8_t status;
  packet >> snapshot.player_id >> snapshot.position.x >> snapshot.position.y >>
      snapshot.velocity.x >> snapshot.velocity.y >> status >> snapshot.dir_x >>
      snapshot.is_grounded >> snapshot.lives >> snapshot.health;
  snapshot.status = static_cast<PlayerStatus>(status);
  return packet;
}

struct ProjectileSnapshot {
  int projectile_id;
  int owner_id;
  sf::Vector2f position;
  sf::Vector2f velocity;
};
inline sf::Packet &operator<<(sf::Packet &packet,
                              const ProjectileSnapshot &snapshot) {
  return packet << snapshot.projectile_id << snapshot.owner_id
                << snapshot.position.x << snapshot.position.y
                << snapshot.velocity.x << snapshot.velocity.y;
}
inline sf::Packet &operator>>(sf::Packet &packet,
                              ProjectileSnapshot &snapshot) {
  return packet >> snapshot.projectile_id >> snapshot.owner_id >>
         snapshot.position.x >> snapshot.position.y >> snapshot.velocity.x >>
         snapshot.velocity.y;
}

struct StageSnapshot {
  int game_tick;
  std::vector<PlayerSnapshot> players;
  std::vector<ProjectileSnapshot> projectiles;
  // other info about current game state
};
inline sf::Packet &operator<<(sf::Packet &packet,
                              const StageSnapshot &snapshot) {
  packet << snapshot.game_tick;
  packet << static_cast<u_int8_t>(snapshot.players.size());
  packet << static_cast<u_int16_t>(snapshot.projectiles.size());
  for (auto &player_snapshot : snapshot.players) {
    packet << player_snapshot;
  }
  for (auto &projectile_snapshot : snapshot.projectiles) {
    packet << projectile_snapshot;
  }
  return packet;
}
inline sf::Packet &operator>>(sf::Packet &packet, StageSnapshot &snapshot) {
  packet >> snapshot.game_tick;
  u_int8_t player_count;
  u_int16_t projectile_count;
  packet >> player_count;
  packet >> projectile_count;
  snapshot.players.resize(player_count);
  snapshot.projectiles.resize(projectile_count);
  for (u_int8_t i = 0; i < player_count; ++i) {
    packet >> snapshot.players[i];
  }
  for (u_int16_t i = 0; i < projectile_count; ++i) {
    packet >> snapshot.projectiles[i];
  }
  return packet;
}
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
inline sf::Packet &operator<<(sf::Packet &packet,
                              const PlayerInputState &player_input) {
  return packet << player_input.move_left << player_input.move_right
                << player_input.jump << player_input.attack_melee
                << player_input.attack_ranged << player_input.block
                << player_input.aimDirection.x << player_input.aimDirection.y
                << player_input.sequence_number;
}
inline sf::Packet &operator>>(sf::Packet &packet,
                              PlayerInputState &player_input) {
  return packet >> player_input.move_left >> player_input.move_right >>
         player_input.jump >> player_input.attack_melee >>
         player_input.attack_ranged >> player_input.block >>
         player_input.aimDirection.x >> player_input.aimDirection.y >>
         player_input.sequence_number;
}
struct GameSettings {
  unsigned int max_players;
  unsigned int player_lives = 3;
  float player_health = 0.1f;
};
