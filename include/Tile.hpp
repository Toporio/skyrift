#pragma once
#include "Entity.hpp"
#include "common.hpp"
#include <SFML/Graphics.hpp>
class Tile : public Entity {
public:
  Tile(int id, const sf::Texture &texture, const sf::Vector2f &position,
       const sf::Vector2f &velocity);
  ~Tile() = default;

  void update(float delta_time) override {}

  void draw(sf::RenderWindow &window) const override;

  EntityType get_entity_type() const override;
};
