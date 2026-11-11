#include <stdio.h>
#include <string.h>
#include <time.h>
#include "medicine.h"
#include "validation.h"
#include "file_handler.h"
#include "report.h"

#define MENU_ADD        1
#define MENU_SELL       2
#define MENU_SEARCH     3
#define MENU_VIEW_ALL   4
#define MENU_RESTOCK    5
#define MENU_DELETE     6
#define MENU_UPDATE     7
#define MENU_REPORT     8
#define MENU_SAVE       9
#define MENU_EXIT       0

/* get today's date from system clock */
static Date_t getCurrentDate(void)
{
    Date_t today;
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    today.day = tm_info->tm_mday;
    today.month = tm_info->tm_mon + 1;
    today.year = tm_info->tm_year + 1900;

    return today;
}

static void printMenu(void)
{
    printf("\n===== MEDICAL STORE INVENTORY SYSTEM =====\n");
    printf("  1. Add Medicine\n");
    printf("  2. Sell Medicine\n");
    printf("  3. Search Medicine\n");
    printf("  4. View All Medicines\n");
    printf("  5. Restock Medicine\n");
    printf("  6. Delete Medicine\n");
    printf("  7. Update Medicine Price\n");
    printf("  8. Generate Report\n");
    printf("  9. Save Data\n");
    printf("  0. Exit\n");
    printf("==========================================\n");
    printf("Enter your choice: ");
}

/* flush leftover characters from stdin */
static void clearInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        /* discard */
    }
}

static void handleAddMedicine(Medicine_t inventory[], int *count)
{
    Medicine_t med;
    int catChoice;

    printf("\n--- Add New Medicine ---\n");

    printf("Enter Medicine ID: ");
    if (scanf("%d", &med.id) != 1)
    {
        printf("Error: Invalid ID input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (!isValidId(med.id))
    {
        printf("Error: ID must be a positive number.\n");
        return;
    }

    printf("Enter Medicine Name: ");
    if (fgets(med.name, MAX_NAME_LEN, stdin) == NULL)
    {
        printf("Error: Could not read name.\n");
        return;
    }
    med.name[strcspn(med.name, "\n")] = '\0';

    if (!isValidName(med.name))
    {
        printf("Error: Name cannot be empty.\n");
        return;
    }

    printf("Enter Batch Number: ");
    if (fgets(med.batch_number, MAX_BATCH_LEN, stdin) == NULL)
    {
        printf("Error: Could not read batch number.\n");
        return;
    }
    med.batch_number[strcspn(med.batch_number, "\n")] = '\0';

    if (!isValidBatchNumber(med.batch_number))
    {
        printf("Error: Batch number cannot be empty.\n");
        return;
    }

    printf("Select Category:\n");
    printf("  0 - Tablet\n  1 - Capsule\n  2 - Syrup\n");
    printf("  3 - Injection\n  4 - Ointment\n  5 - Drops\n");
    printf("Enter choice: ");
    if (scanf("%d", &catChoice) != 1)
    {
        printf("Error: Invalid category input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (!isValidCategory(catChoice))
    {
        printf("Error: Invalid category. Pick 0-5.\n");
        return;
    }
    med.category = (MedicineCategory_t)catChoice;

    printf("Enter Price (Rs.): ");
    if (scanf("%f", &med.price) != 1)
    {
        printf("Error: Invalid price input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (!isValidPrice(med.price))
    {
        printf("Error: Price must be positive.\n");
        return;
    }

    printf("Enter Quantity: ");
    if (scanf("%d", &med.quantity) != 1)
    {
        printf("Error: Invalid quantity input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (!isValidQuantity(med.quantity))
    {
        printf("Error: Quantity cannot be negative.\n");
        return;
    }

    printf("Enter Minimum Stock Level: ");
    if (scanf("%d", &med.min_stock_level) != 1)
    {
        printf("Error: Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (med.min_stock_level < 0)
    {
        printf("Error: Min stock level cannot be negative.\n");
        return;
    }

    printf("Enter Expiry Date (DD MM YYYY): ");
    if (scanf("%d %d %d", &med.expiry_date.day, &med.expiry_date.month,
              &med.expiry_date.year) != 3)
    {
        printf("Error: Invalid date input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (!isValidDate(med.expiry_date.day, med.expiry_date.month,
                     med.expiry_date.year))
    {
        printf("Error: Invalid date.\n");
        return;
    }

    printf("Enter Supplier Name: ");
    if (fgets(med.supplier, MAX_SUPPLIER_LEN, stdin) == NULL)
    {
        printf("Error: Could not read supplier.\n");
        return;
    }
    med.supplier[strcspn(med.supplier, "\n")] = '\0';

    Status_t status = addMedicine(inventory, count, med);

    switch (status)
    {
        case STATUS_SUCCESS:
            printf("Medicine added successfully! Total items: %d\n", *count);
            break;
        case STATUS_ERROR_DUPLICATE:
            printf("Error: Medicine with ID %d already exists.\n", med.id);
            break;
        case STATUS_ERROR_FULL:
            printf("Error: Inventory is full (max %d).\n", MAX_MEDICINES);
            break;
        default:
            printf("Error: Could not add medicine.\n");
            break;
    }
}

static void handleSellMedicine(Medicine_t inventory[], int count)
{
    int id;
    int qty;

    printf("\n--- Sell Medicine ---\n");
    printf("Enter Medicine ID: ");
    if (scanf("%d", &id) != 1)
    {
        printf("Error: Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int idx = findMedicineById(inventory, count, id);
    if (idx == -1)
    {
        printf("Error: Medicine with ID %d not found.\n", id);
        return;
    }

    printf("Current stock of '%s': %d\n", inventory[idx].name,
           inventory[idx].quantity);

    printf("Enter Quantity to Sell: ");
    if (scanf("%d", &qty) != 1)
    {
        printf("Error: Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    Status_t status = sellMedicine(inventory, count, id, qty);

    switch (status)
    {
        case STATUS_SUCCESS:
            printf("Sale successful! Remaining stock: %d\n",
                   inventory[idx].quantity);
            printf("Total bill: Rs. %.2f\n", inventory[idx].price * qty);
            break;
        case STATUS_ERROR_INSUFFICIENT_STOCK:
            printf("Error: Not enough stock. Available: %d\n",
                   inventory[idx].quantity);
            break;
        case STATUS_ERROR_INVALID_INPUT:
            printf("Error: Invalid quantity.\n");
            break;
        default:
            printf("Error: Sale failed.\n");
            break;
    }
}

static void handleSearchMedicine(const Medicine_t inventory[], int count)
{
    int searchChoice;

    printf("\n--- Search Medicine ---\n");
    printf("  1. Search by ID\n");
    printf("  2. Search by Name\n");
    printf("Enter choice: ");
    if (scanf("%d", &searchChoice) != 1)
    {
        printf("Error: Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int idx = -1;

    if (searchChoice == 1)
    {
        int id;
        printf("Enter Medicine ID: ");
        if (scanf("%d", &id) != 1)
        {
            printf("Error: Invalid input.\n");
            clearInputBuffer();
            return;
        }
        clearInputBuffer();
        idx = findMedicineById(inventory, count, id);
    }
    else if (searchChoice == 2)
    {
        char name[MAX_NAME_LEN];
        printf("Enter Medicine Name: ");
        if (fgets(name, MAX_NAME_LEN, stdin) == NULL)
        {
            printf("Error: Could not read name.\n");
            return;
        }
        name[strcspn(name, "\n")] = '\0';
        idx = findMedicineByName(inventory, count, name);
    }
    else
    {
        printf("Error: Pick 1 or 2.\n");
        return;
    }

    if (idx == -1)
    {
        printf("Medicine not found.\n");
    }
    else
    {
        displayMedicine(&inventory[idx]);
    }
}

static void handleRestock(Medicine_t inventory[], int count)
{
    int id;
    int qty;

    printf("\n--- Restock Medicine ---\n");
    printf("Enter Medicine ID: ");
    if (scanf("%d", &id) != 1)
    {
        printf("Error: Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int idx = findMedicineById(inventory, count, id);
    if (idx == -1)
    {
        printf("Error: Medicine with ID %d not found.\n", id);
        return;
    }

    printf("Current stock of '%s': %d\n", inventory[idx].name,
           inventory[idx].quantity);

    printf("Enter Quantity to Add: ");
    if (scanf("%d", &qty) != 1)
    {
        printf("Error: Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    Status_t status = restockMedicine(inventory, count, id, qty);
    if (status == STATUS_SUCCESS)
    {
        printf("Restocked! New quantity: %d\n", inventory[idx].quantity);
    }
    else
    {
        printf("Error: Could not restock.\n");
    }
}

static void handleDelete(Medicine_t inventory[], int *count)
{
    int id;
    int confirm;

    printf("\n--- Delete Medicine ---\n");
    printf("Enter Medicine ID to Delete: ");
    if (scanf("%d", &id) != 1)
    {
        printf("Error: Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int idx = findMedicineById(inventory, *count, id);
    if (idx == -1)
    {
        printf("Error: Medicine with ID %d not found.\n", id);
        return;
    }

    printf("Are you sure you want to delete '%s'? (1=Yes, 0=No): ",
           inventory[idx].name);
    if (scanf("%d", &confirm) != 1 || confirm != 1)
    {
        printf("Deletion cancelled.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    Status_t status = deleteMedicineById(inventory, count, id);
    if (status == STATUS_SUCCESS)
    {
        printf("Medicine deleted. Total items: %d\n", *count);
    }
    else
    {
        printf("Error: Could not delete.\n");
    }
}

static void handleUpdatePrice(Medicine_t inventory[], int count)
{
    int id;
    float newPrice;

    printf("\n--- Update Medicine Price ---\n");
    printf("Enter Medicine ID: ");
    if (scanf("%d", &id) != 1)
    {
        printf("Error: Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int idx = findMedicineById(inventory, count, id);
    if (idx == -1)
    {
        printf("Error: Medicine with ID %d not found.\n", id);
        return;
    }

    printf("Current price of '%s': Rs. %.2f\n", inventory[idx].name,
           inventory[idx].price);

    printf("Enter New Price (Rs.): ");
    if (scanf("%f", &newPrice) != 1)
    {
        printf("Error: Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    Status_t status = updateMedicinePrice(inventory, count, id, newPrice);
    if (status == STATUS_SUCCESS)
    {
        printf("Price updated to Rs. %.2f\n", newPrice);
    }
    else if (status == STATUS_ERROR_INVALID_INPUT)
    {
        printf("Error: Price must be positive.\n");
    }
    else
    {
        printf("Error: Could not update price.\n");
    }
}

int main(void)
{
    Medicine_t inventory[MAX_MEDICINES];
    int count = 0;
    int choice;
    int running = 1;

    printf("Loading inventory data...\n");
    Status_t loadStatus = loadFromFile(inventory, &count, DATA_FILE);
    if (loadStatus == STATUS_SUCCESS)
    {
        printf("Loaded %d medicine(s) from file.\n", count);
    }
    else
    {
        printf("Warning: Could not load data file. Starting fresh.\n");
    }

    Date_t today = getCurrentDate();
    printf("Today's date: %02d/%02d/%d\n", today.day, today.month, today.year);

    while (running)
    {
        printMenu();

        if (scanf("%d", &choice) != 1)
        {
            printf("Error: Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice)
        {
            case MENU_ADD:
                handleAddMedicine(inventory, &count);
                break;

            case MENU_SELL:
                handleSellMedicine(inventory, count);
                break;

            case MENU_SEARCH:
                handleSearchMedicine(inventory, count);
                break;

            case MENU_VIEW_ALL:
                displayAllMedicines(inventory, count);
                break;

            case MENU_RESTOCK:
                handleRestock(inventory, count);
                break;

            case MENU_DELETE:
                handleDelete(inventory, &count);
                break;

            case MENU_UPDATE:
                handleUpdatePrice(inventory, count);
                break;

            case MENU_REPORT:
                today = getCurrentDate();
                printSummaryReport(inventory, count, today);
                break;

            case MENU_SAVE:
                if (saveToFile(inventory, count, DATA_FILE) == STATUS_SUCCESS)
                {
                    printf("Data saved successfully.\n");
                }
                else
                {
                    printf("Error: Could not save data.\n");
                }
                break;

            case MENU_EXIT:
                printf("Saving data before exit...\n");
                saveToFile(inventory, count, DATA_FILE);
                printf("Goodbye!\n");
                running = 0;
                break;

            default:
                printf("Error: Invalid choice. Enter 0-9.\n");
                break;
        }
    }

    return 0;
}
