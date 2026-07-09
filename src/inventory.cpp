#include "Inventory.h"
#include <iostream>
#include <cctype>
#include <cstdlib>
#include <algorithm>
#include <limits>

Inventory::Inventory()
{
	newProductID = 1;
}

//date validation function
bool isValidDate(const std::string& date)
{
	if (date.empty())
		return true; //for specific product

	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
	{
		return false;
	}

	for (int i = 0; i < date.length(); i++)
	{
		if (i == 4 || i == 7)
			continue;
		if (!isdigit(date[i]))
			return false;
	}
		int year = std::stoi(date.substr(0, 4));
		int month = std::stoi(date.substr(5, 2));
		int day = std::stoi(date.substr(8, 2));

		if (month < 1 || month > 12)
		{
			return false;
		}

		int daysInMonth;

		switch (month)
		{
		case 2:
			if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
				daysInMonth = 29; // Leap year
			else
				daysInMonth = 28;
			break;

		case 4: case 6: case 9: case 11:
			daysInMonth = 30;
			break;

		default:
			daysInMonth = 31;
		}

		if (day < 1 || day > daysInMonth)
		{
			return false;
		}

	return true;
}

//barcode validation function
bool isValidBarcode(const std::string& barcode)
{
	if (barcode.length() != 9)
	{
		return false;
	}

	for (char c : barcode)
	{
		if (!isdigit(c))
		{
			return false;
		}
	}

	return true;
}

//quantity validation function
bool isValidQuantity(int quantity)
{
	return quantity >= 0;
}

//price validation function
bool isValidPrice(double price)
{
	return price >= 0.0;
}

void Inventory::addProduct()
{
	int quantity;
	double price;
	std::string barcode, name, description, category, supplier, expiryDate, manufactureDate;

	std::cout << "\n===============================Add Product===============================\n";
	do
	{
		std::cout << "Enter Product Barcode (9 digits): ";
		std::getline(std::cin, barcode);

		if (!isValidBarcode(barcode))
		{
			std::cout << "The barcode format is invalid. Please re-enter the barcode with 9 digits." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
		}

		else if (isBarcodeExist(barcode))
		{
			std::cout << "The barcode is already in use. Please enter a unique barcode." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
		}

	} while (!isValidBarcode(barcode) || isBarcodeExist(barcode));

	std::cout << "Enter Product Name: ";
	std::getline(std::cin, name);

	std::cout << "Enter Product Description: ";
	std::getline(std::cin, description);

	std::cout << "Enter Product Category: ";
	std::getline(std::cin, category);

	while (true)
	{
		std::cout << "Enter Product Quantity: ";
		if (!(std::cin >> quantity) || quantity < 0)
		{
			std::cout << "The quantity format is invalid. Please re-enter a valid quantity." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		if (!isValidQuantity(quantity))
		{
			std::cout << "The quantity cannot be negative. Please re-enter the quantity with a non-negative quantity." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			continue;
		}
		break;
	}

	while (true)
	{
		std::cout << "Enter Product Price: RM ";
		if (!(std::cin >> price) || price < 0.0)
		{
			std::cout << "The price format is invalid. Please re-enter the price with a non-negative number." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;

		}

		if (!isValidPrice(price))
		{
			std::cout << "The price cannot be negative. Please re-enter the price with a non-negative number." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			continue;
		}
		break;
	}

	std::cin.ignore();

	std::cout << "Enter Product Supplier: ";
	std::getline(std::cin, supplier);

	do
	{
		std::cout << "Enter Product Expiry Date (YYYY-MM-DD, press Enter if none): ";
		std::getline(std::cin, expiryDate);

		if (!isValidDate(expiryDate))
		{
			std::cout << "The date format is invalid. Please re-enter the date in YYYY-MM-DD format." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
		}
	} while (!isValidDate(expiryDate));

	do
	{
		std::cout << "Enter Product Manufacture Date (YYYY-MM-DD, press Enter if none): ";
		std::getline(std::cin, manufactureDate);

		if (!isValidDate(manufactureDate))
		{
			std::cout << "The date format is invalid. Please re-enter the date in YYYY-MM-DD format." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
		}
	} while (!isValidDate(manufactureDate));

	while (true)
	{
		if (!expiryDate.empty() && !manufactureDate.empty() && expiryDate < manufactureDate)
		{
			std::cout << "The expiry date cannot be earlier than the manufacture date. Please re-enter the dates." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			continue;
		}

		if (!expiryDate.empty() && !manufactureDate.empty() && expiryDate > manufactureDate)
		{
			std::cout << "The manufacture date cannot be later than the expiry date. Please re-enter the dates." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			return;
		}
		break;
	}

	Product newProduct(
		newProductID,
		barcode,
		name,
		description,
		category,
		quantity,
		price,
		supplier,
		expiryDate,
		manufactureDate
	);

	addProduct(newProduct);
	newProductID++;

	std::cout << "=======================================================================\n";
	std::cout << "Product is added successfully!\n";
}

bool Inventory::isBarcodeExist(const std::string& barcode) const
{
	for (const auto& product : products)
	{
		if (product.getBarcode() == barcode)
		{
			return true;
		}
	}
	return false;
}

std::string toLowerCase(const std::string& str)
{
	std::string lowerStr = str;
	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return lowerStr;
}

void Inventory::addProduct(const Product& product)
{
	products.push_back(product); //to add the product to the vector 
	//(products(empty)) become (products(1)) after adding the product
}

void Inventory::displayProducts() const
{
	if (products.empty())
	{
		std::cout << "No products available in the inventory. Please try again!" << std::endl;
		return;
	}

	for (const auto& product : products)
	{
		product.display();
	}
}

void Inventory::searchProduct() const
{
	int option;
	std::cout << "=======================================================================\n";
	std::cout << "Search Product by:\n";
	std::cout << "=======================================================================\n";
	std::cout << "1. Search by Product ID" << std::endl;
	std::cout << "2. Search by Product Barcode" << std::endl;
	std::cout << "3. Search by Product Name" << std::endl;
	std::cout << "4. Search by Product Category" << std::endl;
	std::cout << "5. Search by Product Supplier" << std::endl;
	std::cout << "6. Search by Product Price Range" << std::endl;
	std::cout << "7. Return to Main Menu" << std::endl;
	std::cout << "8. Exit" << std::endl;
	std::cout << "=======================================================================\n";
	std::cout << "Please select an option: ";
	while (true)
	{
		if (!(std::cin >> option))
		{
			std::cout << "The input is invalid. Please re-enter a number between 1 and 8." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		if (option < 1 || option > 8)
		{
			std::cout << "The option is invalid. Please try again." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			continue;
		}
		break;
	}
	std::cout << "=======================================================================\n";
	std::cout << "The page is loading, please wait..." << std::endl;

	switch (option)
	{
		case 1:
		{
			bool found = false;
			int id;

			std::cout << "Please enter Product ID to search: ";
			std::cin >> id;

			for (const auto& product : products)
			{
				if (product.getID() == id)
				{
					product.display();
					found = true;
					break;
				}
			}

			if (!found)
			{
				std::cout << "Sorry! The product with ID " << id << " is not found.Please try again!" << std::endl;
			}
			break;
		}

		case 2:
		{
			bool found = false;
			std::string barcode;

			std::cout << "Please enter Product Barcode to search: ";
			std::cin >> barcode;

			for (const auto& product : products)
			{
				if (product.getBarcode() == barcode)
				{
					product.display();
					found = true;
					break;
				}
			}

			if (!found)
			{
				std::cout << "Sorry! The product with Barcode " << barcode << " is not found.Please try again!" << std::endl;
			}
			break;
		}

		case 3:
		{
			bool found = false;
			std::string name;
			
			std::cout << "Please enter Product Name to search: ";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::getline(std::cin, name);
			
			for (const auto& product : products)
			{
				if (toLowerCase(product.getName()).find(toLowerCase(name))
					!= std::string::npos)
				{
					product.display();
					found = true;
				}
			}
			
			if (!found)
			{
				std::cout << "Sorry! The product with Name " << name << " is not found.Please try again!" << std::endl;
			}
			break;
		}

		case 4:
		{
			bool found = false;
			std::string category;
			
			std::cout << "Please enter Product Category to search: ";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::getline(std::cin, category);
			
			for (const auto& product : products)
			{
				if (toLowerCase(product.getCategory()).find(toLowerCase(category))
					!= std::string::npos)
				{
					product.display();
					found = true;
				}
			}
			
			if (!found)
			{
				std::cout << "Sorry! The product with Category " << category << " is not found.Please try again!" << std::endl;
			}
			break;
		}

		case 5:
		{
			bool found = false;
			std::string supplier;
			
			std::cout << "Please enter Product Supplier to search: ";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::getline(std::cin, supplier);
			
			for (const auto& product : products)
			{
				if (toLowerCase(product.getSupplier()).find(toLowerCase(supplier))
					!= std::string::npos)
				{
					product.display();
					found = true;
				}
			}
			
			if (!found)
			{
				std::cout << "Sorry! The product with Supplier " << supplier << " is not found.Please try again!" << std::endl;
			}
			break;
		}

		case 6:
		{
			bool found = false;
			double minPrice, maxPrice;
		
			std::cout << "Please enter minimum price: RM ";
			std::cin >> minPrice;
		
			std::cout << "Please enter maximum price: RM ";
			std::cin >> maxPrice;
		
			if (minPrice > maxPrice)
			{
				std::cout << "The minimum price cannot be greater than the maximum price. Please try again!" << std::endl;
				break;
			}

			for (const auto& product : products)
			{
				if (product.getPrice() >= minPrice && product.getPrice() <= maxPrice)
				{
					product.display();
					found = true;
				}
			}
		
			if (!found)
			{
				std::cout << "Sorry! No products found in the price range RM " << minPrice << " - RM " << maxPrice << ". Please try again!" << std::endl;
			}
			break;
		}

		case 7:
		{
			std::cout << "You are now returning to main menu..." << std::endl;
			break;
		}

		case 8:
		{
			std::cout << "You are exiting the program. Thank you for using the Inventory Management System!" << std::endl;
			std::cout << "Goodbye! Wish you have a happy day!" << std::endl;
			exit(0);
		}

		default:
		{
			std::cout << "This is an invalid option. Please try again!" << std::endl;
			break;
		}
	}
