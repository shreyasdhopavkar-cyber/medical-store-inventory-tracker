extern "C"
{
    #include "validation.h"
}
#include <gtest/gtest.h>

/* ---- isValidId tests ---- */

TEST(ValidationTest, ValidId_PositiveNumber)
{
    EXPECT_EQ(isValidId(1), 1);
    EXPECT_EQ(isValidId(100), 1);
    EXPECT_EQ(isValidId(9999), 1);
}

TEST(ValidationTest, InvalidId_ZeroAndNegative)
{
    EXPECT_EQ(isValidId(0), 0);
    EXPECT_EQ(isValidId(-1), 0);
    EXPECT_EQ(isValidId(-500), 0);
}

/* ---- isValidName tests ---- */

TEST(ValidationTest, ValidName_NormalString)
{
    EXPECT_EQ(isValidName("Paracetamol"), 1);
    EXPECT_EQ(isValidName("Cough Syrup 100ml"), 1);
}

TEST(ValidationTest, InvalidName_EmptyAndNull)
{
    EXPECT_EQ(isValidName(""), 0);
    EXPECT_EQ(isValidName(NULL), 0);
}

/* ---- isValidPrice tests ---- */

TEST(ValidationTest, ValidPrice_PositiveValues)
{
    EXPECT_EQ(isValidPrice(10.5f), 1);
    EXPECT_EQ(isValidPrice(0.01f), 1);
    EXPECT_EQ(isValidPrice(999.99f), 1);
}

TEST(ValidationTest, InvalidPrice_ZeroAndNegative)
{
    EXPECT_EQ(isValidPrice(0.0f), 0);
    EXPECT_EQ(isValidPrice(-5.0f), 0);
}

/* ---- isValidQuantity tests ---- */

TEST(ValidationTest, ValidQuantity_ZeroAndPositive)
{
    EXPECT_EQ(isValidQuantity(0), 1);
    EXPECT_EQ(isValidQuantity(50), 1);
}

TEST(ValidationTest, InvalidQuantity_Negative)
{
    EXPECT_EQ(isValidQuantity(-1), 0);
    EXPECT_EQ(isValidQuantity(-100), 0);
}

/* ---- isValidDate tests ---- */

TEST(ValidationTest, ValidDate_NormalDates)
{
    EXPECT_EQ(isValidDate(15, 6, 2025), 1);
    EXPECT_EQ(isValidDate(1, 1, 2024), 1);
    EXPECT_EQ(isValidDate(31, 12, 2030), 1);
}

TEST(ValidationTest, InvalidDate_BadMonth)
{
    EXPECT_EQ(isValidDate(15, 13, 2025), 0);
    EXPECT_EQ(isValidDate(15, 0, 2025), 0);
}

TEST(ValidationTest, InvalidDate_BadDay)
{
    EXPECT_EQ(isValidDate(32, 1, 2025), 0);
    EXPECT_EQ(isValidDate(0, 6, 2025), 0);
}

TEST(ValidationTest, ValidDate_LeapYear29Feb)
{
    EXPECT_EQ(isValidDate(29, 2, 2024), 1);
}

TEST(ValidationTest, InvalidDate_NonLeapYear29Feb)
{
    EXPECT_EQ(isValidDate(29, 2, 2023), 0);
}

TEST(ValidationTest, BoundaryDate_30April)
{
    EXPECT_EQ(isValidDate(30, 4, 2025), 1);
    EXPECT_EQ(isValidDate(31, 4, 2025), 0);
}

/* ---- isValidCategory tests ---- */

TEST(ValidationTest, ValidCategory_WithinRange)
{
    EXPECT_EQ(isValidCategory(0), 1);
    EXPECT_EQ(isValidCategory(CATEGORY_COUNT - 1), 1);
}

TEST(ValidationTest, InvalidCategory_OutOfRange)
{
    EXPECT_EQ(isValidCategory(-1), 0);
    EXPECT_EQ(isValidCategory(CATEGORY_COUNT), 0);
    EXPECT_EQ(isValidCategory(99), 0);
}

/* ---- isExpired tests ---- */

TEST(ValidationTest, Expired_PastDate)
{
    Date_t expiry = {1, 1, 2023};
    Date_t today = {15, 6, 2025};
    EXPECT_EQ(isExpired(expiry, today), 1);
}

TEST(ValidationTest, NotExpired_FutureDate)
{
    Date_t expiry = {1, 12, 2028};
    Date_t today = {15, 6, 2025};
    EXPECT_EQ(isExpired(expiry, today), 0);
}

TEST(ValidationTest, NotExpired_SameDate)
{
    Date_t expiry = {15, 6, 2025};
    Date_t today = {15, 6, 2025};
    EXPECT_EQ(isExpired(expiry, today), 0);
}

/* ---- isValidMenuChoice tests ---- */

TEST(ValidationTest, ValidMenuChoice_InRange)
{
    EXPECT_EQ(isValidMenuChoice(0, 0, 9), 1);
    EXPECT_EQ(isValidMenuChoice(5, 0, 9), 1);
    EXPECT_EQ(isValidMenuChoice(9, 0, 9), 1);
}

TEST(ValidationTest, InvalidMenuChoice_OutOfRange)
{
    EXPECT_EQ(isValidMenuChoice(10, 0, 9), 0);
    EXPECT_EQ(isValidMenuChoice(-1, 0, 9), 0);
}

/* ---- daysUntilExpiry tests ---- */

TEST(ValidationTest, DaysUntilExpiry_FutureDate)
{
    Date_t expiry = {10, 7, 2025};
    Date_t today = {10, 6, 2025};
    int days = daysUntilExpiry(expiry, today);
    EXPECT_GT(days, 0);
}

TEST(ValidationTest, DaysUntilExpiry_PastDate)
{
    Date_t expiry = {10, 5, 2025};
    Date_t today = {10, 6, 2025};
    int days = daysUntilExpiry(expiry, today);
    EXPECT_LT(days, 0);
}

/* ---- isValidBatchNumber tests ---- */

TEST(ValidationTest, ValidBatch_NormalString)
{
    EXPECT_EQ(isValidBatchNumber("BT2024001"), 1);
}

TEST(ValidationTest, InvalidBatch_EmptyAndNull)
{
    EXPECT_EQ(isValidBatchNumber(""), 0);
    EXPECT_EQ(isValidBatchNumber(NULL), 0);
}
