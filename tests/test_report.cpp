extern "C"
{
    #include "medicine.h"
    #include "report.h"
    #include "validation.h"
}
#include <gtest/gtest.h>
#include <cstring>

class ReportTest : public ::testing::Test
{
protected:
    Medicine_t inventory[MAX_MEDICINES];
    int count;

    void SetUp() override
    {
        count = 0;
        memset(inventory, 0, sizeof(inventory));
    }

    Medicine_t makeMed(int id, const char *name, float price, int qty,
                       int minStock, int expDay, int expMonth, int expYear,
                       MedicineCategory_t cat)
    {
        Medicine_t med;
        memset(&med, 0, sizeof(med));
        med.id = id;
        strncpy(med.name, name, MAX_NAME_LEN - 1);
        strncpy(med.batch_number, "B001", MAX_BATCH_LEN - 1);
        med.category = cat;
        med.price = price;
        med.quantity = qty;
        med.min_stock_level = minStock;
        med.expiry_date.day = expDay;
        med.expiry_date.month = expMonth;
        med.expiry_date.year = expYear;
        strncpy(med.supplier, "Supplier", MAX_SUPPLIER_LEN - 1);
        return med;
    }
};

/* ---- calculateTotalValue tests ---- */

TEST_F(ReportTest, TotalValue_SingleItem)
{
    Medicine_t med = makeMed(1, "Paracetamol", 25.0f, 10, 5,
                             1, 12, 2027, CATEGORY_TABLET);
    addMedicine(inventory, &count, med);
    EXPECT_FLOAT_EQ(calculateTotalValue(inventory, count), 250.0f);
}

TEST_F(ReportTest, TotalValue_MultipleItems)
{
    addMedicine(inventory, &count,
                makeMed(1, "Paracetamol", 25.0f, 10, 5, 1, 12, 2027, CATEGORY_TABLET));
    addMedicine(inventory, &count,
                makeMed(2, "Cough Syrup", 80.0f, 5, 3, 1, 6, 2026, CATEGORY_SYRUP));

    /* 25*10 + 80*5 = 250 + 400 = 650 */
    EXPECT_FLOAT_EQ(calculateTotalValue(inventory, count), 650.0f);
}

TEST_F(ReportTest, TotalValue_EmptyInventory)
{
    EXPECT_FLOAT_EQ(calculateTotalValue(inventory, 0), 0.0f);
}

TEST_F(ReportTest, TotalValue_NullInventory)
{
    EXPECT_FLOAT_EQ(calculateTotalValue(NULL, 5), 0.0f);
}

/* ---- countByCategory tests ---- */

TEST_F(ReportTest, CategoryCount_MultipleSameCategory)
{
    addMedicine(inventory, &count,
                makeMed(1, "Paracetamol", 25.0f, 10, 5, 1, 12, 2027, CATEGORY_TABLET));
    addMedicine(inventory, &count,
                makeMed(2, "Aspirin", 15.0f, 20, 5, 1, 12, 2027, CATEGORY_TABLET));
    addMedicine(inventory, &count,
                makeMed(3, "Cough Syrup", 80.0f, 5, 3, 1, 6, 2026, CATEGORY_SYRUP));

    EXPECT_EQ(countByCategory(inventory, count, CATEGORY_TABLET), 2);
    EXPECT_EQ(countByCategory(inventory, count, CATEGORY_SYRUP), 1);
}

TEST_F(ReportTest, CategoryCount_NoneInCategory)
{
    addMedicine(inventory, &count,
                makeMed(1, "Paracetamol", 25.0f, 10, 5, 1, 12, 2027, CATEGORY_TABLET));

    EXPECT_EQ(countByCategory(inventory, count, CATEGORY_INJECTION), 0);
}

TEST_F(ReportTest, CategoryCount_EmptyInventory)
{
    EXPECT_EQ(countByCategory(inventory, 0, CATEGORY_TABLET), 0);
}

/* ---- getLowStockMedicines tests ---- */

TEST_F(ReportTest, LowStock_BelowMinimum)
{
    /* quantity=3 but min_stock=10, so this is low */
    addMedicine(inventory, &count,
                makeMed(1, "Paracetamol", 25.0f, 3, 10, 1, 12, 2027, CATEGORY_TABLET));

    int results[MAX_MEDICINES];
    int lowCount = getLowStockMedicines(inventory, count, results, MAX_MEDICINES);
    EXPECT_EQ(lowCount, 1);
    EXPECT_EQ(results[0], 0);
}

TEST_F(ReportTest, LowStock_AboveMinimum)
{
    /* quantity=50 and min_stock=10, this is fine */
    addMedicine(inventory, &count,
                makeMed(1, "Paracetamol", 25.0f, 50, 10, 1, 12, 2027, CATEGORY_TABLET));

    int results[MAX_MEDICINES];
    int lowCount = getLowStockMedicines(inventory, count, results, MAX_MEDICINES);
    EXPECT_EQ(lowCount, 0);
}

TEST_F(ReportTest, LowStock_ExactlyAtMinimum)
{
    /* quantity=10 and min_stock=10, not low (10 is not < 10) */
    addMedicine(inventory, &count,
                makeMed(1, "Paracetamol", 25.0f, 10, 10, 1, 12, 2027, CATEGORY_TABLET));

    int results[MAX_MEDICINES];
    int lowCount = getLowStockMedicines(inventory, count, results, MAX_MEDICINES);
    EXPECT_EQ(lowCount, 0);
}

/* ---- getExpiringMedicines tests ---- */

TEST_F(ReportTest, Expiring_WithinWarningPeriod)
{
    /* medicine expiring on 15/6/2025, today is 10/6/2025 => ~5 days left */
    addMedicine(inventory, &count,
                makeMed(1, "OldMed", 25.0f, 10, 5, 15, 6, 2025, CATEGORY_TABLET));

    Date_t today = {10, 6, 2025};
    int results[MAX_MEDICINES];
    int expCount = getExpiringMedicines(inventory, count, today, 30,
                                        results, MAX_MEDICINES);
    EXPECT_EQ(expCount, 1);
}

TEST_F(ReportTest, Expiring_FarFutureExpiry)
{
    /* medicine expiring in 2030, well outside 30-day window */
    addMedicine(inventory, &count,
                makeMed(1, "NewMed", 25.0f, 10, 5, 1, 12, 2030, CATEGORY_TABLET));

    Date_t today = {10, 6, 2025};
    int results[MAX_MEDICINES];
    int expCount = getExpiringMedicines(inventory, count, today, 30,
                                        results, MAX_MEDICINES);
    EXPECT_EQ(expCount, 0);
}

TEST_F(ReportTest, Expiring_AlreadyExpired)
{
    /* expired medicine - date is in the past */
    addMedicine(inventory, &count,
                makeMed(1, "ExpiredMed", 25.0f, 10, 5, 1, 1, 2024, CATEGORY_TABLET));

    Date_t today = {10, 6, 2025};
    int results[MAX_MEDICINES];
    int expCount = getExpiringMedicines(inventory, count, today, 30,
                                        results, MAX_MEDICINES);
    EXPECT_EQ(expCount, 1);  /* expired = within warning */
}

TEST_F(ReportTest, Expiring_EmptyInventory)
{
    Date_t today = {10, 6, 2025};
    int results[MAX_MEDICINES];
    int expCount = getExpiringMedicines(inventory, 0, today, 30,
                                        results, MAX_MEDICINES);
    EXPECT_EQ(expCount, 0);
}

/* ---- getCategoryName tests ---- */

TEST_F(ReportTest, CategoryName_Tablet)
{
    EXPECT_STREQ(getCategoryName(CATEGORY_TABLET), "Tablet");
}

TEST_F(ReportTest, CategoryName_Syrup)
{
    EXPECT_STREQ(getCategoryName(CATEGORY_SYRUP), "Syrup");
}

TEST_F(ReportTest, CategoryName_InvalidValue)
{
    EXPECT_STREQ(getCategoryName((MedicineCategory_t)99), "Unknown");
}
