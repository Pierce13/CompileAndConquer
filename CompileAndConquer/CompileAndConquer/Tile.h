#pragma once

enum class TileType {
    Wall,
    Floor,
    Player,
    Enemy,
    Item,
    Stairs
};

inline char tileToChar(TileType tile) {
    switch (tile)
    {
    case TileType::Wall: return '#';
    case TileType::Floor: return '.';
    case TileType::Player: return '@';
    case TileType::Enemy: return 'E';
    case TileType::Item: return 'I';
    case TileType::Stairs: return '>';
    default: return '?';
    }
}

