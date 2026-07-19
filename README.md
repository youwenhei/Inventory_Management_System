# Smart Inventory Management System with RFID Support and Database Integration

Project Progress: Still on Progress

Full Project Name: IoT-Enabled Smart Inventory Management System with RFID, Database, and FPGA-Based Stock Monitoring

Full Project System Architecture：



---

## Description

A C++ based smart inventory management system developed using Object-Oriented Programming (OOP) principles.

The system provides complete product management functionalities with MySQL database integration, inventory dashboard monitoring, and report generation. The system provides inventory management functionalities with MySQL database integration, dashboard monitoring, RFID-based product identification, and IoT device status monitoring.

## Current System Architecture (Until Version 3.0)

Hardware Layer
────────────────────────
RFID Card
RC522 RFID Reader
ESP32

Communication Layer
────────────────────────
USB Serial Communication

Application Layer
────────────────────────
C++ Inventory Management System
Dashboard
RFID Scanner
Inventory Module
Report Module

Database Layer
────────────────────────
MySQL Database

# In Diagram:



---

# Features

## Product Management
✔ Add Product  
✔ Display Product  
✔ Update Product Information  
✔ Delete Product  

## Product Search
✔ Search by ID  
✔ Search by Barcode  
✔ Search by Name  
✔ Search by Category  
✔ Search by Price Range  
✔ Search by Supplier  

## Product Organization
✔ Sort Products
   - Sort by ID
   - Sort by Name
   - Sort by Category
   - Sort by Quantity
   - Sort by Price
   - Sort by Expiry Date

## Inventory Monitoring
✔ Product Status Monitoring  
✔ Low Stock Detection  
✔ Out of Stock Detection  
✔ Inventory Dashboard Statistics  
   - Total Products
   - Total Quantity
   - Inventory Value
   - RFID Registration Rate

## IoT Device Monitoring
✔ ESP32 Connection Status Monitoring  
✔ RFID Module Connection Status Monitoring  
✔ Device Communication Status Display

## Database Integration
✔ MySQL Database Storage  
✔ CRUD Database Operations  
✔ Database Synchronization  
✔ Transaction Handling using Commit/Rollback  
✔ Database Connection Status Monitoring

## Reporting
✔ CSV Inventory Report Export  
✔ TXT Inventory Report Export

## RFID Integration
✔ RFID UID Storage  
✔ RFID UID Update  
✔ RFID Product Identification  
✔ RFID Scanner Module  
✔ RFID-based Product Lookup
✔ RFID Connection Status Monitoring

---

# Technologies

## Programming Language
- C++

## Concepts
- Object-Oriented Programming
- File Handling
- Database Management
- Data Validation
- STL (vector, algorithm)
- Exception Handling

## Database
- MySQL
- MySQL Connector/C++

## Hardware Integration
- ESP32 Microcontroller
- MFRC522 RFID Module
- FPGA-Based Low-Stock Alert System (Planned)

## Tools
- Visual Studio
- MySQL Workbench
- Git/GitHub

---

# Screenshots

## Version 1.0 - C++ Inventory Management System

### Main Menu
![Main Menu](screenshots/v1.0/main_menu.png)

### Add Product
![Add Product](screenshots/v1.0/add_product.png)

### Display Products
![Display Products](screenshots/v1.0/display_products.png)

### Search Product
![Search Product](screenshots/v1.0/search_product.png)

### Sort Products
![Sort Products](screenshots/v1.0/sort_products.png)

### Update Product
![Update Product](screenshots/v1.0/update_product.png)

### Delete Product
![Delete Product](screenshots/v1.0/delete_product.png)

### Product Status Monitoring
![Check Product Status](screenshots/v1.0/check_product_status.png)


---

# Version 2.0 - MySQL Database Integration & Smart Dashboard

### Database Connection
![Database Connection](screenshots/v2.0/database_connection.png)

### Main Menu(Updated Version)
![Updated Main Menu](screenshots/v2.0/main_menu.png)

### Inventory Dashboard
![Inventory Dashboard](screenshots/v2.0/dashboard.png)
![Dahsboard Sample Case](screenshots/v2.0/dashboard_after_delete_product.png)
![Dahsboard Sample Case](screenshots/v2.0/dashboard_after_update.png)

### Database Product Table After Update/Delete
![Database Table](screenshots/v2.0/database_table_after_update_delete.png)

### Database Structure
![Database Structure](screenshots/v2.0/database_structure.png)

### RFID UID Support
![Add Product](screenshots/v2.0/add_product_crud.png)
![Update Product](screenshots/v2.0/update_product_crud.png)

### Refresh Function
![Refresh Function](screenshots/v2.0/refresh_product_list.png)

### Export Report
![Export Report](screenshots/v2.0/export_report.png)

### CSV/TXT Report
![CSV Report](screenshots/v2.0/csv_export.png)
![TXT Report](screenshots/v2.0/txt_report.png)


---


# Version 3.0 - RFID and IoT Device Integration

## Main Menu with RFID Scanning Function
![Main Menu](screenshots/v3.0/main_menu_with_rfid.png)

## System Dashboard

## Before
![Dashboard](screenshots/v3.0/dashboard_before_scanning_rfid.png)

## After
![Dashboard](screenshots/v3.0/dashboard_for_hardware_connection.png)


## RFID Identification

![RFID Scanner](screenshots/v3.0/wait_for_rfid_scanning.png)
![RFID Scanner](screenshots/v3.0/after_rfid_scanning.png)
![RFID Scanner](screenshots/v3.0/product_found_after_scanning.png)


## Database Integration

![MySQL](screenshots/v3.0/mysql_rfid.png)


## Hardware Prototype

![RFID Hardware](screenshots/v3.0/hardware/hardware_connection.jpg)


---


# Version 3.5 - Real-Time RFID Inventory Monitoring


---


# Version 4.0 - FPGA-Based Low Stock Alert


---


# Version History

## v1.0
Initial C++ Inventory Management System.

Implemented:
- Product Management
- Search and Sorting Functions
- Input Validation
- File-based Data Storage using `products.txt`
- Product Status Monitoring

---

## v2.0
MySQL Database Integration and Smart Inventory Dashboard.

Implemented:
- MySQL database storage
- CRUD database operations
- Database synchronization
- Inventory dashboard
- Database connection monitoring
- CSV/TXT report export
- RFID UID support

---

## v3.0
RFID and IoT Device Integration.

Implemented:
-RFID UID Storage
- RFID UID-based product identification
- RFID scanner module
- ESP32 device status monitoring
- RFID connection monitoring

## v3.5
Real-Time RFID Inventory Monitoring

Advanced Features:
-ESP32 sends sensor data
-MQTT communication
-Automatic database update
-Dashboard auto refresh
-Configuration Management: Externalized hardware and database parameters using configuration file.

---

## v4.0 (Planned)
FPGA-Based Low-Stock Alert System.

Planned:
- FPGA low-stock alert system
- Hardware-based inventory monitoring
- Real-time stock warning mechanism
