#include "storage.h"

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

void save_data(StorageData *data) {
	SaveFileData(STORAGE_DATA_FILE, data, sizeof(StorageData));
}
