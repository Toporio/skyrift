#include "Entity.hpp"
#include "Tile.hpp"
#include "common.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

Tile::Tile(int id, const sf::Texture &texture, const sf::Vector2f &position,
           const sf::Vector2f &velocity)
    : Entity(id, texture, position, velocity) {}
void Tile::draw(sf::RenderWindow &window) const { window.draw(sprite); }

EntityType Tile::get_entity_type() const { return EntityType::TILE; }
