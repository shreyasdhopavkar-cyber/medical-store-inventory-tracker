extern "C"
{
    #include "medicine.h"
    #include "validation.h"
}
#include <gtest/gtest.h>
#include <cstring>

/* test fixture to set up a fresh inventory for each test */
class MedicineTest : public ::testing::Test
{
protected:
    Medicine_t inventory[MAX_MEDICINES];
    int count;

    void SetUp() override
    {
        count = 0;
        memset(inventory, 0, sizeof(inventory));
    }

    /* helper to quickly create a medicine record */
    Medicine_t makeMed(int id, const char *name, float price, int qty)
    {
        Medicine_t med;
        memset(&med, 0, sizeof(med));
        med.id = id;
        strncpy(med.name, name, MAX_NAME_LEN - 1);
        strncpy(med.batch_number, "B001", MAX_BATCH_LEN - 1);
        med.category = CATEGORY_TABLET;
        med.price = price;
        med.quantity = qty;
        med.min_stock_level = 10;
        med.expiry_date.day = 1;
        med.expiry_date.month = 12;
        med.expiry_date.year = 2027;
        strncpy(med.supplier, "TestSupplier", MAX_SUPPLIER_LEN - 1);
        return med;
    }
};

/* ---- addMedicine tests ---- */

TEST_F(MedicineTest, AddMedicine_Success)
{
    Medicine_t med = makeMed(1, "Paracetamol", 25.0f, 100);
    EXPECT_EQ(addMedicine(inventory, &count, med), STATUS_SUCCESS);
    EXPECT_EQ(count, 1);
}

TEST_F(MedicineTest, AddMedicine_DuplicateId)
{
    Medicine_t med1 = makeMed(1, "Paracetamol", 25.0f, 100);
    Medicine_t med2 = makeMed(1, "Aspirin", 30.0f, 50);

    addMedicine(inventory, &count, med1);
    EXPECT_EQ(addMedicine(inventory, &count, med2), STATUS_ERROR_DUPLICATE);
    EXPECT_EQ(count, 1);
}

TEST_F(MedicineTest, AddMedicine_NullInventory)
{
    Medicine_t med = makeMed(1, "Paracetamol", 25.0f, 100);
    EXPECT_EQ(addMedicine(NULL, &count, med), STATUS_ERROR_NULL_POINTER);
}

TEST_F(MedicineTest, AddMedicine_NullCount)
{
    Medicine_t med = makeMed(1, "Paracetamol", 25.0f, 100);
    EXPECT_EQ(addMedicine(inventory, NULL, med), STATUS_ERROR_NULL_POINTER);
}

/* ---- findMedicineById tests ---- */

TEST_F(MedicineTest, FindById_ExistingMedicine)
{
    Medicine_t med = makeMed(42, "Ibuprofen", 35.0f, 60);
    addMedicine(inventory, &count, med);
    EXPECT_EQ(findMedicineById(inventory, count, 42), 0);
}

TEST_F(MedicineTest, FindById_NonExistentId)
{
    Medicine_t med = makeMed(1, "Paracetamol", 25.0f, 100);
    addMedicine(inventory, &count, med);
    EXPECT_EQ(findMedicineById(inventory, count, 99), -1);
}

TEST_F(MedicineTest, FindById_EmptyInventory)
{
    EXPECT_EQ(findMedicineById(inventory, 0, 1), -1);
}

/* ---- findMedicineByName tests ---- */

TEST_F(MedicineTest, FindByName_ExistingName)
{
    Medicine_t med = makeMed(1, "Crocin", 20.0f, 80);
    addMedicine(inventory, &count, med);
    EXPECT_EQ(findMedicineByName(inventory, count, "Crocin"), 0);
}

TEST_F(MedicineTest, FindByName_WrongName)
{
    Medicine_t med = makeMed(1, "Crocin", 20.0f, 80);
    addMedicine(inventory, &count, med);
    EXPECT_EQ(findMedicineByName(inventory, count, "Aspirin"), -1);
}

TEST_F(MedicineTest, FindByName_NullName)
{
    EXPECT_EQ(findMedicineByName(inventory, count, NULL), -1);
}

/* ---- sellMedicine tests ---- */

TEST_F(MedicineTest, SellMedicine_NormalSale)
{
    Medicine_t med = makeMed(1, "Paracetamol", 25.0f, 100);
    addMedicine(inventory, &count, med);
    EXPECT_EQ(sellMedicine(inventory, count, 1, 10), STATUS_SUCCESS);
    EXPECT_EQ(inventory[0].quantity, 90);
}

TEST_F(MedicineTest, SellMedicine_ExactStock)
{
    Medicine_t med = makeMed(1, "Paracetamol", 25.0f, 10);
    addMedicine(inventory, &count, med);
    EXPECT_EQ(sellMedicine(inventory, count, 1, 10), STATUS_SUCCESS);
    EXPECT_EQ(inventory[0].quantity, 0);
}

TEST_F(MedicineTest, SellMedicine_InsufficientStock)
{
    Medicine_t med = makeMed(1, "Paracetamol", 25.0f, 5);
    addMedicine(inventory, &count, med);
    EXPECT_EQ(sellMedicine(inventory, count, 1, 10), STATUS_ERROR_INSUFFICIENT_STOCK);
    EXPECT_EQ(inventory[0].quantity, 5);  /* quantity should not change */
}

TEST_F(MedicineTest, SellMedicine_ZeroQuantity)
{
    Medicine_t med = makeMed(1, "Paracetamol", 25.0f, 50);
    addMedicine(inventory, &count, med);
    EXPECT_EQ(sellMedicine(inventory, count, 1, 0), STATUS_ERROR_INVALID_INPUT);
}

TEST_F(MedicineTest, SellMedicine_MedicineNotFound)
{
    EXPECT_EQ(sellMedicine(inventory, count, 99, 5), STATUS_ERROR_NOT_FOUND);
}

/* ---- deleteMedicineById tests ---- */

TEST_F(MedicineTest, Delete_Success)
{
    Medicine_t med = makeMed(1, "Paracetamol", 25.0f, 100);
    addMedicine(inventory, &count, med);
    EXPECT_EQ(deleteMedicineById(inventory, &count, 1), STATUS_SUCCESS);
    EXPECT_EQ(count, 0);
}

TEST_F(MedicineTest, Delete_NotFound)
{
    EXPECT_EQ(deleteMedicineById(inventory, &count, 99), STATUS_ERROR_NOT_FOUND);
}

TEST_F(MedicineTest, Delete_MiddleElement)
{
    addMedicine(inventory, &count, makeMed(1, "Med A", 10.0f, 10));
    addMedicine(inventory, &count, makeMed(2, "Med B", 20.0f, 20));
    addMedicine(inventory, &count, makeMed(3, "Med C", 30.0f, 30));

    EXPECT_EQ(deleteMedicineById(inventory, &count, 2), STATUS_SUCCESS);
    EXPECT_EQ(count, 2);
    /* Med C should now be at index 1 */
    EXPECT_EQ(inventory[1].id, 3);
}

/* ---- restockMedicine tests ---- */

TEST_F(MedicineTest, Restock_Success)
{
    Medicine_t med = makeMed(1, "Paracetamol", 25.0f, 50);
    addMedicine(inventory, &count, med);
    EXPECT_EQ(restockMedicine(inventory, count, 1, 30), STATUS_SUCCESS);
    EXPECT_EQ(inventory[0].quantity, 80);
}

TEST_F(MedicineTest, Restock_NotFound)
{
    EXPECT_EQ(restockMedicine(inventory, count, 99, 10), STATUS_ERROR_NOT_FOUND);
}

/* ---- updateMedicinePrice tests ---- */

TEST_F(MedicineTest, UpdatePrice_Success)
{
    Medicine_t med = makeMed(1, "Paracetamol", 25.0f, 100);
    addMedicine(inventory, &count, med);
    EXPECT_EQ(updateMedicinePrice(inventory, count, 1, 30.0f), STATUS_SUCCESS);
    EXPECT_FLOAT_EQ(inventory[0].price, 30.0f);
}

TEST_F(MedicineTest, UpdatePrice_NegativePrice)
{
    Medicine_t med = makeMed(1, "Paracetamol", 25.0f, 100);
    addMedicine(inventory, &count, med);
    EXPECT_EQ(updateMedicinePrice(inventory, count, 1, -10.0f),
              STATUS_ERROR_INVALID_INPUT);
}

TEST_F(MedicineTest, UpdatePrice_NotFound)
{
    EXPECT_EQ(updateMedicinePrice(inventory, count, 99, 50.0f),
              STATUS_ERROR_NOT_FOUND);
}

/* ---- getCategoryName tests ---- */

TEST_F(MedicineTest, CategoryName_AllValid)
{
    EXPECT_STREQ(getCategoryName(CATEGORY_TABLET), "Tablet");
    EXPECT_STREQ(getCategoryName(CATEGORY_CAPSULE), "Capsule");
    EXPECT_STREQ(getCategoryName(CATEGORY_SYRUP), "Syrup");
    EXPECT_STREQ(getCategoryName(CATEGORY_INJECTION), "Injection");
    EXPECT_STREQ(getCategoryName(CATEGORY_OINTMENT), "Ointment");
    EXPECT_STREQ(getCategoryName(CATEGORY_DROPS), "Drops");
}

TEST_F(MedicineTest, CategoryName_Invalid)
{
    EXPECT_STREQ(getCategoryName((MedicineCategory_t)99), "Unknown");
}
