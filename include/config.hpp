#pragma once
#include "Game.hpp"
#include <string>

namespace Config {
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
const std::string WINDOW_TITLE = "SkyRift";
const unsigned int FPS_LIMIT = 144;

const float GRAVITY = 981.0f * 2.0f;
const float PLAYER_MOVE_SPEED = 300.0f;
const float PLAYER_JUMP_SPEED = -500.0f;
const unsigned int PLAYER_MAX_JUMP = 3;
const std::string BLUE_PLAYER_SPRITES = "../assets/blue_player.png";
const std::string PINK_PLAYER_SPRITES = "../assets/pink_player.png";
const std::string WHITE_PLAYER_SPRITES = "../assets/white_player.png";
} // namespace Config
