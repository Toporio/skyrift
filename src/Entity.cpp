#include "Entity.hpp"
#include "common.hpp"
#include <SFML/Graphics.hpp>

Entity::Entity(int id, const sf::Texture &texture, const sf::Vector2f &position,
               const sf::Vector2f &velocity)
    : id(id), position(position), velocity(velocity), sprite(texture) {
  sprite.setPosition(position);
}

bool Entity::check_collision(const Entity &other) const {
  return (this->sprite.getGlobalBounds().findIntersection(
              other.sprite.getGlobalBounds()))
      .has_value();
}
void Entity::draw(sf::RenderWindow &window) const { window.draw(sprite); }
