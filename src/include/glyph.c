#include "glyph.h"

void generate_glyph_chunk(GlyphSpawner *spawner) {
    spawner->chunk[0] = rand() % 12;
    spawner->chunk[1] = spawner->chunk[0];

    spawner->chunk[2] = rand() % 12;
    spawner->chunk[3] = spawner->chunk[2];

    spawner->chunk[4] = rand() % 12;
    spawner->chunk[5] = rand() % 12;

    for (size_t i = 0; i < 5; ++i) {
        size_t j = i + rand() / (RAND_MAX / (6 - i) + 1);
        SignU8 t = spawner->chunk[j];
        spawner->chunk[j] = spawner->chunk[i];
        spawner->chunk[i] = t;
    }
}
