#include "Inventory.h"
#include "Product.h"
#include <iostream>
#include <windows.h>

using namespace std;

void displayMenu()
{
	cout << "====================================================" << endl;
	cout << "Inventory Management System Menu" << endl;
	cout << "====================================================" << endl;
	cout << "\n";
	cout << "Welcome to YouExpress Inventory Management System!" << endl;
	cout << "Manage your products efficiently and effectively." << endl;
	cout << "\n";
	cout << "What would you like to do today?" << endl;
	cout << "\n";
	cout << "1. Add Product" << endl;
	cout << "2. Display Products" << endl;
	cout << "3. Search Product" << endl;
	cout << "4. Update Product" << endl;
	cout << "5. Delete Product" << endl;
	cout << "6. Exit" << endl;
	cout << "====================================================" << endl;
	cout << "\n";
	cout << "Please select an option: ";
}

int main()
{
	SetConsoleOutputCP(CP_UTF8); // Set console output to UTF-8)

	Inventory inventory; // Create an instance of Inventory

	while (true)
	{
		displayMenu();
		int option;
		cin >> option;

		switch (option)
		{
		case 1: // add product
			std::cin.ignore();
			inventory.addProduct();
			break;

		case 2: //display products
			inventory.displayProducts();
			break;

		case 3: //search product
			
			break;

		case 4: //update product
			
			break;

		case 5: //delete product
			
			break;

		case 6:
			cout << "Exiting the program....." << endl;
			cout << "Thank You for Using YouExpress Inventory Management System!" << endl;
			cout << "Goodbye! Wish You Have A Nice Day!" << endl;
			
			return 0;

		default:
			cout << "It is not a valid option." << endl;
			cout << "Please try again" << endl;
			break;
		}
	}
	
	/*
	//Sample product data
	Product product1(
		1,
		123456789,
		"I LOVE MEE",
		"With flavoring powder and seasoning oil",
		"Instant Noodles",
		50,
		4.90,
		"My Company",
		"2026-12-31",
		"2025-01-01"
	);
	*/

	return 0;
}
