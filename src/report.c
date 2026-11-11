#include <stdio.h>
#include "report.h"
#include "validation.h"

void displayMedicine(const Medicine_t *med)
{
    if (med == NULL)
    {
        printf("Error: No medicine data to display.\n");
        return;
    }

    printf("--------------------------------------\n");
    printf("  ID          : %d\n", med->id);
    printf("  Name        : %s\n", med->name);
    printf("  Batch       : %s\n", med->batch_number);
    printf("  Category    : %s\n", getCategoryName(med->category));
    printf("  Price       : Rs. %.2f\n", med->price);
    printf("  Quantity    : %d\n", med->quantity);
    printf("  Min Stock   : %d\n", med->min_stock_level);
    printf("  Expiry Date : %02d/%02d/%d\n",
           med->expiry_date.day, med->expiry_date.month, med->expiry_date.year);
    printf("  Supplier    : %s\n", med->supplier);
    printf("--------------------------------------\n");
}

void displayAllMedicines(const Medicine_t inventory[], int count)
{
    if (inventory == NULL || count <= 0)
    {
        printf("No medicines in inventory.\n");
        return;
    }

    printf("\n===== COMPLETE INVENTORY (%d items) =====\n", count);
    printf("%-5s %-20s %-12s %-10s %-10s %-5s %-12s\n",
           "ID", "Name", "Batch", "Category", "Price", "Qty", "Expiry");
    printf("------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("%-5d %-20s %-12s %-10s Rs.%-7.2f %-5d %02d/%02d/%d\n",
               inventory[i].id,
               inventory[i].name,
               inventory[i].batch_number,
               getCategoryName(inventory[i].category),
               inventory[i].price,
               inventory[i].quantity,
               inventory[i].expiry_date.day,
               inventory[i].expiry_date.month,
               inventory[i].expiry_date.year);
    }
    printf("========================================================================\n");
}

int getExpiringMedicines(const Medicine_t inventory[], int count,
                         Date_t currentDate, int withinDays,
                         int results[], int maxResults)
{
    if (inventory == NULL || results == NULL || count <= 0)
    {
        return 0;
    }

    int found = 0;
    for (int i = 0; i < count && found < maxResults; i++)
    {
        int daysLeft = daysUntilExpiry(inventory[i].expiry_date, currentDate);
        if (daysLeft <= withinDays)
        {
            results[found] = i;
            found++;
        }
    }

    return found;
}

int getLowStockMedicines(const Medicine_t inventory[], int count,
                         int results[], int maxResults)
{
    if (inventory == NULL || results == NULL || count <= 0)
    {
        return 0;
    }

    int found = 0;
    for (int i = 0; i < count && found < maxResults; i++)
    {
        if (inventory[i].quantity < inventory[i].min_stock_level)
        {
            results[found] = i;
            found++;
        }
    }

    return found;
}

float calculateTotalValue(const Medicine_t inventory[], int count)
{
    if (inventory == NULL || count <= 0)
    {
        return 0.0f;
    }

    float total = 0.0f;
    for (int i = 0; i < count; i++)
    {
        total += inventory[i].price * (float)inventory[i].quantity;
    }

    return total;
}

int countByCategory(const Medicine_t inventory[], int count,
                    MedicineCategory_t category)
{
    if (inventory == NULL || count <= 0)
    {
        return 0;
    }

    int result = 0;
    for (int i = 0; i < count; i++)
    {
        if (inventory[i].category == category)
        {
            result++;
        }
    }

    return result;
}

void printSummaryReport(const Medicine_t inventory[], int count,
                        Date_t currentDate)
{
    if (inventory == NULL || count <= 0)
    {
        printf("No medicines in inventory to generate report.\n");
        return;
    }

    printf("\n========== INVENTORY SUMMARY REPORT ==========\n\n");
    printf("Total medicines       : %d\n", count);
    printf("Total inventory value : Rs. %.2f\n\n", calculateTotalValue(inventory, count));

    /* category breakdown */
    printf("--- Category Breakdown ---\n");
    for (int c = 0; c < CATEGORY_COUNT; c++)
    {
        int catCount = countByCategory(inventory, count, (MedicineCategory_t)c);
        if (catCount > 0)
        {
            printf("  %-12s : %d\n", getCategoryName((MedicineCategory_t)c), catCount);
        }
    }

    /* low stock warnings */
    int lowStockIdx[MAX_MEDICINES];
    int lowCount = getLowStockMedicines(inventory, count, lowStockIdx, MAX_MEDICINES);

    printf("\n--- Low Stock Alert (%d items) ---\n", lowCount);
    if (lowCount == 0)
    {
        printf("  All items are sufficiently stocked.\n");
    }
    else
    {
        for (int i = 0; i < lowCount; i++)
        {
            int idx = lowStockIdx[i];
            printf("  [!] %s (ID: %d) - Stock: %d, Min Required: %d\n",
                   inventory[idx].name, inventory[idx].id,
                   inventory[idx].quantity, inventory[idx].min_stock_level);
        }
    }

    /* expiry warnings */
    int expiringIdx[MAX_MEDICINES];
    int expCount = getExpiringMedicines(inventory, count, currentDate,
                                        EXPIRY_WARNING_DAYS, expiringIdx, MAX_MEDICINES);

    printf("\n--- Expiry Alert (within %d days) (%d items) ---\n",
           EXPIRY_WARNING_DAYS, expCount);

    if (expCount == 0)
    {
        printf("  No medicines expiring soon.\n");
    }
    else
    {
        for (int i = 0; i < expCount; i++)
        {
            int idx = expiringIdx[i];
            int daysLeft = daysUntilExpiry(inventory[idx].expiry_date, currentDate);

            if (daysLeft < 0)
            {
                printf("  [EXPIRED] %s (ID: %d) - Expired %d days ago\n",
                       inventory[idx].name, inventory[idx].id, -daysLeft);
            }
            else
            {
                printf("  [WARNING] %s (ID: %d) - Expires in %d days\n",
                       inventory[idx].name, inventory[idx].id, daysLeft);
            }
        }
    }

    printf("\n================================================\n");
}
