#pragma once
#include "common.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
class Entity {
public:
  Entity(int id, const sf::Texture &texture, const sf::Vector2f &position,
         const sf::Vector2f &velocity);
  virtual ~Entity() = default;
  int id;
  sf::Vector2f position;
  sf::Vector2f velocity;

  sf::Sprite sprite;
  virtual bool check_collision(const Entity &other) const;

  virtual void update(float delta_time) = 0;
  virtual void draw(sf::RenderWindow &window) const;
  virtual EntityType get_entity_type() const = 0;
};
