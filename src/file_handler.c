#include <stdio.h>
#include <string.h>
#include "file_handler.h"

Status_t saveToFile(const Medicine_t inventory[], int count, const char *filename)
{
    if (inventory == NULL || filename == NULL)
    {
        return STATUS_ERROR_NULL_POINTER;
    }

    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        return STATUS_ERROR_FILE;
    }

    /* write total count on the first line */
    fprintf(fp, "%d\n", count);

    /* write each medicine as pipe-separated values */
    for (int i = 0; i < count; i++)
    {
        fprintf(fp, "%d|%s|%s|%d|%.2f|%d|%d|%d/%d/%d|%s\n",
                inventory[i].id,
                inventory[i].name,
                inventory[i].batch_number,
                inventory[i].category,
                inventory[i].price,
                inventory[i].quantity,
                inventory[i].min_stock_level,
                inventory[i].expiry_date.day,
                inventory[i].expiry_date.month,
                inventory[i].expiry_date.year,
                inventory[i].supplier);
    }

    fclose(fp);
    return STATUS_SUCCESS;
}

Status_t loadFromFile(Medicine_t inventory[], int *count, const char *filename)
{
    if (inventory == NULL || count == NULL || filename == NULL)
    {
        return STATUS_ERROR_NULL_POINTER;
    }

    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        /* file might not exist on first run, thats okay */
        *count = 0;
        return STATUS_SUCCESS;
    }

    int totalRecords = 0;
    if (fscanf(fp, "%d\n", &totalRecords) != 1)
    {
        fclose(fp);
        *count = 0;
        return STATUS_ERROR_FILE;
    }

    if (totalRecords < 0 || totalRecords > MAX_MEDICINES)
    {
        fclose(fp);
        *count = 0;
        return STATUS_ERROR_FILE;
    }

    int loaded = 0;
    for (int i = 0; i < totalRecords; i++)
    {
        int catVal = 0;
        int result = fscanf(fp, "%d|%49[^|]|%19[^|]|%d|%f|%d|%d|%d/%d/%d|%49[^\n]\n",
                            &inventory[i].id,
                            inventory[i].name,
                            inventory[i].batch_number,
                            &catVal,
                            &inventory[i].price,
                            &inventory[i].quantity,
                            &inventory[i].min_stock_level,
                            &inventory[i].expiry_date.day,
                            &inventory[i].expiry_date.month,
                            &inventory[i].expiry_date.year,
                            inventory[i].supplier);

        if (result != 11)
        {
            break;
        }

        inventory[i].category = (MedicineCategory_t)catVal;
        loaded++;
    }

    *count = loaded;
    fclose(fp);
    return STATUS_SUCCESS;
}
