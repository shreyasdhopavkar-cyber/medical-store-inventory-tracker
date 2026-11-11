#ifndef MEDICINE_H
#define MEDICINE_H

#define MAX_MEDICINES       200
#define MAX_NAME_LEN        50
#define MAX_BATCH_LEN       20
#define MAX_SUPPLIER_LEN    50

/* Medicine categories */
typedef enum
{
    CATEGORY_TABLET = 0,
    CATEGORY_CAPSULE,
    CATEGORY_SYRUP,
    CATEGORY_INJECTION,
    CATEGORY_OINTMENT,
    CATEGORY_DROPS,
    CATEGORY_COUNT
} MedicineCategory_t;

/* Simple date structure */
typedef struct
{
    int day;
    int month;
    int year;
} Date_t;

/* Main medicine record */
typedef struct
{
    int id;
    char name[MAX_NAME_LEN];
    char batch_number[MAX_BATCH_LEN];
    MedicineCategory_t category;
    float price;
    int quantity;
    int min_stock_level;
    Date_t expiry_date;
    char supplier[MAX_SUPPLIER_LEN];
} Medicine_t;

/* Status codes used across modules */
typedef enum
{
    STATUS_SUCCESS = 0,
    STATUS_ERROR_INVALID_INPUT,
    STATUS_ERROR_NOT_FOUND,
    STATUS_ERROR_DUPLICATE,
    STATUS_ERROR_FULL,
    STATUS_ERROR_FILE,
    STATUS_ERROR_INSUFFICIENT_STOCK,
    STATUS_ERROR_NULL_POINTER
} Status_t;

/* Add a new medicine to inventory array */
Status_t addMedicine(Medicine_t inventory[], int *count, Medicine_t newMed);

/* Remove medicine by ID */
Status_t deleteMedicineById(Medicine_t inventory[], int *count, int id);

/* Find medicine index by ID, returns -1 if not found */
int findMedicineById(const Medicine_t inventory[], int count, int id);

/* Find medicine index by name, returns -1 if not found */
int findMedicineByName(const Medicine_t inventory[], int count, const char *name);

/* Sell given quantity of a medicine */
Status_t sellMedicine(Medicine_t inventory[], int count, int id, int qty);

/* Update price of a medicine */
Status_t updateMedicinePrice(Medicine_t inventory[], int count, int id, float newPrice);

/* Add stock to existing medicine */
Status_t restockMedicine(Medicine_t inventory[], int count, int id, int addQty);

/* Get readable category name */
const char* getCategoryName(MedicineCategory_t category);

#endif
