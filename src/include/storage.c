#include "storage.h"

/**
 * Loads data file and parses it into a struct
 * 
 * @return The storage data
 */
StorageData load_data() {
	StorageData data = {0};

	int size = sizeof(StorageData);
	uint8_t *buf = LoadFileData(STORAGE_DATA_FILE, &size);
	if (buf != NULL) {
		memcpy(&data, buf, sizeof(StorageData));
		UnloadFileData(buf);
	}

	return data;
}

/**
 * Saves data to the data file
 * 
 * @param data The storage data
 */
void save_data(StorageData *data) {
	SaveFileData(STORAGE_DATA_FILE, data, sizeof(StorageData));
}
