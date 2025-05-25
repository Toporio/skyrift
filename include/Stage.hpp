#pragma once
#include "Map.hpp"
#include "Player.hpp"
#include "Projectile.hpp"
#include "common.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <map>
#include <memory>
#include <vector>

class ResourceManager {
public:
  sf::Texture &getTexture(const std::string &name);
  void loadTexture(const std::string &name, const std::string &filename);

private:
  std::map<std::string, sf::Texture> textures;
};

class Stage {
public:
  Stage(const GameSettings &settings);
  void add_player(int player_id, const sf::Vector2f &spawn_position);
  void remove_player(int player_id);

  void handle_player_input(int player_id, const PlayerInputState &input,
                           float delta_time);
  void update(float delta_time);
  void render(sf::RenderWindow &window);
  void draw(sf::RenderWindow &window);
  void reset_game();
  Map &get_map() { return game_map; }

private:
  void check_player_map_collision(Player &player, float delta_time);
  void check_projectile_map_collision(Projectile &projectile);
  void check_player_projectile_collision();
  void check_player_attack_collision();

  GameSettings game_settings;
  Map game_map;
  std::map<int, std::unique_ptr<Player>> players;
  std::vector<std::unique_ptr<Projectile>> projectiles;
  ResourceManager resource_manager;

  sf::FloatRect death_bounds;
  void setup_death_bounds();
};
