# IoT-Enabled Smart Inventory Management System with RFID, Database, and FPGA-Based Stock Monitoring

## Description
A C++ based inventory management application developed using Object-Oriented Programming principles. The system provides product management functions with data persistence and prepares future integration with IoT devices for real-time inventory monitoring.

## Features
✔ Add Product
✔ Display Product
✔ Search Product
   - Search by ID
   - Search by Barcode
   - Search by Name
   - Search by Category
   - Search by Price Range
   - Search by Supplier
✔ Update Product Information
✔ Delete Product
✔ Sort Products
✔ Product Status Monitoring
✔ Data Storage using TXT File
✔ RFID UID Support

## Technologies
1. Language:
    - C++
2. Concepts:
    - Object-Oriented Programming
    - File Handling
    - Data Validation
    - STL (vector, algorithm)
3. Tools:
    - Visual Studio
    - Git/GitHub

## Future Improvements
- MySQL database integration
- ESP32-based product identification
- RFID integration using MFRC522
- Wireless inventory synchronization
- Real-time stock monitoring dashboard

## Screenshots （C++)
### Main Menu
![Main Menu](screenshots/main_menu.png)
### Add Product
![Add Product](screenshots/add_product.png)
### Display Products
![Display Products](screenshots/display_products.png)
### Search Product
![Search Product](screenshots/search_product.png)
### Sort Products
![Sort Products](screenshots/sort_products.png)
### Update Product
![Update Product](screenshots/update_product.png)
### Delete Product
![Delete Product](screenshots/delete_product.png)
### Check Product Status
![Check Product Status](screenshots/check_product_status.png)

## Version History
### v1.0.0
- Product Management
- Search, Sort and Update
- Input Validation
- File-based Storage (.txt) for sample Data. The repository includes a sample `products.txt` file for testing purposes.
Users can modify or replace the file with their own inventory records.)
### v2.0.0 (In Progress)
- SQLite Integration
- Database CRUD Operations
### v3.0.0 (Planned)
- ESP32 + RFID Integration
### v4.0.0 (Planned)
- FPGA-Based Low-Stock Alert
