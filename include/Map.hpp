#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Tile.hpp"

class Map {
public:
    Map() = default;

    void add_tile(const Tile& tile);
    void draw(sf::RenderWindow& window) const;

private:
    std::vector<Tile> tiles;
};
