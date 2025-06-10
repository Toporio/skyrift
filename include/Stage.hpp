#pragma once
#include "Map.hpp"
#include "Player.hpp"
#include "Projectile.hpp"
#include "common.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
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
  void add_tiles(const sf::Vector2f &start_position);
  void add_player(int player_id, const sf::Vector2f &spawn_position);
  void remove_player(int player_id);

  void handle_player_input(int player_id, const PlayerInputState &input,
                           float delta_time);
  void update(float delta_time);
  void render(sf::RenderWindow &window);
  void draw(sf::RenderWindow &window);
  void reset_game();
  int get_new_projectile_id();
  StageSnapshot get_stage_snapshot() const;
  void update_interpolation(sf::Time timestamp, int client_id);
  void check_player_map_collision(Player &player, float delta_time);
  void add_projectile_from_snapshot(const ProjectileSnapshot &snapshot);
  void apply_stage_snapshot(const StageSnapshot &snapshot, sf::Time timestamp,
                            int player_id);
  ResourceManager &get_resource_manager() { return resource_manager; }
  std::map<int, std::unique_ptr<Player>> &get_players() { return players; };
  std::vector<std::unique_ptr<Projectile>> &get_projectiles() {
    return projectiles;
  };

private:
  int new_player_id;
  sf::Texture background_tex;
  void check_projectile_map_collision();
  void check_player_projectile_collision();
  void check_player_attack_collision();
  void draw_background(sf::RenderWindow &window);
  void check_player_out_of_map(Player &player);
  GameSettings game_settings;

  std::vector<std::unique_ptr<Tile>> tiles;
  std::map<int, std::unique_ptr<Player>> players;
  std::vector<std::unique_ptr<Projectile>> projectiles;
  ResourceManager resource_manager;

  sf::FloatRect death_bounds;
  void setup_death_bounds();
};
