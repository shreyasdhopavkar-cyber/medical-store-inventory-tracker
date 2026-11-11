# Project Proposal

## Medical Store Inventory and Expiry Tracker

---

### 1. Project Information

| Field | Details |
|---|---|
| **Project Title** | Medical Store Inventory and Expiry Tracker |
| **Domain** | Healthcare |
| **Project Type** | Individual Mini Project |
| **Student Name** | Shreyas Yashodhan Dhopavkar |
| **MIS Number** | 612505092 |
| **Repository Link** | https://github.com/shreyasdhopavkar-cyber/medical-store-inventory-tracker |
| **Programming Language** | C |

---

### 2. Engineering Scenario

Small medical shops (pharmacies) in India deal with hundreds of different medicines on a daily basis. Currently, most shopkeepers rely on paper registers or basic notebooks to keep track of their stock. This manual approach causes several real problems:

- **Expired medicines get sold** — This is both a health hazard for the customer and illegal under Indian drug regulations. The shopkeeper simply forgets to check expiry dates among hundreds of items.
- **Stock runs out without notice** — The shopkeeper only realizes a medicine is out of stock when a customer asks for it. There is no warning system.
- **No quick way to check availability** — When a customer asks "Do you have this medicine?", the shopkeeper has to physically go and check the shelf or flip through pages in a register.
- **Manual calculations for bills** — Prices are looked up manually and totals are calculated by hand, leading to errors.
- **No reports or summaries** — The shopkeeper has no easy way to see the total value of inventory, which medicines are selling fast, or how many items are close to expiry.

A simple command-line software tool can solve most of these problems. It does not need to be fancy — it just needs to be reliable and easy to use. The shopkeeper can enter medicines into the system, sell them, and the software will automatically track quantities, warn about expiry, and generate basic reports. All data is saved to files so nothing is lost when the program is closed.

---

### 3. Problem Statement

In small medical stores, shopkeepers currently face difficulty in tracking medicine stock levels, expiry dates, and sales. The current manual method of using paper registers results in expired medicines being overlooked, stock shortages going unnoticed, and inaccurate inventory records. There is no mechanism to alert the shopkeeper when a medicine is about to expire or when stock falls below a safe level. This leads to financial losses, legal risks, and poor customer service.

The proposed software will provide a digital inventory management system so that users can efficiently manage stock, receive expiry alerts, and generate inventory reports. It will be a command-line application written in C that stores all data in text files, making it simple to use and deploy on any computer without special software.

---

### 4. Target Users

| # | User | Role / How They Use the System |
|---|---|---|
| 1 | **Shopkeeper / Pharmacist** | This is the primary user. They add new medicines to inventory when stock arrives, sell medicines to customers (which deducts quantity), search for medicines by name or ID, check expiry alerts before opening the shop each day, and restock items when new supply comes in. |
| 2 | **Store Manager / Owner** | This user is more interested in the overall picture. They view summary reports to see total inventory value, check which medicines are expiring soon, review low-stock items that need to be reordered, and monitor the general health of the store's inventory. |

---

### 5. Scope

#### 5.1 In-Scope (What the software WILL do)

- Add new medicine records with all details (name, batch number, category, price, quantity, expiry date, supplier)
- Delete medicine records that are no longer needed
- Search for medicines by ID or by name
- Update medicine details (like price changes)
- Sell medicines with automatic stock deduction
- Restock medicines when new supply arrives
- Generate expiry alerts — show medicines expiring within the next 30 days
- Generate low stock alerts — show medicines with quantity below a threshold
- Display all medicines in a formatted table
- Save all data to a file when exiting
- Load all data from a file when starting
- Input validation to prevent bad data entry (empty names, negative prices, invalid dates, etc.)
- Generate summary reports (total items, total inventory value, category-wise counts)

#### 5.2 Out-of-Scope (What the software will NOT do)

- **No GUI** — The application is command-line only. No graphical windows or buttons.
- **No networking** — The software runs on a single computer. No internet or LAN features.
- **No multi-user support** — Only one person can use the software at a time. No login/password system.
- **No billing / invoice printing** — The software tracks inventory but does not generate printed bills or invoices.
- **No barcode scanning** — Medicine entry is done manually by typing. No barcode reader support.
- **No database** — Data is stored in plain text files, not in a database like MySQL or SQLite.

---

### 6. Functional Requirements

| Req ID | Requirement | Description |
|---|---|---|
| FR-01 | Add Medicine | The system shall allow the user to add a new medicine by entering its name, batch number, category, price per unit, quantity, expiry date (month and year), and supplier name. A unique medicine ID is auto-generated. |
| FR-02 | Validate Medicine ID | The system shall check that a medicine ID exists before performing any operation (sell, delete, update, restock) on it. If the ID is not found, it shall display an appropriate error message. |
| FR-03 | Search by ID | The system shall allow the user to search for a medicine using its unique ID and display all its details if found. |
| FR-04 | Search by Name | The system shall allow the user to search for medicines by name (or partial name). It shall display all medicines whose names match or contain the search term. The search shall be case-insensitive. |
| FR-05 | Sell Medicine | The system shall allow the user to sell a medicine by entering its ID and the quantity to sell. It shall check that enough stock is available. If yes, it shall deduct the quantity and display the total cost. If not, it shall show an error. |
| FR-06 | Restock Medicine | The system shall allow the user to add more quantity to an existing medicine by entering its ID and the quantity to add. The new quantity shall be added to the current stock. |
| FR-07 | Delete Medicine | The system shall allow the user to delete a medicine record by its ID. Before deleting, it shall ask the user for confirmation. |
| FR-08 | Update Price | The system shall allow the user to update the price per unit of a medicine by entering its ID and the new price. |
| FR-09 | Display All Medicines | The system shall display all medicines currently in the inventory in a formatted table showing ID, name, batch, category, price, quantity, expiry date, and supplier. |
| FR-10 | Generate Expiry Alert | The system shall identify and display all medicines whose expiry date is within the next 30 days (or already expired) so the shopkeeper can remove them from the shelf. |
| FR-11 | Generate Low Stock Alert | The system shall identify and display all medicines whose quantity is below a defined threshold (e.g., 10 units). This helps the shopkeeper know what to reorder. |
| FR-12 | Save and Load Data | The system shall save all medicine records to a file when the user exits the program. When the program starts, it shall load previously saved records from the file so that data is not lost between sessions. |

---

### 7. Non-Functional Requirements

| Req ID | Category | Requirement |
|---|---|---|
| NFR-01 | **Usability** | The system shall have a clear menu-driven interface with numbered options. The user should be able to navigate without any training. Error messages shall be descriptive and guide the user on what to do. |
| NFR-02 | **Performance** | The system shall respond to all user actions (add, search, sell, delete) within 1-2 seconds for an inventory of up to 500 medicines. File loading and saving shall complete without noticeable delay. |
| NFR-03 | **Reliability** | The system shall not crash due to invalid user input. All inputs shall be validated before processing. If invalid data is entered, the system shall prompt the user to re-enter without losing previous data. |
| NFR-04 | **Portability** | The system shall be written in standard C (C99/C11) and shall compile and run on any platform with a standard C compiler (GCC, MinGW, Turbo C). No platform-specific libraries shall be used. |
| NFR-05 | **Maintainability** | The code shall be organized into separate modules (source files) with clear function names and comments. Each module shall handle a specific responsibility, making it easy to update or extend. |
| NFR-06 | **Data Integrity** | The system shall save data to the file in a consistent format. If the file is corrupted or missing, the system shall start with an empty inventory and inform the user instead of crashing. |

---

### 8. System Modules

The project code will be organized into the following modules (source files):

| # | Module | Files | Responsibility |
|---|---|---|---|
| 1 | **Main Module** | `main.c` | Application entry point. Displays the main menu, takes user choices, and calls the appropriate functions from other modules. Controls the overall workflow of the program. |
| 2 | **Medicine Module** | `medicine.c`, `medicine.h` | Contains all medicine-related data operations — adding a new medicine, deleting a medicine, finding/searching medicines, selling (quantity deduction), restocking, and updating price. Defines the `Medicine` structure. |
| 3 | **Validation Module** | `validation.c`, `validation.h` | Contains input validation functions — checking if a string is empty, if a number is positive, if a date is valid, if a quantity is within range, etc. Used by other modules before processing data. |
| 4 | **File Handler Module** | `file_handler.c`, `file_handler.h` | Handles reading medicine records from a file (on program start) and writing all records to a file (on program exit or save). Handles cases where the file does not exist yet. |
| 5 | **Report Module** | `report.c`, `report.h` | Handles display and report generation — printing all medicines in a table, generating the expiry alert list, generating the low stock alert list, and showing summary statistics (total items, total value, category-wise breakdown). |

#### Module Interaction Diagram

```
┌─────────────────────────────────────────────────┐
│                   main.c                        │
│          (Menu, User Input, Flow Control)        │
└──────┬──────────┬───────────┬───────────┬───────┘
       │          │           │           │
       ▼          ▼           ▼           ▼
  ┌─────────┐ ┌──────────┐ ┌──────────┐ ┌────────┐
  │medicine │ │validation│ │  file    │ │ report │
  │  .c/.h  │ │  .c/.h   │ │handler   │ │ .c/.h  │
  │         │ │          │ │  .c/.h   │ │        │
  │ Add     │ │ Check    │ │ Save to  │ │ Display│
  │ Delete  │ │ empty    │ │ file     │ │ all    │
  │ Search  │ │ Check    │ │ Load     │ │ Expiry │
  │ Sell    │ │ number   │ │ from     │ │ alert  │
  │ Restock │ │ Check    │ │ file     │ │ Low    │
  │ Update  │ │ date     │ │          │ │ stock  │
  └─────────┘ └──────────┘ └──────────┘ │ Summary│
                                         └────────┘
```

---

### 9. Planned C Concepts

The following C programming concepts will be used in this project:

| Concept | How It Will Be Used |
|---|---|
| **Functions** | The entire program is divided into small functions — each doing one task (e.g., `addMedicine()`, `sellMedicine()`, `searchById()`). This keeps the code organized and reusable. |
| **Structures (`struct`)** | A `Medicine` structure will hold all details of a medicine (ID, name, batch, category, price, quantity, expiry month, expiry year, supplier). An array of structures will store the full inventory. |
| **Enumerations (`enum`)** | Enumerations will be used for medicine categories (e.g., `TABLET`, `SYRUP`, `INJECTION`, `OINTMENT`, `DROPS`) and for status/error codes returned by functions (e.g., `SUCCESS`, `NOT_FOUND`, `OUT_OF_STOCK`). |
| **Arrays** | An array of `Medicine` structures will be used to store all medicine records in memory. Character arrays (strings) will store names, batch numbers, etc. |
| **Strings** | String functions from `<string.h>` (like `strcmp`, `strcpy`, `strstr`, `strlen`) will be used for name comparisons, searching, and copying data. |
| **Pointers** | Pointers will be used to pass arrays and structures to functions efficiently (pass by reference). File pointers (`FILE *`) will be used for file operations. |
| **Constants (`#define` / `const`)** | Constants will define fixed values like `MAX_MEDICINES` (maximum inventory size), `LOW_STOCK_THRESHOLD`, `MAX_NAME_LENGTH`, etc. |
| **File Handling** | `fopen()`, `fclose()`, `fprintf()`, `fscanf()` (or `fwrite()`/`fread()`) will be used to save and load medicine data to/from a text file so data persists between sessions. |
| **Error / Status Codes** | Functions will return status codes (using `enum`) to indicate success or the type of error. The calling code (in `main.c`) will use these to display appropriate messages. |

---

### 10. Summary

This project aims to build a simple but practical command-line application in C that helps small medical store owners manage their inventory. The key features are adding and selling medicines, tracking expiry dates, alerting for low stock, and generating reports. The code will be modular, well-structured, and will use core C programming concepts like structures, file handling, functions, and enumerations. All data will be persisted using file I/O so nothing is lost between sessions.

---
