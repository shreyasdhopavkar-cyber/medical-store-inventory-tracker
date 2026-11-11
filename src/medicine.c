#include <stdio.h>
#include <string.h>
#include "medicine.h"
#include "validation.h"

Status_t addMedicine(Medicine_t inventory[], int *count, Medicine_t newMed)
{
    if (inventory == NULL || count == NULL)
    {
        return STATUS_ERROR_NULL_POINTER;
    }

    if (*count >= MAX_MEDICINES)
    {
        return STATUS_ERROR_FULL;
    }

    /* check for duplicate ID */
    if (findMedicineById(inventory, *count, newMed.id) != -1)
    {
        return STATUS_ERROR_DUPLICATE;
    }

    /* basic field validation */
    if (!isValidId(newMed.id) || !isValidName(newMed.name) ||
        !isValidPrice(newMed.price) || !isValidQuantity(newMed.quantity))
    {
        return STATUS_ERROR_INVALID_INPUT;
    }

    inventory[*count] = newMed;
    (*count)++;

    return STATUS_SUCCESS;
}

Status_t deleteMedicineById(Medicine_t inventory[], int *count, int id)
{
    if (inventory == NULL || count == NULL)
    {
        return STATUS_ERROR_NULL_POINTER;
    }

    int index = findMedicineById(inventory, *count, id);
    if (index == -1)
    {
        return STATUS_ERROR_NOT_FOUND;
    }

    /* shift elements left to fill the gap */
    for (int i = index; i < *count - 1; i++)
    {
        inventory[i] = inventory[i + 1];
    }
    (*count)--;

    return STATUS_SUCCESS;
}

int findMedicineById(const Medicine_t inventory[], int count, int id)
{
    if (inventory == NULL || count <= 0)
    {
        return -1;
    }

    for (int i = 0; i < count; i++)
    {
        if (inventory[i].id == id)
        {
            return i;
        }
    }

    return -1;
}

int findMedicineByName(const Medicine_t inventory[], int count, const char *name)
{
    if (inventory == NULL || name == NULL || count <= 0)
    {
        return -1;
    }

    for (int i = 0; i < count; i++)
    {
        if (strcmp(inventory[i].name, name) == 0)
        {
            return i;
        }
    }

    return -1;
}

Status_t sellMedicine(Medicine_t inventory[], int count, int id, int qty)
{
    if (inventory == NULL)
    {
        return STATUS_ERROR_NULL_POINTER;
    }

    if (!isValidQuantity(qty) || qty == 0)
    {
        return STATUS_ERROR_INVALID_INPUT;
    }

    int index = findMedicineById(inventory, count, id);
    if (index == -1)
    {
        return STATUS_ERROR_NOT_FOUND;
    }

    if (inventory[index].quantity < qty)
    {
        return STATUS_ERROR_INSUFFICIENT_STOCK;
    }

    inventory[index].quantity -= qty;

    return STATUS_SUCCESS;
}

Status_t updateMedicinePrice(Medicine_t inventory[], int count, int id, float newPrice)
{
    if (inventory == NULL)
    {
        return STATUS_ERROR_NULL_POINTER;
    }

    if (!isValidPrice(newPrice))
    {
        return STATUS_ERROR_INVALID_INPUT;
    }

    int index = findMedicineById(inventory, count, id);
    if (index == -1)
    {
        return STATUS_ERROR_NOT_FOUND;
    }

    inventory[index].price = newPrice;

    return STATUS_SUCCESS;
}

Status_t restockMedicine(Medicine_t inventory[], int count, int id, int addQty)
{
    if (inventory == NULL)
    {
        return STATUS_ERROR_NULL_POINTER;
    }

    if (!isValidQuantity(addQty) || addQty == 0)
    {
        return STATUS_ERROR_INVALID_INPUT;
    }

    int index = findMedicineById(inventory, count, id);
    if (index == -1)
    {
        return STATUS_ERROR_NOT_FOUND;
    }

    inventory[index].quantity += addQty;

    return STATUS_SUCCESS;
}

const char* getCategoryName(MedicineCategory_t category)
{
    switch (category)
    {
        case CATEGORY_TABLET:    return "Tablet";
        case CATEGORY_CAPSULE:   return "Capsule";
        case CATEGORY_SYRUP:     return "Syrup";
        case CATEGORY_INJECTION: return "Injection";
        case CATEGORY_OINTMENT:  return "Ointment";
        case CATEGORY_DROPS:     return "Drops";
        default:                 return "Unknown";
    }
}
