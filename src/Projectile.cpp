#include "Projectile.hpp"
#include "common.hpp"
#include "config.hpp"
#include <SFML/Graphics/Texture.hpp>

Projectile::Projectile(int id, sf::Texture &texture, float dir_x,
                       const sf::Vector2f &position)
    : Entity(id, texture, position,
             sf::Vector2f(dir_x * Config::PROJECTILE_SPEED, 0.f)),
      hp(1), dir_x(dir_x) {
  this->sprite.setPosition(position);
}

void Projectile::update(float delta_time) {
  position += velocity * delta_time;
  sprite.setPosition(position);
  if (position.x < 0 || position.x > Config::WINDOW_WIDTH || position.y < 0 ||
      position.y > Config::WINDOW_HEIGHT) {
    hp = 0;
  }
  // if (this->hp <= 0) {
  //  delete
  //}
}

void Projectile::draw(sf::RenderWindow &window) const { window.draw(sprite); }

EntityType Projectile::get_entity_type() const {
  return EntityType::PROJECTILE;
}

int Projectile::get_id() const { return id; }
bool Projectile::check_collision(const Entity &other) const {
  return check_collision(other);
}
