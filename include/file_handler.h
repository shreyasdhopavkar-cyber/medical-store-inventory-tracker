#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "medicine.h"

#define DATA_FILE "data/medicines.dat"

/* Save all inventory records to file */
Status_t saveToFile(const Medicine_t inventory[], int count, const char *filename);

/* Load inventory records from file */
Status_t loadFromFile(Medicine_t inventory[], int *count, const char *filename);

#endif
