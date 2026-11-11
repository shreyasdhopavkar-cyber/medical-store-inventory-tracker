#ifndef REPORT_H
#define REPORT_H

#include "medicine.h"

#define EXPIRY_WARNING_DAYS 30

/* Print details of one medicine */
void displayMedicine(const Medicine_t *med);

/* Print all medicines in a table format */
void displayAllMedicines(const Medicine_t inventory[], int count);

/* Find medicines expiring within given days, store indices in results[] */
int getExpiringMedicines(const Medicine_t inventory[], int count,
                         Date_t currentDate, int withinDays,
                         int results[], int maxResults);

/* Find medicines below their minimum stock, store indices in results[] */
int getLowStockMedicines(const Medicine_t inventory[], int count,
                         int results[], int maxResults);

/* Calculate total value of all inventory (price * quantity) */
float calculateTotalValue(const Medicine_t inventory[], int count);

/* Count how many medicines belong to a given category */
int countByCategory(const Medicine_t inventory[], int count,
                    MedicineCategory_t category);

/* Print a full summary report with alerts */
void printSummaryReport(const Medicine_t inventory[], int count,
                        Date_t currentDate);

#endif
