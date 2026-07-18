#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define GAME_WIDTH 180
#define GAME_HEIGHT 180
#define TICK_RATE 0.1
#define N_ROWS 3
#define N_COLS 6

typedef struct {
    uint8_t x;
    uint8_t y;
} Position;

typedef enum {
    TILE_NORMAL,
    TILE_ROCK,
    TILE_CRYSTAL,
    TILE_MAGMA,
} TileType;

typedef uint8_t TileTypeU8;

typedef struct {
    TileTypeU8 type;
    uint8_t hp;
    bool is_player;
} Tile;

typedef Tile Grid[N_ROWS][N_COLS];

typedef enum {
    FIRE,
    WATER,
    EARTH,
    AIR,
} Element;

typedef uint8_t ElementU8;

#define max(a,b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a > _b ? _a : _b; })

#define min(a,b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a < _b ? _a : _b; })

bool is_pos_eq(Position a, Position b);

bool is_in_bounds(Position pos);

Tile get_tile(const Grid grid, Position pos);

TileTypeU8 get_tile_type(const Grid grid, Position pos);

bool is_player_tile(const Grid grid, Position pos);

void set_tile(Grid grid, Position pos, TileTypeU8 type, uint8_t hp);

bool is_wall(const Grid grid, Position pos);

bool is_in_player_bounds(const Grid grid, Position pos);

void damage_tile(Grid grid, Position pos, uint8_t damage);

#endif
