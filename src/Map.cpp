#pragma once
#include "Map.hpp"

void Map::add_tile(const Tile& tile) {
    tiles.push_back(tile);
}

void Map::draw(sf::RenderWindow& window) const {
    for (const auto& tile : tiles) {
        tile.draw(window);
    }
}
