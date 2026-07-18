#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "raylib.h"

static const char *STORAGE_DATA_FILE = "storage.data";

typedef struct {
	uint16_t high_score;
} StorageData;

StorageData load_data();

void save_data(StorageData *data);

#endif
