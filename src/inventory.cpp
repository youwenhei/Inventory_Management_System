#include "Inventory.h"
#include <iostream>
#include <cctype>

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
		if (i == 4 || i == 7) continue;
		if (!isdigit(date[i])) return false;
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

	do
	{
		std::cout << "Enter Product Quantity: ";
		std::cin >> quantity;

		if (!isValidQuantity(quantity))
		{
			std::cout << "The quantity format is invalid. Please re-enter the quantity with a non-negative integer." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
		}
	} while (!isValidQuantity(quantity));

	do
	{
		std::cout << "Enter Product Price: RM ";
		std::cin >> price;

		if (!isValidPrice(price))
		{
			std::cout << "The price format is invalid. Please re-enter the price with a non-negative number." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
		}
	} while (!isValidPrice(price));

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
	int id;

	std::cout << "Please Enter the Product ID to Search: ";
	std::cin >> id;

	bool found = false;

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
		std::cout << "The product with ID " << id << " is not found." << std::endl;
	}
}
