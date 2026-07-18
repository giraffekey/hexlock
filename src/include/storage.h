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

/**
 * Loads data file and parses it into a struct
 * 
 * @return The storage data
 */
StorageData load_data();

/**
 * Saves data to the data file
 * 
 * @param data The storage data
 */
void save_data(StorageData *data);

#endif
