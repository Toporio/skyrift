#pragma once
#include <SFML/Graphics.hpp>
#include "common.hpp"
#include "Entity.hpp"
class Tile : public Entity {
public:
    Tile(int id, const sf::Texture& texture, const sf::Vector2f& position);

    ~Tile() = default;

    void update(float delta_time) override {}

    void draw(sf::RenderWindow& window) const override;

    EntityType get_entity_type() const override;

    bool check_collision(const Entity& other) const override;
};