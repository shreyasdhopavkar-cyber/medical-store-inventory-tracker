#ifndef VALIDATION_H
#define VALIDATION_H

#include "medicine.h"

/* Check if medicine ID is valid (positive number) */
int isValidId(int id);

/* Check if name is non-empty and within limit */
int isValidName(const char *name);

/* Check if price is positive */
int isValidPrice(float price);

/* Check if quantity is non-negative */
int isValidQuantity(int quantity);

/* Check if date values make sense */
int isValidDate(int day, int month, int year);

/* Check if category is within valid range */
int isValidCategory(int category);

/* Check if batch number is non-empty and within limit */
int isValidBatchNumber(const char *batch);

/* Check if medicine is past its expiry date */
int isExpired(Date_t expiryDate, Date_t currentDate);

/* Get approximate days remaining until expiry */
int daysUntilExpiry(Date_t expiryDate, Date_t currentDate);

/* Check if menu choice is within allowed range */
int isValidMenuChoice(int choice, int min, int max);

#endif
