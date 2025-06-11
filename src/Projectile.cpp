#include "Entity.hpp"
#include "Projectile.hpp"
#include "common.hpp"
#include "config.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
inline sf::Vector2f lerp(const sf::Vector2f &start, const sf::Vector2f &end,
                         float factor) {
  return start + (end - start) * factor;
}
Projectile::Projectile(int id, int owner_id, sf::Texture &texture, float dir_x,
                       const sf::Vector2f &position)
    : Entity(id, texture, position,
             sf::Vector2f(dir_x * Config::PROJECTILE_SPEED, 0.f)),
      hp(1), dir_x(dir_x), owner_id(owner_id) {
  this->sprite.setPosition(position);
}

void Projectile::update(float delta_time) {
  position += velocity * delta_time;
  sprite.setPosition(position);
  if (position.x < 0 || position.x > Config::WINDOW_WIDTH || position.y < 0 ||
      position.y > Config::WINDOW_HEIGHT) {
    hp--;
  }
}
void Projectile::add_snapshot(const ProjectileSnapshot &snapshot,
                              sf::Time timestamp) {
  snapshot_buffer.push_back({timestamp, snapshot});
  const int MAX_SIZE = 10;
  while (snapshot_buffer.size() > MAX_SIZE) {
    snapshot_buffer.pop_front();
  }
}
void Projectile::update_interpolation(sf::Time timestamp) {

  if (snapshot_buffer.size() < 2) {
    if (!snapshot_buffer.empty()) {
      sprite.setPosition(snapshot_buffer.back().data.position);
    }
    return;
  }
  TimedProjectileSnapshot *from = nullptr;
  TimedProjectileSnapshot *to = nullptr;
  for (auto &snapshot : snapshot_buffer) {
    if (snapshot.timestamp <= timestamp) {
      from = &snapshot;
    } else {
      to = &snapshot;
      break;
    }
  }
  if (!from || !to) {
    sprite.setPosition(snapshot_buffer.back().data.position);
    position = snapshot_buffer.back().data.position;
    velocity = snapshot_buffer.back().data.velocity;
    return;
  }
  sf::Time time_difference = to->timestamp - from->timestamp;
  sf::Time progress_time = timestamp - from->timestamp;
  float factor = progress_time.asSeconds() / time_difference.asSeconds();
  if (factor < 0.f)
    factor = 0.f;
  if (factor > 1.f)
    factor = 1.f;
  sf::Vector2f interpolated_position =
      lerp(from->data.position, to->data.position, factor);
  position = interpolated_position;
  sprite.setPosition(interpolated_position);
  velocity = to->data.velocity;
  std::cout << "uzyto interpolacji projectile" << std::endl;
}
void Projectile::draw(sf::RenderWindow &window) const { window.draw(sprite); }

EntityType Projectile::get_entity_type() const {
  return EntityType::PROJECTILE;
}
int Projectile::get_owner_id() const { return owner_id; }
int Projectile::get_id() const { return id; }
// bool Projectile::check_collision(const Entity &other) const {
// return Entity::check_collision(other);
//}
ProjectileSnapshot Projectile::get_projectile_snapshot() const {
  return {id, owner_id, position, velocity};
}
void Projectile::apply_snapshot(const ProjectileSnapshot &snapshot) {
  id = snapshot.projectile_id;
  owner_id = snapshot.owner_id;
  position = snapshot.position;
  velocity = snapshot.velocity;
}
