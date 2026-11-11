#include <string.h>
#include "validation.h"

int isValidId(int id)
{
    return (id > 0);
}

int isValidName(const char *name)
{
    if (name == NULL)
    {
        return 0;
    }

    int len = (int)strlen(name);
    return (len > 0 && len < MAX_NAME_LEN);
}

int isValidPrice(float price)
{
    return (price > 0.0f);
}

int isValidQuantity(int quantity)
{
    return (quantity >= 0);
}

int isValidDate(int day, int month, int year)
{
    if (year < 2020 || year > 2035)
    {
        return 0;
    }

    if (month < 1 || month > 12)
    {
        return 0;
    }

    if (day < 1 || day > 31)
    {
        return 0;
    }

    /* days in each month */
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    /* leap year adjustment for february */
    if (month == 2)
    {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        {
            daysInMonth[2] = 29;
        }
    }

    return (day <= daysInMonth[month]);
}

int isValidCategory(int category)
{
    return (category >= 0 && category < CATEGORY_COUNT);
}

int isValidBatchNumber(const char *batch)
{
    if (batch == NULL)
    {
        return 0;
    }

    int len = (int)strlen(batch);
    return (len > 0 && len < MAX_BATCH_LEN);
}

int isExpired(Date_t expiryDate, Date_t currentDate)
{
    /* compare year first, then month, then day */
    if (expiryDate.year < currentDate.year)
    {
        return 1;
    }

    if (expiryDate.year == currentDate.year &&
        expiryDate.month < currentDate.month)
    {
        return 1;
    }

    if (expiryDate.year == currentDate.year &&
        expiryDate.month == currentDate.month &&
        expiryDate.day < currentDate.day)
    {
        return 1;
    }

    return 0;
}

int daysUntilExpiry(Date_t expiryDate, Date_t currentDate)
{
    /* approximate - using 365 days/year and 30 days/month */
    int expiryTotal = expiryDate.year * 365 + expiryDate.month * 30 + expiryDate.day;
    int currentTotal = currentDate.year * 365 + currentDate.month * 30 + currentDate.day;

    return (expiryTotal - currentTotal);
}

int isValidMenuChoice(int choice, int min, int max)
{
    return (choice >= min && choice <= max);
}
