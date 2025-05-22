#include "Entity.hpp"
#include "common.hpp"
#include <SFML/Graphics.hpp>

Entity::Entity(int id, const sf::Texture &texture, const sf::Vector2f &position,
               const sf::Vector2f &velocity)
    : id(id), position(position), velocity(velocity), sprite(texture) {}
bool Entity::check_collision(const Entity &other) const {
  sf::FloatRect intersection;
  return false;
  // return sprite.getGlobalBounds().findIntersection(
  //   other.sprite.getGlobalBounds(), intersection);
}
void Entity::draw(sf::RenderWindow &window) const { window.draw(sprite); }
