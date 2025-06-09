#pragma once
#include "Entity.hpp"
#include "common.hpp"
#include "config.hpp"
#include <SFML/Graphics.hpp>
struct TimedProjectileSnapshot {
  sf::Time timestamp;
  ProjectileSnapshot data;
};
class Projectile : public Entity {
private:
  std::deque<TimedProjectileSnapshot> snapshot_buffer;

public:
  Projectile(int id, int owner_id, sf::Texture &texture, float dir_x,
             const sf::Vector2f &position);

  void update(float delta_time) override;
  void draw(sf::RenderWindow &window) const override;
  EntityType get_entity_type() const override;
  ProjectileSnapshot get_projectile_snapshot() const;
  void add_snapshot(const ProjectileSnapshot &snapshot, sf::Time timestamp);
  void apply_snapshot(const ProjectileSnapshot &snapshot);
  void update_interpolation(sf::Time timestamp);
  // bool check_collision(const Entity &other) const override;
  int owner_id;
  int get_owner_id() const;
  int get_id() const;
  float dir_x;
  float damage = Config::PROJECTILE_DAMAGE;
  int hp;
};
