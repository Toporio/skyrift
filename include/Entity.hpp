#pragma once
#include <SFML/Graphics.hpp>
#include "common.hpp"
class Entity {
public:
    virtual Entity();
    virtual ~Entity() = default;
    int id;
    sf::Vector2f position;
    sf::Vector2f velocity;

    sf::Sprite sprite;
    virtual bool check_collision(const Entity& other) const;

    virtual void update(float delta_time);
    virtual void draw() const;
    virtual EntityType get_entity_type() const;

}
