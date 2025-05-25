#pragma once
#include <string>

namespace Config {
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
const std::string WINDOW_TITLE = "SkyRift";
const unsigned int FPS_LIMIT = 144;
const float PROJECTILE_DAMAGE = 1.0f;
const float PROJECTILE_SPEED = 200.0f;
const float GRAVITY = 9.0f * 2.0f;
const float PLAYER_MOVE_SPEED = 30.0f;
const float PLAYER_JUMP_SPEED = 60.0f;
const unsigned int PLAYER_MAX_JUMPS = 3;
const std::string BLUE_PLAYER_SPRITES =
    "../assets/Pink_Monster/Pink_Monster.png";
const std::string PINK_PLAYER_SPRITES = "../assets/pink_player.png";
const std::string WHITE_PLAYER_SPRITES = "../assets/white_player.png";
} // namespace Config
