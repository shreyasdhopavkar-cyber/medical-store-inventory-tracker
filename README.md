# Medical Store Inventory and Expiry Tracker

A command-line application built in C for managing medicine inventory in small pharmacy shops. It helps store owners track stock levels, monitor expiry dates, and generate useful reports — all from a simple text-based menu.

> **Course:** C Programming Mini Project  
> **Domain:** Healthcare  
> **Team Members:**  
> - [Your Name] — [Your Roll Number]  
> - [Your Name] — [Your Roll Number]  
> - [Your Name] — [Your Roll Number]  

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Architecture](#architecture)
- [C Concepts Used](#c-concepts-used)
- [Prerequisites](#prerequisites)
- [Building and Running](#building-and-running)
- [Sample Input / Output](#sample-input--output)
- [Data File Format](#data-file-format)
- [Coding Standards](#coding-standards)
- [Unit Testing Summary](#unit-testing-summary)
- [Error Handling Summary](#error-handling-summary)
- [Known Limitations](#known-limitations)
- [Future Enhancements](#future-enhancements)
- [License](#license)

---

## Overview

Managing medicines manually is error-prone — expired drugs can slip through, stock can run out without warning, and keeping track of hundreds of items on paper is just not practical. This project tries to solve that for small medical shops by giving them a simple tool that runs right in the terminal.

The program lets you add medicines, sell them (with automatic stock deduction), search by ID or name, restock, update prices, delete entries, and generate a full inventory report. It also warns you about medicines that are about to expire (within 30 days) and items that have fallen below their minimum stock level.

All data is persisted in a pipe-separated text file (`data/medicines.dat`), so nothing is lost between sessions.

---

## Features

| Feature | Description |
|---|---|
| **Add Medicine** | Add a new medicine with all its details (name, batch, category, price, quantity, etc.) |
| **Sell Medicine** | Sell a quantity of a medicine — stock is automatically reduced |
| **Search by ID** | Look up a specific medicine using its unique ID |
| **Search by Name** | Find medicines by name (partial match supported) |
| **View All** | Display the entire inventory in a formatted table |
| **Restock** | Increase the quantity of an existing medicine |
| **Delete Medicine** | Remove a medicine entry from the inventory |
| **Update Price** | Change the price of a medicine |
| **Generate Report** | Summary report with total value, category breakdown, low stock list, and expiry alerts |
| **Save & Exit** | Persist all changes to the data file before quitting |
| **Expiry Alerts** | Warns about medicines expiring within the next 30 days |
| **Low Stock Alerts** | Flags items whose quantity has dropped below their minimum stock level |

---

## Project Structure

```
medical-store-inventory-tracker/
├── src/
│   ├── main.c              # Entry point — menu display and user input loop
│   ├── medicine.c           # Core business logic (add, sell, search, delete, etc.)
│   ├── validation.c         # Input validation functions
│   ├── file_handler.c       # Reading from and writing to data files
│   └── report.c             # Report generation and display utilities
├── include/
│   ├── medicine.h           # Medicine struct, enums, and function declarations
│   ├── validation.h         # Validation function declarations
│   ├── file_handler.h       # File I/O function declarations
│   └── report.h             # Report function declarations
├── tests/
│   ├── test_validation.cpp  # Unit tests for input validation
│   ├── test_medicine.cpp    # Unit tests for medicine operations
│   └── test_report.cpp      # Unit tests for report generation
├── data/
│   └── medicines.dat        # Persistent data file (pipe-separated)
├── docs/
│   ├── project_proposal.md  # Initial project proposal
│   └── detailed_design.md   # Detailed design document
├── Makefile                 # Build automation
├── README.md                # This file
├── .gitignore               # Git ignore rules
└── LICENSE                  # Project license
```

---

## Architecture

The project follows a layered architecture to keep things organized and maintainable. Each layer has a single responsibility:

```
┌─────────────────────────────────────┐
│      User Interface (main.c)        │  ← Menu display, user input
├─────────────────────────────────────┤
│    Business Logic (medicine.c)      │  ← Add, sell, search, delete, restock
├──────────────┬──────────────────────┤
│  Validation  │    Reporting         │
│(validation.c)│   (report.c)        │  ← Input checks / report generation
├──────────────┴──────────────────────┤
│    File Storage (file_handler.c)    │  ← Read/write medicines.dat
└─────────────────────────────────────┘
```

**How data flows:**
1. `main.c` shows the menu and collects user input
2. Input is validated through `validation.c` before anything happens
3. Valid requests go to `medicine.c` which performs the actual operation
4. `file_handler.c` handles loading data on startup and saving on exit
5. `report.c` pulls data from the medicine array and formats reports

---

## C Concepts Used

This project makes use of several core C programming concepts:

- **Structures** — `Medicine_t` (stores all medicine info) and `Date_t` (day, month, year for expiry)
- **Enumerations** — `MedicineCategory_t` (Tablet, Syrup, Injection, etc.) and `Status_t` (return codes like `SUCCESS`, `FAILURE`, `NOT_FOUND`)
- **Functions** — Modular design with one responsibility per function
- **Arrays** — Medicine inventory stored as an array of `Medicine_t` structs
- **Strings** — Name matching, input parsing, string manipulation
- **Pointers** — Passing structs by reference, array traversal
- **Constants** — `#define` macros for `MAX_MEDICINES`, `MAX_NAME_LENGTH`, `EXPIRY_WARNING_DAYS`, etc.
- **File Handling** — `fopen`, `fclose`, `fprintf`, `fscanf` for persistent storage
- **Error/Status Codes** — Functions return status codes to indicate success or specific errors

---

## Prerequisites

Before building the project, make sure you have the following installed:

| Tool | Purpose | Install Command (Ubuntu/Debian) |
|---|---|---|
| **GCC** | C compiler | `sudo apt install gcc` |
| **G++** | C++ compiler (for tests) | `sudo apt install g++` |
| **Make** | Build automation | `sudo apt install make` |
| **Google Test** | Unit testing framework | `sudo apt install libgtest-dev` |

> [!NOTE]
> On Windows, you can use MinGW or WSL to get GCC and Make. Google Test setup on Windows takes a few extra steps — check the [gtest docs](https://github.com/google/googletest) for details.

To verify everything is installed:

```bash
gcc --version
g++ --version
make --version
```

---

## Building and Running

### Build the project

```bash
make all
```

This compiles all source files and produces the executable.

### Run the application

```bash
make run
```

### Run unit tests

```bash
make test
```

### Clean build artifacts

```bash
make clean
```

> [!TIP]
> Run `make clean` before `make all` if you want a fresh build from scratch.

---

## Sample Input / Output

Here's what the application looks like when you run it:

### Main Menu

```
============================================
   MEDICAL STORE INVENTORY & EXPIRY TRACKER
============================================

  1. Add Medicine
  2. Sell Medicine
  3. Search by ID
  4. Search by Name
  5. View All Medicines
  6. Restock Medicine
  7. Delete Medicine
  8. Update Price
  9. Generate Report
  10. Save Data
  0. Save & Exit

  Enter your choice: _
```

### Adding a Medicine

```
--- Add New Medicine ---
Enter medicine name: Paracetamol 500mg
Enter batch number: BT2024-001
Select category:
  1. Tablet
  2. Syrup
  3. Injection
  4. Ointment
  5. Drops
  6. Other
Enter category (1-6): 1
Enter price: 25.50
Enter quantity: 200
Enter minimum stock level: 50
Enter expiry date (DD/MM/YYYY): 15/06/2025
Enter supplier name: PharmaCorp Ltd

Medicine added successfully! (ID: 1)
```

### Selling a Medicine

```
--- Sell Medicine ---
Enter medicine ID: 1
Medicine: Paracetamol 500mg
Available quantity: 200
Enter quantity to sell: 30

Sale successful! Remaining stock: 170
```

### Expiry & Low Stock Alerts

```
*** ALERTS ***
[EXPIRY WARNING] ID: 3 - Amoxicillin 250mg expires on 05/01/2025 (12 days remaining)
[LOW STOCK] ID: 7 - Cetrizine 10mg — Current: 8, Minimum: 20
```

---

## Data File Format

Medicine data is stored in `data/medicines.dat` using a pipe-separated format. Each line represents one medicine:

```
ID|Name|BatchNumber|Category|Price|Quantity|MinStock|ExpiryDay|ExpiryMonth|ExpiryYear|Supplier
```

**Example:**

```
1|Paracetamol 500mg|BT2024-001|1|25.50|200|50|15|06|2025|PharmaCorp Ltd
2|Cough Syrup|BT2024-012|2|85.00|45|15|20|09|2025|MediSupply Inc
3|Amoxicillin 250mg|BT2024-003|1|12.75|100|30|05|01|2025|PharmaCorp Ltd
```

> [!IMPORTANT]
> Do not edit `medicines.dat` manually while the program is running — your changes will be overwritten when the program saves.

---

## Coding Standards

We tried to follow consistent coding standards throughout the project:

- **Indentation:** 4 spaces (no tabs)
- **Function names:** camelCase (e.g., `addMedicine`, `validatePrice`)
- **Constants:** UPPER_CASE with underscores (e.g., `MAX_MEDICINES`, `EXPIRY_WARNING_DAYS`)
- **Header guards:** Used in all `.h` files to prevent double inclusion
- **One responsibility per function:** Each function does one thing
- **Compiler warnings:** Code compiles cleanly with `-Wall -Wextra -Wpedantic`
- **Meaningful names:** Variables and functions are named to be self-explanatory

---

## Unit Testing Summary

Unit tests are written in C++ using the **Google Test (gtest)** framework. They live in the `tests/` directory.

| Test File | What It Tests |
|---|---|
| `test_validation.cpp` | Input validation — checks for valid names, prices, quantities, dates, categories |
| `test_medicine.cpp` | Core operations — adding, selling, searching, deleting, restocking medicines |
| `test_report.cpp` | Report generation — total value calculation, category counts, alert detection |

Run all tests with:

```bash
make test
```

Tests cover both normal cases and edge cases (like selling more than available stock, invalid dates, empty names, etc.). We focused on testing the business logic and validation layers since those are the most critical parts.

---

## Error Handling Summary

The project uses status codes (`Status_t` enum) to handle errors consistently:

| Status Code | Meaning |
|---|---|
| `SUCCESS` | Operation completed without issues |
| `FAILURE` | General failure |
| `NOT_FOUND` | Medicine with given ID/name doesn't exist |
| `INVALID_INPUT` | User input failed validation |
| `INVENTORY_FULL` | Cannot add more medicines (max capacity reached) |
| `INSUFFICIENT_STOCK` | Trying to sell more than available quantity |
| `FILE_ERROR` | Could not open/read/write the data file |

**How errors are handled in practice:**
- All user inputs are validated before processing (no raw input goes straight to business logic)
- Functions return status codes; the caller checks them and displays appropriate error messages
- File operations check for `NULL` file pointers and report errors gracefully
- The program doesn't crash on bad input — it shows a message and lets you try again

---

## Known Limitations

- **Single-user only** — No support for concurrent access or multi-user environments
- **Fixed array size** — The inventory has a maximum capacity defined by `MAX_MEDICINES`; it's not dynamically resized
- **No encryption** — Data file is stored in plain text; anyone with file access can read it
- **Basic search** — Name search is simple string matching, not fuzzy or ranked
- **No undo** — Once you delete a medicine or confirm a sale, there's no way to undo it
- **No login/authentication** — Anyone who can run the program has full access
- **Date validation** — Checks basic format and ranges but doesn't account for all edge cases (e.g., leap year February 29 handling may be basic)

---

## Future Enhancements

Some ideas for improving this project down the line:

- **Database backend** — Replace the text file with SQLite for better data management
- **GUI interface** — Add a graphical frontend using GTK or a web-based UI
- **Barcode scanning** — Support barcode input for faster medicine lookup
- **Multi-user support** — Add login system with role-based access (admin vs. cashier)
- **Export reports** — Generate reports in PDF or CSV format
- **Automatic reorder** — Send alerts or auto-generate purchase orders when stock is low
- **Batch expiry tracking** — Track expiry per batch instead of per medicine entry
- **Dynamic memory** — Use linked lists or dynamic arrays instead of fixed-size arrays
- **Backup & restore** — Automatic data backups with restore functionality

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

> Built as a mini project for learning C programming concepts like structures, file handling, modular design, and more. It's not production software, but it covers a realistic use case and gave us a good understanding of how to build a complete application in C.
