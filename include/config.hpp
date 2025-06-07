#pragma once
#include <string>

namespace Config {
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
const std::string WINDOW_TITLE = "SkyRift";
const unsigned int FPS_LIMIT = 144;
const float PROJECTILE_DAMAGE = 1.0f;
const float PLAYER_ATTACK_RANGE__COOLDOWN = 1.f;
const float PLAYER_ATTACK_MELEE__COOLDOWN = 0.5f;
const float PLAYER_AIR_RESISTANCE = 0.5f;
const float PLAYER_BLOCK_COOLDOWN = 3.f;
const float PLAYER_BLOCK_TIMER = 0.5f;
const float PLAYER_DMG_TIMER = 0.5f;
const float PROJECTILE_SPEED = 500.0f;
const float GRAVITY = 500.f;
const float PLAYER_MOVE_SPEED = 300.0f;
const float PLAYER_JUMP_SPEED = 250.0f;
const float PLAYER_JUMP_COOLDOWN = 0.5f;
const unsigned int PLAYER_MAX_JUMPS = 3;

const float PLAYER_TIMEOUT = 1.f;
const int SERVER_PORT = 7777;
const std::string TERRAIN_SPRITE = "../assets/terrain.png";
const std::string BLUE_PLAYER_SPRITES =
    "../assets/Pink_Monster/Pink_Monster.png";
const std::string PINK_PLAYER_SPRITES = "../assets/pink_player.png";
const std::string WHITE_PLAYER_SPRITES = "../assets/white_player.png";
const std::string PLAYER_ATTACK =
    "../assets/Pink_Monster/Pink_Monster_Attack1_4.png";
const std::string PROJECTILE_SPRITE = "../assets/Rock1.png";
const std::string ROCK = "../assets/Rock1.png";
} // namespace Config
