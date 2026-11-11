# Detailed Design Document

## Medical Store Inventory and Expiry Tracker

| Field | Details |
|-------|---------|
| **Project Title** | Medical Store Inventory and Expiry Tracker |
| **Language** | C (with C++ unit tests) |
| **Platform** | Windows / Linux (Console Application) |
| **Version** | 1.0 |

---

## Table of Contents

1. [Introduction](#1-introduction)
2. [Software Architecture](#2-software-architecture)
3. [Module Design](#3-module-design)
4. [Data Design](#4-data-design)
5. [Function Design](#5-function-design)
6. [Input Validation Plan](#6-input-validation-plan)
7. [File Handling Design](#7-file-handling-design)
8. [Error Handling Strategy](#8-error-handling-strategy)
9. [Application Flow](#9-application-flow)
10. [Unit Test Plan](#10-unit-test-plan)

---

## 1. Introduction

### 1.1 Purpose

This document describes the detailed internal design of the Medical Store Inventory and Expiry Tracker system. It covers the architecture, data structures, module breakdowns, function-level design, error handling, and testing strategy. Basically everything someone would need to understand how the code works and why things were done a certain way.

### 1.2 Scope

The system lets a medical store manage its medicine inventory through a console menu. The main features are:
- Adding, deleting, and searching medicines
- Selling medicines and tracking stock
- Monitoring expiry dates and low stock situations
- Saving/loading data to a file so nothing is lost between runs
- Generating reports like total inventory value, category counts, etc.

### 1.3 Intended Audience

This document is meant for anyone evaluating or maintaining the project — faculty, fellow students, or future developers who might extend it.

---

## 2. Software Architecture

### 2.1 Architectural Style

The project follows a **modular procedural architecture**. The code is split into separate `.c` files, each handling a specific concern. This keeps things organized and makes it easier to test individual parts without worrying about the rest.

### 2.2 Module Overview

```
┌─────────────────────────────────────────────┐
│                  main.c                     │
│           (Menu Loop & User I/O)            │
└──────┬──────────┬──────────┬────────────────┘
       │          │          │
       ▼          ▼          ▼
 ┌──────────┐ ┌──────────┐ ┌──────────┐
 │medicine.c│ │ report.c │ │file_     │
 │          │ │          │ │handler.c │
 └─────┬────┘ └────┬─────┘ └──────────┘
       │           │
       ▼           ▼
 ┌─────────────────────────┐
 │     validation.c        │
 │  (Input Validation)     │
 └─────────────────────────┘
```

### 2.3 Module Dependencies

| Module | Depends On |
|--------|-----------|
| `main.c` | `medicine.c`, `report.c`, `file_handler.c`, `validation.c` |
| `medicine.c` | `validation.c` |
| `report.c` | `validation.c` (for expiry checks) |
| `file_handler.c` | (standalone — just reads/writes the file) |
| `validation.c` | (standalone — no dependencies on other modules) |

The idea is that `validation.c` sits at the bottom with no dependencies on anything else. `medicine.c` and `report.c` use validation functions, and `main.c` ties everything together.

### 2.4 File Organization

```
project/
├── src/
│   ├── main.c              # Entry point, menu loop
│   ├── medicine.c           # Core inventory operations
│   ├── validation.c         # All input validation
│   ├── file_handler.c       # File I/O (save/load)
│   └── report.c             # Display and reporting
├── include/
│   ├── medicine.h           # Medicine structs, function prototypes
│   ├── validation.h         # Validation function prototypes
│   ├── file_handler.h       # File I/O prototypes
│   └── report.h             # Report function prototypes
├── data/
│   └── medicines.dat        # Persistent data file
├── tests/
│   ├── test_medicine.cpp    # Medicine module tests
│   ├── test_validation.cpp  # Validation module tests
│   └── test_report.cpp      # Report module tests
├── docs/
│   └── detailed_design.md   # This document
├── Makefile
└── README.md
```

---

## 3. Module Design

### 3.1 main.c — Application Entry Point

**Responsibility:** Handles the main menu loop, takes user input, and calls the appropriate functions from other modules. Also handles loading data at startup and saving before exit.

**Key behavior:**
- On startup, calls `loadFromFile()` to restore previous data
- Displays a menu with options numbered 1–12 (or similar)
- Reads the user's choice and dispatches to the right function
- On exit, calls `saveToFile()` to persist everything

### 3.2 medicine.c — Core Inventory Operations

**Responsibility:** All the CRUD operations on the medicine array — adding, deleting, finding, selling, restocking, and updating prices.

**Internal data:** Works on an array of `Medicine_t` and a count variable that tracks how many medicines are stored. These are managed in `main.c` and passed by pointer.

**Key functions:** `addMedicine`, `deleteMedicineById`, `findMedicineById`, `findMedicineByName`, `sellMedicine`, `updateMedicinePrice`, `restockMedicine`, `getCategoryName`

### 3.3 validation.c — Input Validation

**Responsibility:** Validates every piece of user input before it gets used anywhere. This module is completely independent — it doesn't know about the inventory array or file handling, it just checks if values make sense.

**Key functions:** `isValidId`, `isValidName`, `isValidPrice`, `isValidQuantity`, `isValidDate`, `isValidCategory`, `isValidBatchNumber`, `isExpired`, `daysUntilExpiry`, `isValidMenuChoice`

### 3.4 file_handler.c — File I/O

**Responsibility:** Saving the entire medicine array to a file and loading it back. Uses a pipe-separated text format so the data file is human-readable if needed.

**Key functions:** `saveToFile`, `loadFromFile`

### 3.5 report.c — Reporting and Display

**Responsibility:** Displaying medicine records, generating lists of expiring or low-stock items, and computing summary statistics.

**Key functions:** `displayMedicine`, `displayAllMedicines`, `getExpiringMedicines`, `getLowStockMedicines`, `calculateTotalValue`, `countByCategory`, `printSummaryReport`

---

## 4. Data Design

### 4.1 Structures

#### 4.1.1 Date_t

Represents a calendar date. Used for medicine expiry dates.

```c
typedef struct {
    int day;
    int month;
    int year;
} Date_t;
```

| Field | Type | Range | Description |
|-------|------|-------|-------------|
| `day` | `int` | 1–31 | Day of the month |
| `month` | `int` | 1–12 | Month of the year |
| `year` | `int` | 2000+ (reasonable range) | Calendar year |

#### 4.1.2 Medicine_t

The main data structure. Each medicine in the inventory is stored as one of these.

```c
typedef struct {
    int id;
    char name[50];
    char batch_number[20];
    MedicineCategory_t category;
    float price;
    int quantity;
    int min_stock_level;
    Date_t expiry_date;
    char supplier[50];
} Medicine_t;
```

| Field | Type | Size/Range | Description |
|-------|------|-----------|-------------|
| `id` | `int` | > 0 | Unique identifier for the medicine |
| `name` | `char[]` | Max 50 chars | Name of the medicine (e.g., "Paracetamol") |
| `batch_number` | `char[]` | Max 20 chars | Manufacturer's batch number |
| `category` | `MedicineCategory_t` | 0–5 | Type of medicine (tablet, syrup, etc.) |
| `price` | `float` | > 0.0 | Price per unit in currency |
| `quantity` | `int` | >= 0 | Current stock quantity |
| `min_stock_level` | `int` | >= 0 | Threshold below which stock is "low" |
| `expiry_date` | `Date_t` | Valid date | When the medicine expires |
| `supplier` | `char[]` | Max 50 chars | Supplier/distributor name |

### 4.2 Enumerations

#### 4.2.1 MedicineCategory_t

Categories of medicine. `CATEGORY_COUNT` isn't a real category — it's just used to know how many categories there are (handy for loops and validation).

```c
typedef enum {
    CATEGORY_TABLET = 0,
    CATEGORY_CAPSULE,
    CATEGORY_SYRUP,
    CATEGORY_INJECTION,
    CATEGORY_OINTMENT,
    CATEGORY_DROPS,
    CATEGORY_COUNT        // Not a real category, just the count
} MedicineCategory_t;
```

| Value | Integer | Meaning |
|-------|---------|---------|
| `CATEGORY_TABLET` | 0 | Tablet form |
| `CATEGORY_CAPSULE` | 1 | Capsule form |
| `CATEGORY_SYRUP` | 2 | Liquid syrup |
| `CATEGORY_INJECTION` | 3 | Injectable |
| `CATEGORY_OINTMENT` | 4 | Topical ointment/cream |
| `CATEGORY_DROPS` | 5 | Eye/ear drops |
| `CATEGORY_COUNT` | 6 | Sentinel value (total number of categories) |

#### 4.2.2 Status_t

Return codes used throughout the project. Every function that can fail returns one of these instead of printing error messages directly. This way the caller can decide what to do.

```c
typedef enum {
    STATUS_SUCCESS = 0,
    STATUS_ERROR_INVALID_INPUT,
    STATUS_ERROR_NOT_FOUND,
    STATUS_ERROR_DUPLICATE,
    STATUS_ERROR_FULL,
    STATUS_ERROR_FILE,
    STATUS_ERROR_INSUFFICIENT_STOCK,
    STATUS_ERROR_NULL_POINTER
} Status_t;
```

| Value | Integer | When it's returned |
|-------|---------|-------------------|
| `STATUS_SUCCESS` | 0 | Operation completed successfully |
| `STATUS_ERROR_INVALID_INPUT` | 1 | A parameter failed validation |
| `STATUS_ERROR_NOT_FOUND` | 2 | Medicine with given ID/name doesn't exist |
| `STATUS_ERROR_DUPLICATE` | 3 | Trying to add a medicine with an ID that already exists |
| `STATUS_ERROR_FULL` | 4 | Inventory array is full (200 medicines) |
| `STATUS_ERROR_FILE` | 5 | File couldn't be opened, read, or written |
| `STATUS_ERROR_INSUFFICIENT_STOCK` | 6 | Trying to sell more than what's in stock |
| `STATUS_ERROR_NULL_POINTER` | 7 | A NULL pointer was passed where it shouldn't be |

### 4.3 Constants

```c
#define MAX_MEDICINES       200
#define MAX_NAME_LEN         50
#define MAX_BATCH_LEN        20
#define MAX_SUPPLIER_LEN     50
#define EXPIRY_WARNING_DAYS  30
#define DATA_FILE            "data/medicines.dat"
```

| Constant | Value | Purpose |
|----------|-------|---------|
| `MAX_MEDICINES` | 200 | Maximum number of medicines the system can hold |
| `MAX_NAME_LEN` | 50 | Max length of medicine name (including null terminator) |
| `MAX_BATCH_LEN` | 20 | Max length of batch number string |
| `MAX_SUPPLIER_LEN` | 50 | Max length of supplier name |
| `EXPIRY_WARNING_DAYS` | 30 | Days before expiry to flag a medicine as "expiring soon" |
| `DATA_FILE` | `"data/medicines.dat"` | Path to the data file for persistence |

### 4.4 Data Dictionary

This is a consolidated reference of all data items in the system.

| Data Item | Type | Source | Constraints | Used By |
|-----------|------|--------|-------------|---------|
| Medicine ID | `int` | User input | Must be > 0, unique | medicine.c, validation.c |
| Medicine Name | `char[50]` | User input | Non-empty, max 49 chars | medicine.c, validation.c |
| Batch Number | `char[20]` | User input | Non-empty, max 19 chars | medicine.c, validation.c |
| Category | `MedicineCategory_t` | User input | 0 to CATEGORY_COUNT-1 | medicine.c, validation.c |
| Price | `float` | User input | Must be > 0.0 | medicine.c, validation.c |
| Quantity | `int` | User input | Must be >= 0 | medicine.c, validation.c |
| Min Stock Level | `int` | User input | Must be >= 0 | medicine.c, report.c |
| Expiry Date | `Date_t` | User input | Must be a valid calendar date | validation.c, report.c |
| Supplier | `char[50]` | User input | Non-empty, max 49 chars | medicine.c |
| Medicine Array | `Medicine_t[]` | Runtime | Max 200 elements | All modules |
| Medicine Count | `int` | Runtime | 0 to MAX_MEDICINES | All modules |

---

## 5. Function Design

### 5.1 medicine.c Functions

---

#### `addMedicine`

```c
Status_t addMedicine(Medicine_t medicines[], int *count, Medicine_t newMedicine);
```

**Purpose:** Adds a new medicine record to the inventory array.

**Parameters:**

| Parameter | Type | Direction | Description |
|-----------|------|-----------|-------------|
| `medicines` | `Medicine_t[]` | IN/OUT | The inventory array |
| `count` | `int*` | IN/OUT | Pointer to current number of medicines |
| `newMedicine` | `Medicine_t` | IN | The medicine to add |

**Return Value:** `Status_t` — `STATUS_SUCCESS` on success.

**Preconditions:**
- `medicines` and `count` must not be NULL
- `*count` must be less than `MAX_MEDICINES`
- `newMedicine` fields should be validated before calling

**Postconditions:**
- Medicine is appended at index `*count`
- `*count` is incremented by 1

**Error Conditions:**

| Error | Condition |
|-------|-----------|
| `STATUS_ERROR_NULL_POINTER` | `medicines` or `count` is NULL |
| `STATUS_ERROR_FULL` | `*count >= MAX_MEDICINES` |
| `STATUS_ERROR_DUPLICATE` | A medicine with the same `id` already exists |
| `STATUS_ERROR_INVALID_INPUT` | `newMedicine` has invalid fields |

---

#### `deleteMedicineById`

```c
Status_t deleteMedicineById(Medicine_t medicines[], int *count, int id);
```

**Purpose:** Removes a medicine from the array by its ID. Shifts remaining elements to fill the gap.

**Parameters:**

| Parameter | Type | Direction | Description |
|-----------|------|-----------|-------------|
| `medicines` | `Medicine_t[]` | IN/OUT | The inventory array |
| `count` | `int*` | IN/OUT | Pointer to current count |
| `id` | `int` | IN | ID of medicine to delete |

**Return Value:** `Status_t`

**Preconditions:**
- `medicines` and `count` must not be NULL
- `id` must be valid (> 0)

**Postconditions:**
- Medicine is removed, array is compacted
- `*count` is decremented by 1

**Error Conditions:**

| Error | Condition |
|-------|-----------|
| `STATUS_ERROR_NULL_POINTER` | `medicines` or `count` is NULL |
| `STATUS_ERROR_NOT_FOUND` | No medicine has the given ID |
| `STATUS_ERROR_INVALID_INPUT` | `id` is not valid |

---

#### `findMedicineById`

```c
Medicine_t* findMedicineById(Medicine_t medicines[], int count, int id);
```

**Purpose:** Searches the array for a medicine with the specified ID. Returns a pointer to it if found, NULL otherwise.

**Parameters:**

| Parameter | Type | Direction | Description |
|-----------|------|-----------|-------------|
| `medicines` | `Medicine_t[]` | IN | The inventory array |
| `count` | `int` | IN | Current number of medicines |
| `id` | `int` | IN | ID to search for |

**Return Value:** Pointer to the matching `Medicine_t`, or `NULL` if not found.

**Preconditions:**
- `medicines` should not be NULL
- `id` should be > 0

**Postconditions:** Array is not modified.

---

#### `findMedicineByName`

```c
Medicine_t* findMedicineByName(Medicine_t medicines[], int count, const char *name);
```

**Purpose:** Searches the array for a medicine by name. The comparison should be case-insensitive ideally, or at least exact match.

**Parameters:**

| Parameter | Type | Direction | Description |
|-----------|------|-----------|-------------|
| `medicines` | `Medicine_t[]` | IN | The inventory array |
| `count` | `int` | IN | Current number of medicines |
| `name` | `const char*` | IN | Name to search for |

**Return Value:** Pointer to the matching `Medicine_t`, or `NULL` if not found.

**Preconditions:**
- `medicines` and `name` should not be NULL

**Postconditions:** Array is not modified.

---

#### `sellMedicine`

```c
Status_t sellMedicine(Medicine_t medicines[], int count, int id, int quantity);
```

**Purpose:** Reduces the stock of a medicine by the specified quantity (simulating a sale).

**Parameters:**

| Parameter | Type | Direction | Description |
|-----------|------|-----------|-------------|
| `medicines` | `Medicine_t[]` | IN/OUT | The inventory array |
| `count` | `int` | IN | Current count |
| `id` | `int` | IN | ID of medicine to sell |
| `quantity` | `int` | IN | Number of units to sell |

**Return Value:** `Status_t`

**Preconditions:**
- `medicines` must not be NULL
- `id` must be valid, `quantity` must be > 0
- Medicine must have sufficient stock

**Postconditions:**
- `medicine.quantity` is decreased by `quantity`

**Error Conditions:**

| Error | Condition |
|-------|-----------|
| `STATUS_ERROR_NULL_POINTER` | `medicines` is NULL |
| `STATUS_ERROR_NOT_FOUND` | No medicine with given ID |
| `STATUS_ERROR_INVALID_INPUT` | `quantity` <= 0 or `id` invalid |
| `STATUS_ERROR_INSUFFICIENT_STOCK` | Current stock < requested quantity |

---

#### `updateMedicinePrice`

```c
Status_t updateMedicinePrice(Medicine_t medicines[], int count, int id, float newPrice);
```

**Purpose:** Updates the price of a specific medicine.

**Parameters:**

| Parameter | Type | Direction | Description |
|-----------|------|-----------|-------------|
| `medicines` | `Medicine_t[]` | IN/OUT | The inventory array |
| `count` | `int` | IN | Current count |
| `id` | `int` | IN | ID of the medicine |
| `newPrice` | `float` | IN | New price value |

**Return Value:** `Status_t`

**Preconditions:**
- `medicines` not NULL
- `newPrice` > 0.0

**Postconditions:** The medicine's `price` field is updated.

**Error Conditions:**

| Error | Condition |
|-------|-----------|
| `STATUS_ERROR_NULL_POINTER` | `medicines` is NULL |
| `STATUS_ERROR_NOT_FOUND` | No medicine with given ID |
| `STATUS_ERROR_INVALID_INPUT` | `newPrice` <= 0 or `id` invalid |

---

#### `restockMedicine`

```c
Status_t restockMedicine(Medicine_t medicines[], int count, int id, int quantity);
```

**Purpose:** Increases the stock of a medicine (when new stock arrives from supplier).

**Parameters:**

| Parameter | Type | Direction | Description |
|-----------|------|-----------|-------------|
| `medicines` | `Medicine_t[]` | IN/OUT | The inventory array |
| `count` | `int` | IN | Current count |
| `id` | `int` | IN | ID of medicine to restock |
| `quantity` | `int` | IN | Number of units to add |

**Return Value:** `Status_t`

**Preconditions:**
- `medicines` not NULL
- `quantity` > 0

**Postconditions:** `medicine.quantity` is increased by `quantity`.

**Error Conditions:**

| Error | Condition |
|-------|-----------|
| `STATUS_ERROR_NULL_POINTER` | `medicines` is NULL |
| `STATUS_ERROR_NOT_FOUND` | No medicine with given ID |
| `STATUS_ERROR_INVALID_INPUT` | `quantity` <= 0 or `id` invalid |

---

#### `getCategoryName`

```c
const char* getCategoryName(MedicineCategory_t category);
```

**Purpose:** Converts a `MedicineCategory_t` enum value to its string representation (e.g., `CATEGORY_TABLET` → `"Tablet"`).

**Parameters:**

| Parameter | Type | Direction | Description |
|-----------|------|-----------|-------------|
| `category` | `MedicineCategory_t` | IN | The category enum value |

**Return Value:** A pointer to a string literal (e.g., `"Tablet"`, `"Capsule"`, etc.). Returns `"Unknown"` for invalid values.

**Preconditions:** None — handles invalid values gracefully.

**Postconditions:** No side effects.

---

### 5.2 validation.c Functions

---

#### `isValidId`

```c
int isValidId(int id);
```

**Purpose:** Checks if a medicine ID is valid (positive integer).

| Parameter | Type | Description |
|-----------|------|-------------|
| `id` | `int` | The ID to validate |

**Returns:** 1 if valid (id > 0), 0 otherwise.

---

#### `isValidName`

```c
int isValidName(const char *name);
```

**Purpose:** Checks if a name string is valid — not NULL, not empty, and within length limits.

| Parameter | Type | Description |
|-----------|------|-------------|
| `name` | `const char*` | The name string to validate |

**Returns:** 1 if valid, 0 otherwise.

---

#### `isValidPrice`

```c
int isValidPrice(float price);
```

**Purpose:** Checks if a price is a positive number.

| Parameter | Type | Description |
|-----------|------|-------------|
| `price` | `float` | The price to validate |

**Returns:** 1 if valid (price > 0.0), 0 otherwise.

---

#### `isValidQuantity`

```c
int isValidQuantity(int quantity);
```

**Purpose:** Checks if a quantity is non-negative.

| Parameter | Type | Description |
|-----------|------|-------------|
| `quantity` | `int` | The quantity to validate |

**Returns:** 1 if valid (quantity >= 0), 0 otherwise.

---

#### `isValidDate`

```c
int isValidDate(Date_t date);
```

**Purpose:** Checks if a date is a valid calendar date. Validates month range (1–12), day range for the given month, and handles leap years for February.

| Parameter | Type | Description |
|-----------|------|-------------|
| `date` | `Date_t` | The date to validate |

**Returns:** 1 if valid, 0 otherwise.

---

#### `isValidCategory`

```c
int isValidCategory(int category);
```

**Purpose:** Checks if a category value is within the valid enum range.

| Parameter | Type | Description |
|-----------|------|-------------|
| `category` | `int` | The category value to check |

**Returns:** 1 if valid (0 <= category < CATEGORY_COUNT), 0 otherwise.

---

#### `isValidBatchNumber`

```c
int isValidBatchNumber(const char *batch);
```

**Purpose:** Validates a batch number string — not NULL, not empty, within length limits.

| Parameter | Type | Description |
|-----------|------|-------------|
| `batch` | `const char*` | The batch number to validate |

**Returns:** 1 if valid, 0 otherwise.

---

#### `isExpired`

```c
int isExpired(Date_t expiryDate);
```

**Purpose:** Checks if a medicine has expired by comparing the expiry date against the current system date.

| Parameter | Type | Description |
|-----------|------|-------------|
| `expiryDate` | `Date_t` | The expiry date to check |

**Returns:** 1 if expired (expiry date is in the past), 0 otherwise.

---

#### `daysUntilExpiry`

```c
int daysUntilExpiry(Date_t expiryDate);
```

**Purpose:** Calculates the number of days remaining until the medicine expires. Returns a negative value if already expired.

| Parameter | Type | Description |
|-----------|------|-------------|
| `expiryDate` | `Date_t` | The expiry date |

**Returns:** Number of days until expiry. Negative means already expired.

**Implementation Note:** Uses `mktime()` and `difftime()` from `<time.h>` to do the date math. Converts both dates to `time_t` and computes the difference in seconds, then divides by 86400.

---

#### `isValidMenuChoice`

```c
int isValidMenuChoice(int choice, int min, int max);
```

**Purpose:** Validates that a menu choice falls within a given range.

| Parameter | Type | Description |
|-----------|------|-------------|
| `choice` | `int` | The user's menu selection |
| `min` | `int` | Minimum valid choice |
| `max` | `int` | Maximum valid choice |

**Returns:** 1 if `min <= choice <= max`, 0 otherwise.

---

### 5.3 file_handler.c Functions

---

#### `saveToFile`

```c
Status_t saveToFile(const char *filename, Medicine_t medicines[], int count);
```

**Purpose:** Writes the entire medicine array to a text file in pipe-separated format.

**Parameters:**

| Parameter | Type | Direction | Description |
|-----------|------|-----------|-------------|
| `filename` | `const char*` | IN | Path to the output file |
| `medicines` | `Medicine_t[]` | IN | Array of medicines to save |
| `count` | `int` | IN | Number of medicines |

**Return Value:** `Status_t`

**File Format Written:**
```
<count>
id|name|batch|category|price|qty|min_stock|dd/mm/yyyy|supplier
id|name|batch|category|price|qty|min_stock|dd/mm/yyyy|supplier
...
```

**Error Conditions:**

| Error | Condition |
|-------|-----------|
| `STATUS_ERROR_NULL_POINTER` | `filename` or `medicines` is NULL |
| `STATUS_ERROR_FILE` | File couldn't be opened for writing |

---

#### `loadFromFile`

```c
Status_t loadFromFile(const char *filename, Medicine_t medicines[], int *count);
```

**Purpose:** Reads medicine records from the data file and populates the array. The first line of the file is the count of records.

**Parameters:**

| Parameter | Type | Direction | Description |
|-----------|------|-----------|-------------|
| `filename` | `const char*` | IN | Path to the data file |
| `medicines` | `Medicine_t[]` | OUT | Array to populate |
| `count` | `int*` | OUT | Set to number of records loaded |

**Return Value:** `Status_t`

**Error Conditions:**

| Error | Condition |
|-------|-----------|
| `STATUS_ERROR_NULL_POINTER` | `filename`, `medicines`, or `count` is NULL |
| `STATUS_ERROR_FILE` | File doesn't exist or can't be opened |

**Note:** If the file doesn't exist (first run), this isn't necessarily a fatal error. The system can start with an empty inventory.

---

### 5.4 report.c Functions

---

#### `displayMedicine`

```c
void displayMedicine(const Medicine_t *medicine);
```

**Purpose:** Prints a single medicine's details in a formatted way to the console.

| Parameter | Type | Description |
|-----------|------|-------------|
| `medicine` | `const Medicine_t*` | Pointer to the medicine to display |

---

#### `displayAllMedicines`

```c
void displayAllMedicines(Medicine_t medicines[], int count);
```

**Purpose:** Displays all medicines in the inventory in a tabular format.

| Parameter | Type | Description |
|-----------|------|-------------|
| `medicines` | `Medicine_t[]` | The inventory array |
| `count` | `int` | Number of medicines |

---

#### `getExpiringMedicines`

```c
int getExpiringMedicines(Medicine_t medicines[], int count, Medicine_t result[], int *resultCount);
```

**Purpose:** Finds all medicines that will expire within `EXPIRY_WARNING_DAYS` (30 days). Copies them into the `result` array.

| Parameter | Type | Direction | Description |
|-----------|------|-----------|-------------|
| `medicines` | `Medicine_t[]` | IN | Source inventory |
| `count` | `int` | IN | Number of medicines |
| `result` | `Medicine_t[]` | OUT | Array to store expiring medicines |
| `resultCount` | `int*` | OUT | Number of expiring medicines found |

**Returns:** `STATUS_SUCCESS` or appropriate error code.

---

#### `getLowStockMedicines`

```c
int getLowStockMedicines(Medicine_t medicines[], int count, Medicine_t result[], int *resultCount);
```

**Purpose:** Finds all medicines where `quantity <= min_stock_level`. Copies them into the `result` array.

| Parameter | Type | Direction | Description |
|-----------|------|-----------|-------------|
| `medicines` | `Medicine_t[]` | IN | Source inventory |
| `count` | `int` | IN | Number of medicines |
| `result` | `Medicine_t[]` | OUT | Array to store low-stock medicines |
| `resultCount` | `int*` | OUT | Number of low-stock medicines found |

**Returns:** `STATUS_SUCCESS` or appropriate error code.

---

#### `calculateTotalValue`

```c
float calculateTotalValue(Medicine_t medicines[], int count);
```

**Purpose:** Calculates the total monetary value of all inventory (sum of `price × quantity` for each medicine).

| Parameter | Type | Description |
|-----------|------|-------------|
| `medicines` | `Medicine_t[]` | The inventory array |
| `count` | `int` | Number of medicines |

**Returns:** Total value as a float.

---

#### `countByCategory`

```c
void countByCategory(Medicine_t medicines[], int count, int categoryCounts[]);
```

**Purpose:** Counts how many medicines belong to each category. The `categoryCounts` array should have `CATEGORY_COUNT` elements.

| Parameter | Type | Direction | Description |
|-----------|------|-----------|-------------|
| `medicines` | `Medicine_t[]` | IN | The inventory array |
| `count` | `int` | IN | Number of medicines |
| `categoryCounts` | `int[]` | OUT | Array of size CATEGORY_COUNT, filled with counts |

---

#### `printSummaryReport`

```c
void printSummaryReport(Medicine_t medicines[], int count);
```

**Purpose:** Prints a full summary report including total medicines, total value, category breakdown, expiring medicines, and low stock alerts.

| Parameter | Type | Description |
|-----------|------|-------------|
| `medicines` | `Medicine_t[]` | The inventory array |
| `count` | `int` | Number of medicines |

---

## 6. Input Validation Plan

All user inputs are validated before being used. The validation functions in `validation.c` are used by both the business logic (medicine.c) and the UI layer (main.c). Here's the validation strategy for each field:

| Input Field | Validation Rule | Function Used | Error Response |
|-------------|----------------|---------------|----------------|
| Medicine ID | Must be a positive integer (> 0) | `isValidId()` | Reject and re-prompt |
| Medicine Name | Not empty, length < MAX_NAME_LEN | `isValidName()` | Reject and re-prompt |
| Batch Number | Not empty, length < MAX_BATCH_LEN | `isValidBatchNumber()` | Reject and re-prompt |
| Category | Integer between 0 and CATEGORY_COUNT-1 | `isValidCategory()` | Reject and re-prompt |
| Price | Positive float (> 0.0) | `isValidPrice()` | Reject and re-prompt |
| Quantity | Non-negative integer (>= 0) | `isValidQuantity()` | Reject and re-prompt |
| Min Stock Level | Non-negative integer (>= 0) | `isValidQuantity()` | Reject and re-prompt |
| Expiry Date | Valid calendar date, correct day/month ranges, leap year handling | `isValidDate()` | Reject and re-prompt |
| Supplier Name | Not empty, length < MAX_SUPPLIER_LEN | `isValidName()` | Reject and re-prompt |
| Menu Choice | Within the valid range of menu options | `isValidMenuChoice()` | Display error, show menu again |

### Validation Flow

```
User Input → main.c (read input)
                │
                ▼
        validation.c (validate)
                │
          ┌─────┴─────┐
          │            │
        Valid       Invalid
          │            │
          ▼            ▼
   Process input   Show error,
   (medicine.c)    re-prompt user
```

### Edge Cases Handled

- **NULL strings:** All string validators check for NULL before accessing the string
- **Empty strings:** Treated as invalid for names, batch numbers, suppliers
- **Negative numbers:** Rejected for ID, price, quantity
- **Invalid dates:** February 30th, month 13, day 0, etc. are all caught
- **Leap years:** February 29th is valid only in leap years
- **Buffer overflow:** String lengths are checked against max constants before copying

---

## 7. File Handling Design

### 7.1 File Format

The data file uses a human-readable pipe-separated text format. This was chosen over binary for easier debugging — you can just open the file in a text editor to see what's stored.

**Format:**
```
<number_of_records>
id|name|batch_number|category|price|quantity|min_stock_level|dd/mm/yyyy|supplier
```

**Example file contents:**
```
3
101|Paracetamol|B001|0|12.50|100|20|15/06/2025|MedSupply Co
102|Amoxicillin|B045|1|45.00|50|10|20/03/2025|PharmaDist
103|Cough Syrup|B112|2|85.00|30|5|10/12/2025|HealthCare Ltd
```

### 7.2 Save Operation

1. Open file in write mode (`"w"`)
2. Write the count on the first line
3. For each medicine, write all fields separated by `|`
4. Date is written as `dd/mm/yyyy`
5. Close the file
6. Return `STATUS_SUCCESS` or `STATUS_ERROR_FILE`

### 7.3 Load Operation

1. Try to open the file in read mode (`"r"`)
2. If file doesn't exist, set count to 0 and return (not necessarily an error on first run)
3. Read the count from the first line
4. For each record, parse the pipe-separated fields
5. Populate the `medicines[]` array
6. Close the file
7. Return `STATUS_SUCCESS` or `STATUS_ERROR_FILE`

### 7.4 File Safety Considerations

- The `data/` directory must exist before saving. The program should create it if it doesn't exist, or at least document that it needs to be there.
- If the file is corrupted or has malformed data, `loadFromFile` should handle it gracefully — skip bad records or report an error, but not crash.
- The file path is defined as a constant (`DATA_FILE`), so it's consistent everywhere.

---

## 8. Error Handling Strategy

### 8.1 Approach

The project uses a **return code based** error handling strategy. Functions return `Status_t` values to indicate success or the type of failure. The caller (usually `main.c`) checks the return value and displays an appropriate message to the user.

This approach was chosen because:
- C doesn't have exceptions, so return codes are the standard way
- It keeps error handling logic separate from business logic
- Makes unit testing straightforward — you can check return values

### 8.2 Error Handling Flow

```
Function Call → Returns Status_t
                    │
        ┌───────────┼────────────────┐
        │           │                │
   SUCCESS    INVALID_INPUT    NOT_FOUND    ... other errors
        │           │                │
        ▼           ▼                ▼
   Continue    Show "Invalid     Show "Medicine
   normally     input" msg       not found" msg
```

### 8.3 Error Codes and User Messages

| Status Code | User-Facing Message (example) |
|-------------|-------------------------------|
| `STATUS_SUCCESS` | "Operation completed successfully." |
| `STATUS_ERROR_INVALID_INPUT` | "Invalid input. Please check your values and try again." |
| `STATUS_ERROR_NOT_FOUND` | "Medicine not found. Please check the ID." |
| `STATUS_ERROR_DUPLICATE` | "A medicine with this ID already exists." |
| `STATUS_ERROR_FULL` | "Inventory is full. Cannot add more medicines." |
| `STATUS_ERROR_FILE` | "Error accessing data file. Changes may not be saved." |
| `STATUS_ERROR_INSUFFICIENT_STOCK` | "Not enough stock to complete this sale." |
| `STATUS_ERROR_NULL_POINTER` | (Internal error — shouldn't happen in normal use) |

### 8.4 NULL Pointer Protection

Every function that receives a pointer checks it for NULL at the very beginning. If a NULL pointer is detected, the function immediately returns `STATUS_ERROR_NULL_POINTER`. This prevents segfaults and makes the code more robust, which is also really useful during testing.

---

## 9. Application Flow

### 9.1 Startup Sequence

```
Program Start
     │
     ▼
Initialize medicines[] array and count = 0
     │
     ▼
Call loadFromFile(DATA_FILE, medicines, &count)
     │
     ├── File exists → Load data, set count
     │
     └── File doesn't exist → count stays 0 (first run)
     │
     ▼
Enter Main Menu Loop
```

### 9.2 Main Menu

The user sees a menu something like this:

```
========================================
  Medical Store Inventory Tracker
========================================
 1.  Add Medicine
 2.  Delete Medicine
 3.  Search by ID
 4.  Search by Name
 5.  Sell Medicine
 6.  Restock Medicine
 7.  Update Price
 8.  Display All Medicines
 9.  View Expiring Medicines
 10. View Low Stock Medicines
 11. Print Summary Report
 12. Save & Exit
========================================
 Enter your choice:
```

### 9.3 Typical Operation Flow — Adding a Medicine

```
User selects "Add Medicine"
        │
        ▼
Prompt for each field (ID, name, batch, category, price, qty, min stock, expiry, supplier)
        │
        ▼
Validate each field using validation.c functions
        │
        ├── Any field invalid → Show error, re-prompt or abort
        │
        └── All valid → Call addMedicine()
                            │
                            ├── SUCCESS → Display confirmation
                            │
                            └── ERROR (duplicate/full) → Display error message
```

### 9.4 Typical Operation Flow — Selling a Medicine

```
User selects "Sell Medicine"
        │
        ▼
Prompt for Medicine ID and Quantity
        │
        ▼
Validate ID and Quantity
        │
        ├── Invalid → Show error, re-prompt
        │
        └── Valid → Call sellMedicine()
                        │
                        ├── SUCCESS → Display sale confirmation
                        ├── NOT_FOUND → "Medicine not found"
                        └── INSUFFICIENT_STOCK → "Not enough stock"
```

### 9.5 Shutdown Sequence

```
User selects "Save & Exit"
        │
        ▼
Call saveToFile(DATA_FILE, medicines, count)
        │
        ├── SUCCESS → "Data saved successfully. Goodbye!"
        │
        └── ERROR → "Warning: Could not save data!"
        │
        ▼
Program exits
```

---

## 10. Unit Test Plan

### 10.1 Testing Framework

Unit tests are written in **C++** using the **Google Test** framework. This lets us write tests for the C functions by including the C headers with `extern "C"`. Each module has its own test file.

### 10.2 Test File Mapping

| Test File | Module Tested | Focus Area |
|-----------|---------------|------------|
| `test_validation.cpp` | `validation.c` | All input validation functions |
| `test_medicine.cpp` | `medicine.c` | Add, find, sell, delete, restock, update operations |
| `test_report.cpp` | `report.c` | Total value calc, category counts, low stock, expiring |

### 10.3 Test Case Table

| Test ID | Test File | Function Under Test | Test Description | Input / Setup | Expected Result |
|---------|-----------|-------------------|------------------|---------------|-----------------|
| UT-01 | test_validation.cpp | `isValidId` | Valid positive ID | `id = 101` | Returns 1 (true) |
| UT-02 | test_validation.cpp | `isValidId` | Zero ID (invalid) | `id = 0` | Returns 0 (false) |
| UT-03 | test_validation.cpp | `isValidId` | Negative ID (invalid) | `id = -5` | Returns 0 (false) |
| UT-04 | test_validation.cpp | `isValidName` | Valid name string | `name = "Paracetamol"` | Returns 1 |
| UT-05 | test_validation.cpp | `isValidName` | Empty string | `name = ""` | Returns 0 |
| UT-06 | test_validation.cpp | `isValidName` | NULL pointer | `name = NULL` | Returns 0 |
| UT-07 | test_validation.cpp | `isValidPrice` | Valid positive price | `price = 25.50` | Returns 1 |
| UT-08 | test_validation.cpp | `isValidPrice` | Zero price | `price = 0.0` | Returns 0 |
| UT-09 | test_validation.cpp | `isValidPrice` | Negative price | `price = -10.0` | Returns 0 |
| UT-10 | test_validation.cpp | `isValidDate` | Valid normal date | `{15, 6, 2025}` | Returns 1 |
| UT-11 | test_validation.cpp | `isValidDate` | Invalid month (13) | `{10, 13, 2025}` | Returns 0 |
| UT-12 | test_validation.cpp | `isValidDate` | Feb 29 on leap year | `{29, 2, 2024}` | Returns 1 |
| UT-13 | test_validation.cpp | `isValidDate` | Feb 29 on non-leap year | `{29, 2, 2025}` | Returns 0 |
| UT-14 | test_validation.cpp | `isValidCategory` | Valid category (TABLET) | `category = 0` | Returns 1 |
| UT-15 | test_validation.cpp | `isValidCategory` | Out-of-range category | `category = 10` | Returns 0 |
| UT-16 | test_validation.cpp | `isValidBatchNumber` | Valid batch number | `batch = "B001"` | Returns 1 |
| UT-17 | test_validation.cpp | `isValidBatchNumber` | Empty batch number | `batch = ""` | Returns 0 |
| UT-18 | test_validation.cpp | `isValidQuantity` | Valid quantity | `quantity = 50` | Returns 1 |
| UT-19 | test_validation.cpp | `isValidQuantity` | Negative quantity | `quantity = -1` | Returns 0 |
| UT-20 | test_validation.cpp | `isValidMenuChoice` | Choice within range | `choice=5, min=1, max=12` | Returns 1 |
| UT-21 | test_validation.cpp | `isValidMenuChoice` | Choice out of range | `choice=15, min=1, max=12` | Returns 0 |
| UT-22 | test_validation.cpp | `isExpired` | Future date (not expired) | Date far in future | Returns 0 |
| UT-23 | test_validation.cpp | `isExpired` | Past date (expired) | `{1, 1, 2020}` | Returns 1 |
| UT-24 | test_validation.cpp | `daysUntilExpiry` | Already expired date | `{1, 1, 2020}` | Returns negative value |
| UT-25 | test_medicine.cpp | `addMedicine` | Add to empty inventory | Valid medicine, count=0 | `STATUS_SUCCESS`, count becomes 1 |
| UT-26 | test_medicine.cpp | `addMedicine` | Add duplicate ID | Same ID as existing | `STATUS_ERROR_DUPLICATE` |
| UT-27 | test_medicine.cpp | `addMedicine` | Add when inventory full | count = MAX_MEDICINES | `STATUS_ERROR_FULL` |
| UT-28 | test_medicine.cpp | `addMedicine` | NULL pointer for array | `medicines = NULL` | `STATUS_ERROR_NULL_POINTER` |
| UT-29 | test_medicine.cpp | `findMedicineById` | Find existing medicine | ID that exists in array | Returns non-NULL pointer |
| UT-30 | test_medicine.cpp | `findMedicineById` | Find non-existing ID | ID not in array | Returns NULL |
| UT-31 | test_medicine.cpp | `sellMedicine` | Valid sale | Sufficient stock | `STATUS_SUCCESS`, quantity reduced |
| UT-32 | test_medicine.cpp | `sellMedicine` | Sell more than stock | quantity > current stock | `STATUS_ERROR_INSUFFICIENT_STOCK` |
| UT-33 | test_medicine.cpp | `sellMedicine` | Sell from non-existing ID | Invalid ID | `STATUS_ERROR_NOT_FOUND` |
| UT-34 | test_medicine.cpp | `deleteMedicineById` | Delete existing medicine | Valid ID in array | `STATUS_SUCCESS`, count decremented |
| UT-35 | test_medicine.cpp | `deleteMedicineById` | Delete non-existing ID | ID not in array | `STATUS_ERROR_NOT_FOUND` |
| UT-36 | test_medicine.cpp | `restockMedicine` | Valid restock | Valid ID, qty > 0 | `STATUS_SUCCESS`, quantity increased |
| UT-37 | test_medicine.cpp | `updateMedicinePrice` | Valid price update | Valid ID, price > 0 | `STATUS_SUCCESS`, price updated |
| UT-38 | test_report.cpp | `calculateTotalValue` | Total value calculation | Array with known prices/qtys | Returns correct sum of price×qty |
| UT-39 | test_report.cpp | `calculateTotalValue` | Empty inventory | count = 0 | Returns 0.0 |
| UT-40 | test_report.cpp | `countByCategory` | Count medicines per category | Mixed categories in array | Each category count matches expected |
| UT-41 | test_report.cpp | `getLowStockMedicines` | Find low stock items | Some items below min_stock | Correct items in result array |
| UT-42 | test_report.cpp | `getLowStockMedicines` | No low stock items | All items above min_stock | resultCount = 0 |
| UT-43 | test_report.cpp | `getExpiringMedicines` | Find expiring items | Some items expiring within 30 days | Correct items in result array |
| UT-44 | test_report.cpp | `getExpiringMedicines` | No expiring items | All items with dates far in future | resultCount = 0 |

### 10.4 Test Coverage Goals

- **validation.c:** Every function tested with valid input, boundary values, and invalid input. Special attention to date validation edge cases (leap years, month boundaries).
- **medicine.c:** Every function tested for success case, and each possible error return code tested at least once.
- **report.c:** Calculation functions tested with known inputs. Edge case of empty inventory tested.

### 10.5 How to Run Tests

```bash
# Build and run all tests
make test

# Or run individual test executables
./test_validation
./test_medicine
./test_report
```

---

## Appendix A: Glossary

| Term | Meaning |
|------|---------|
| CRUD | Create, Read, Update, Delete |
| Pipe-separated | Fields in a text record delimited by the `\|` character |
| Sentinel value | A special value indicating a boundary (like `CATEGORY_COUNT`) |
| min_stock_level | The threshold below which the medicine is considered "low stock" |
| Expiry Warning | Alert generated when medicine expires within 30 days |

---

*End of Detailed Design Document*
