#pragma once
#include "Map.hpp"
#include "Player.hpp"
#include "Stage.hpp"
#include <iostream>
#include <memory>

Stage::Stage(const GameSettings &settings)
    : game_settings(settings), game_map() {}
void Stage::add_player(int player_id, const sf::Vector2f &spawn_position) {
  try {
    auto new_player_p = std::make_unique<Player>(
        player_id, resource_manager.getTexture("player_default"),
        spawn_position, sf::Vector2f(0.f, 0.f), game_settings.player_lives);
    players.emplace(player_id, std::move(new_player_p));
  } catch (const std::runtime_error &e) {
    std::cerr << "Stage: Failed to add player " << player_id
              << ". Reason: " << e.what() << std::endl;
  }
}
