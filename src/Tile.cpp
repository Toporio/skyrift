#include "Entity.hpp"
#include "Tile.hpp"
#include "common.hpp"

Tile::Tile(int id, const sf::Texture &texture, const sf::Vector2f &position) {
  this->id = id;
  this->position = position;
  this->velocity = sf::Vector2f(0.f, 0.f);
  this->sprite.setTexture(texture);
  this->sprite.setPosition(position);
}

void Tile::draw(sf::RenderWindow &window) const { window.draw(sprite); }

EntityType Tile::get_entity_type() const { return EntityType::Tile; }

bool Tile::check_collision(const Entity &other) const {
  sf::FloatRect thisBounds = sprite.getGlobalBounds();
  sf::FloatRect otherBounds = other.sprite.getGlobalBounds();

  return thisBounds.intersects(otherBounds);
}
