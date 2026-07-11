#include "util.h"

bool is_pos_eq(Position a, Position b) {
    return a.x == b.x && a.y == b.y;
}

bool is_in_bounds(Position pos) {
    return pos.x >= 0 && pos.x < N_COLS && pos.y >= 0 && pos.y < N_ROWS;
}

Tile get_tile(const Grid grid, Position pos) {
    return grid[pos.y][pos.x];
}

TileTypeU8 get_tile_type(const Grid grid, Position pos) {
    return get_tile(grid, pos).type;
}

bool is_player_tile(const Grid grid, Position pos) {
    return get_tile(grid, pos).is_player;
}

void set_tile(Grid grid, Position pos, TileTypeU8 type, uint8_t hp) {
    grid[pos.y][pos.x].type = type;
    grid[pos.y][pos.x].hp = hp;
}

bool is_wall(const Grid grid, Position pos) {
    switch (get_tile_type(grid, pos)) {
    case TILE_ROCK:
    case TILE_CRYSTAL:
        return true;
    case TILE_NORMAL:
    case TILE_MAGMA:
        return false;
    }
}

bool is_in_player_bounds(const Grid grid, Position pos) {
    return is_in_bounds(pos) && is_player_tile(grid, pos);
}

void damage_tile(Grid grid, Position pos, uint8_t damage) {
    Tile *tile = &grid[pos.y][pos.x];
    if (damage >= tile->hp) tile->type = TILE_NORMAL;
    else tile->hp -= damage;
}
