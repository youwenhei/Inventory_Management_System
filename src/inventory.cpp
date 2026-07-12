#include "Inventory.h"
#include "Utility.h"
#include <iostream>
#include <cctype>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <cmath>
#include <string>
#include <stdexcept>
//=======================================================================
Inventory::Inventory()
{
	newProductID = 1;
}
//=======================================================================
//menu
int Inventory::getMenuOption(int min, int max) const
{
	int option;

	while (true)
	{
		std::string input;
		std::getline(std::cin, input);

		try
		{
			size_t pos;
			option = std::stoi(input, &pos);

			if (pos != input.length())
			{
				throw std::invalid_argument("Invalid");
			}

			if (option >= min && option <= max)
			{
				return option;
			}

			std::cout << "The input is invalid. Please enter a number between "
				<< min << " and " << max << "." << std::endl;
		}

		catch (const std::exception&)
		{
			std::cout << "The input is invalid. Please enter a valid number." << std::endl;
		}
	}
}
//=======================================================================
//date validation
bool isValidDate(const std::string& date)
{
	if (date.empty())
		return true; //for specific product

	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
	{
		return false;
	}

	for (size_t i = 0; i < date.length(); i++)
	{
		if (i == 4 || i == 7)
			continue;
		if (!std::isdigit(static_cast<unsigned char>(date[i])))
			return false;
	}
		int year = std::stoi(date.substr(0, 4));
		int month = std::stoi(date.substr(5, 2));
		int day = std::stoi(date.substr(8, 2));

		if (month < 1 || month > 12)
		{
			return false;
		}

		if (year < 1900)
			return false;

		int daysInMonth;

		switch (month)
		{
		case 2:
			if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
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
//=======================================================================
//barcode validation
bool isValidBarcode(const std::string& barcode)
{
	if (barcode.length() != 9)
	{
		return false;
	}

	for (char c : barcode)
	{
		if(!std::isdigit(static_cast<unsigned char>(c)))
		{
			return false;
		}
	}
	return true;
}
//=======================================================================
//name validation
bool isValidName(const std::string& name)
{
	if (name.empty())
	{
		return false;
	}

	for (char c : name)
	{
		if (!std::isalnum(static_cast<unsigned char>(c)) && c != ' ' && c != '&' && c != '-')
		{
			return false;
		}
	}
	return true;
}
//=======================================================================
//name validation
bool isValidCategory(const std::string& category)
{
	if (category.empty())
	{
		return false;
	}

	for (char c : category)
	{
		if (!std::isalnum(static_cast<unsigned char>(c)) && c != ' ' && c != '&' && c != '-')
		{
			return false;
		}
	}
	return true;
}
//=======================================================================
//quantity validation
bool isValidQuantity(int quantity)
{
	return quantity >= 0;
}
//=======================================================================
//price validation
bool isValidPrice(double price)
{
	return std::isfinite(price) && price >= 0;
}
//=======================================================================
//supplier validation
bool isValidSupplier(const std::string& supplier)
{
	if (supplier.empty())
	{
		return false;
	}

	for (char c : supplier)
	{
		if (!std::isalnum(static_cast<unsigned char>(c)) && c != ' ' && c != '&' && c != '-')
		{
			return false;
		}
	}
	return true;
}
//=======================================================================
//check for the existance of barcode
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
//=======================================================================
std::string trim(const std::string& str)
{
	size_t start = str.find_first_not_of(" \t\r\n");
	size_t end = str.find_last_not_of(" \t\r\n");

	if (start == std::string::npos)
	{
		return "";
	}
	return str.substr(start, end - start + 1);
}
//=======================================================================
//change case
std::string toLowerCase(const std::string& str)
{
	std::string lowerStr = str;

	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), [](unsigned char c)
		{
			return std::tolower(c);
		}
	);
	return lowerStr;
}
//=======================================================================
//checking if there is product
bool Inventory::hasProducts() const
{
	return !products.empty();
}
//=======================================================================
//Exit Program
void Inventory::exitProgram() const
{
	char confirm;

	while (true)
	{
		std::cout << "Are you sure you want to exit the program? (Y/N): ";
		std::cin >> confirm;

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		confirm = std::tolower(static_cast<unsigned char>(confirm));

		if (confirm == 'y' || confirm == 'n')
		{
			break;
		}
		std::cout << "The input is invalid. Please enter Y or N." << std::endl;
	}

	if (confirm == 'y')
	{
		std::cout << "You are exiting the program. Thank you for using the Inventory Management System!" << std::endl;
		std::cout << "Goodbye! Wish you have a happy day!" << std::endl;
		std::exit(0);
	}
}
//Return Program
void Inventory::returnProgram() const
{
	std::cout << "You are now returning to main menu..." << std::endl;
	return;
}
//=======================================================================
//=======================================================================
//Functions in Main Menu
//=======================================================================
//=======================================================================
//Display function
void Inventory::displayProducts() const
{
	for (const auto& product : products)
	{
		product.display();
	}
}
//=======================================================================
//Main Add function
void Inventory::addProduct()
{
	int quantity;
	double price;
	std::string barcode, name, description, category, supplier, expiryDate, manufactureDate;

	std::cout << "\n===============================Add Product===============================\n";

	//barcode validation
	while (true)
	{
		std::cout << "Enter Product Barcode (9 digits): ";
		std::getline(std::cin, barcode);
		barcode = trim(barcode);

		if (!isValidBarcode(barcode))
		{
			std::cout << "The barcode format is invalid. Please re-enter the barcode with 9 digits." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			continue;
		}

		else if (isBarcodeExist(barcode))
		{
			std::cout << "The barcode is already in use. Please enter a unique barcode." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			continue;
		}
		break;
	}

	//name validation
	while (true)
	{
		std::cout << "Enter Product Name: ";
		std::getline(std::cin, name);
		name = trim(name);

		if (!isValidName(name))
		{
			std::cout << "The Product Name format is invalid. Please re-enter the Product Name." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			continue;
		}	
		break;
	}

	//description vaidation
	while (true)
	{
		std::cout << "Enter Product Description: ";
		std::getline(std::cin, description);
		description = trim(description);

		if (!description.empty())
		{
			break;
		}
		std::cout << "The Product Description cannot be empty. Please enter the Product Description: ";
	}

	//category validation
	while (true)
	{
		std::cout << "Enter Product Category: ";
		std::getline(std::cin, category);
		category = trim(category);

		if (!isValidCategory(category))
		{
			std::cout << "The Product Category is invalid. Please re-enter the Product Category." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			continue;
		}
		break;
	}

	//quantity validation
	while (true)
	{
		std::cout << "Enter Product Quantity: ";
		if (!(std::cin >> quantity) || !isValidQuantity(quantity))
		{
			std::cout << "The quantity format is invalid. Please re-enter a valid quantity." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		break;
	}

	//price validation
	while (true)
	{
		std::cout << "Enter Product Price: RM ";
		if (!(std::cin >> price) || !isValidPrice(price))
		{
			std::cout << "The price format is invalid. Please re-enter the price with a non-negative number." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		break;
	}

	//supplier validation
	while (true)
	{
		std::cout << "Enter Product Supplier: ";
		std::getline(std::cin, supplier);
		supplier = trim(supplier);

		if (!isValidSupplier(supplier))
		{
			std::cout << "The Product Supplier format is invalid. Please re-enter the Product Supplier name." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			continue;
		}
		break;
	}

	//dates validation
	while (true)
	{
		//expiry date validation
		std::cout << "Enter Product Expiry Date (YYYY-MM-DD, press Enter if none): ";
		std::getline(std::cin, expiryDate);
		expiryDate = trim(expiryDate);

		if (!isValidDate(expiryDate))
		{
			std::cout << "The date format is invalid. Please re-enter the date in YYYY-MM-DD format." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			continue;
		}
		break;
	}

	while (true)
	{
		//manufacture date validation
		std::cout << "Enter Product Manufacture Date (YYYY-MM-DD, press Enter if none): ";
		std::getline(std::cin, manufactureDate);
		manufactureDate = trim(manufactureDate);

		if (!isValidDate(manufactureDate))
		{
			std::cout << "The date format is invalid. Please re-enter the date in YYYY-MM-DD format." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			continue;
		}
		break;
	}

	//check for expiry date and manufacture date
	//YYYY-MM-DD format allow lexicographical comparison (string comparison)
	if (!expiryDate.empty() && !manufactureDate.empty() && expiryDate < manufactureDate)
	{
		std::cout << "The expiry date cannot be earlier than the manufacture date. Please re-enter the dates." << std::endl;
		std::cout << "Thank you for your understanding." << std::endl;
		return addProduct();
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
//ADD function
void Inventory::addProduct(const Product& product)
{
	products.push_back(product); //to add the product to the vector 
	//(products(empty)) become (products(1)) after adding the product
}
//=======================================================================
//Main Search function
void Inventory::searchProduct() const
{
	while (true)
	{
		clearScreen();
		if (!hasProducts())
		{
			pauseScreen("Please Enter to return...");
			clearScreen();
			return;
		}

		displaySearchMenu();

		int option = getMenuOption(1, 8);

		bool found = false;

		switch (option)
		{
		case 1:
			found = searchByID();
			break;

		case 2:
			found = searchByBarcode();
			break;

		case 3:
			found = searchByName();
			break;

		case 4:
			found = searchByCategory();
			break;

		case 5:
			found = searchByPriceRange();
			break;

		case 6:
			found = searchBySupplier();
			break;

		case 7:
			return;

		case 8:
			exitProgram();
			break;

		}
		pauseScreen("Press Enter to return...");
		clearScreen();
	}
}
//Search Menu
void Inventory::displaySearchMenu() const
{
	std::cout << "=======================================================================\n";
	std::cout << "Search Product Menu:\n";
	std::cout << "=======================================================================\n";
	std::cout << "1. Search by ID" << std::endl;
	std::cout << "2. Search by Barcode" << std::endl;
	std::cout << "3. Search by Name" << std::endl;
	std::cout << "4. Search by Category" << std::endl;
	std::cout << "5. Search by Price Range" << std::endl;
	std::cout << "6. Search by Supplier" << std::endl;
	std::cout << "7. Return to Main Menu" << std::endl;
	std::cout << "8. Exit" << std::endl;
	std::cout << "=======================================================================\n";
	std::cout << "Please select an option: ";
}
// Search Case function
//ID
bool Inventory::searchByID() const
{
	int id;
	bool found = false;

		while (true)
		{
			std::cout << "Please enter Product ID to search: ";
			if (std::cin >> id && id > 0)
			{
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}

			std::cout << "The Product ID is invalid. Please re-enter a positive integer." << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

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
			std::cout << "Sorry! The product with ID " << id << " is not found. Please try again!" << std::endl;
		}
		return found;
}
//Barcode
bool Inventory::searchByBarcode() const
{
		std::string barcode;
		bool found = false;

		while (true)
		{
			std::cout << "Please enter Product Barcode to search: ";
			std::getline(std::cin, barcode);
			barcode = trim(barcode);

			if (isValidBarcode(barcode))
			{
				break;
			}

			std::cout << "The Product Barcode is invalid. Please re-enter a valid barcode." << std::endl;
		}

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
			std::cout << "Sorry! The product with Barcode " << barcode << " is not found. Please try again!" << std::endl;
		}

		return found;
}
//Name
bool Inventory::searchByName() const
{
		std::string name;
		bool found = false;

		while (true)
		{
			std::cout << "Please enter Product Name to search: ";
			std::getline(std::cin, name);
			name = trim(name);

			if (!name.empty())
				break;

			std::cout << "The Product Name cannot be empty. Please enter a product name." << std::endl;
		}

		std::string keyword = toLowerCase(name);

		for (const auto& product : products)
		{
			if (toLowerCase(product.getName()).find(keyword)
				!= std::string::npos)
			{
				product.display();
				found = true;
			}
		}

		if (!found)
		{
			std::cout << "Sorry! The product with Name " << name << " is not found. Please try again!" << std::endl;
		}

		return found;
}
//Category
bool Inventory::searchByCategory() const
{
		std::string category;
		bool found = false;

		while (true)
		{
			std::cout << "Please enter Product Category to search: ";
			std::getline(std::cin, category);
			category = trim(category);

			if (!category.empty())
				break;

			std::cout << "The Product Category cannot be empty. Please enter a category." << std::endl;
		}

		std::string keyword = toLowerCase(category);

		for (const auto& product : products)
		{
			if (toLowerCase(product.getCategory()).find(keyword)
				!= std::string::npos)
			{
				product.display();
				found = true;
			}
		}

		if (!found)
		{
			std::cout << "Sorry! The product with Category " << category << " is not found. Please try again!" << std::endl;
		}

		return found;
}
//Price Range
bool Inventory::searchByPriceRange() const
{
		double minPrice, maxPrice;
		bool found = false;

		while (true)
		{
			//min price validation
			while (true)
			{
				std::cout << "Please enter the Minimum Price: RM ";

				if (std::cin >> minPrice && isValidPrice(minPrice))
				{
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					break;
				}

				std::cout << "The Price input is invalid. Please re-enter a non-negative number." << std::endl;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}

			//max price validation
			while (true)
			{
				std::cout << "Please enter the Maximum Price: RM ";

				if (std::cin >> maxPrice && isValidPrice(maxPrice))
				{
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					break;
				}

				std::cout << "The Price input is invalid. Please re-enter a non-negative number." << std::endl;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}

			if (minPrice <= maxPrice)
			{
				break;
			}
			std::cout << "The Minimum Price cannot be greater than the Maximum Price. Please try again!" << std::endl;
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

		return found;
}
//Supplier
bool Inventory::searchBySupplier() const
{
		std::string supplier;
		bool found = false;

		while (true)
		{
			std::cout << "Please enter Product Supplier to search: ";
			std::getline(std::cin, supplier);
			supplier = trim(supplier);

			if (!isValidSupplier(supplier))
			{
				std::cout << "The Product Supplier cannot be empty. Please enter a Product Supplier." << std::endl;
				continue;
			}

			break;
		}

		std::string keyword = toLowerCase(supplier);

		for (const auto& product : products)
		{
			if (toLowerCase(product.getSupplier()).find(keyword)
				!= std::string::npos)
			{
				product.display();
				found = true;
			}
		}

		if (!found)
		{
			std::cout << "Sorry! The product with Supplier " << supplier << " is not found. Please try again!" << std::endl;
		}

		return found;
}
//=======================================================================
//Sort function
void Inventory::sortProducts()
{
	clearScreen();
	if (!hasProducts())
	{
		pauseScreen("Please Enter to return...");
		clearScreen();
		return;
	}

	int option;
	std::cout << "=======================================================================\n";
	std::cout << "Sort Products	Menu\n";
	std::cout << "=======================================================================\n";
	std::cout << "1. Sort by ID" << std::endl;
	std::cout << "2. Sort by Name" << std::endl;
	std::cout << "3. Sort by Category" << std::endl;
	std::cout << "4. Sort by Quantity" << std::endl;
	std::cout << "5. Sort by Price" << std::endl;
	std::cout << "6. Sort by Expiry Date" << std::endl;
	std::cout << "7. Return to Main Menu" << std::endl;
	std::cout << "8. Exit" << std::endl;
	std::cout << "=======================================================================\n";
	std::cout << "Please select an option: ";

	option = getMenuOption(1, 8);
	if (option == 7)
	{
		returnProgram();
		return;
	}

	if (option == 8)
	{
		exitProgram();
			return;
	}

	int order;
	std::cout << "Please select the order (1 for Ascending, 2 for Descending): ";
	order = getMenuOption(1, 2);
	bool ascending = (order == 1);

	switch (option)
	{
		//sort by id
		case 1:
		{
			std::cout << "Sorting by ID..." << std::endl;
			std::stable_sort(products.begin(), products.end(), [ascending](const Product& a, const Product& b)
				{
					return ascending ?
						a.getID() < b.getID() : 
						a.getID() > b.getID();
				});
		break;
		}

		//sort by name
		case 2:
		{
			std::cout << "Sorting by Name..." << std::endl;
			std::stable_sort(products.begin(), products.end(), [ascending](const Product& a, const Product& b)
			{
				return ascending ?
					toLowerCase(a.getName()) < toLowerCase(b.getName()) :
					toLowerCase(a.getName()) > toLowerCase(b.getName());
			});
			break;
		}

		//sort by category
		case 3:
		{
			std::cout << "Sorting by Category..." << std::endl;
			std::stable_sort(products.begin(), products.end(), [ascending](const Product& a, const Product& b)
			{
				return ascending ?
					toLowerCase(a.getCategory()) < toLowerCase(b.getCategory()) : 
					toLowerCase(a.getCategory()) > toLowerCase(b.getCategory());
			});
			break;
		}

		//sort by quantity
		case 4:
		{
			std::cout << "Sorting by Quantity..." << std::endl;
			std::stable_sort(products.begin(), products.end(), [ascending](const Product& a, const Product& b)
			{
				return ascending ?
					a.getQuantity() < b.getQuantity() : 
					a.getQuantity() > b.getQuantity();
			});
			break;
		}

		//sort by price
		case 5:
		{
			std::cout << "Sorting by Price..." << std::endl;
			std::stable_sort(products.begin(), products.end(), [ascending](const Product& a, const Product& b)
			{
				return ascending ?
					a.getPrice() < b.getPrice() : 
					a.getPrice() > b.getPrice();
			});
			break;
		}

		//sort by expiry date
		case 6:
		{
			std::cout << "Sorting by Expiry Date..." << std::endl;
			std::stable_sort(products.begin(), products.end(), [ascending](const Product& a, const Product& b)
			{
				bool aEmpty = a.getExpiryDate().empty();
				bool bEmpty = b.getExpiryDate().empty();

				if (aEmpty && bEmpty)
					return false;

				if (aEmpty)
					return false;

				if (bEmpty)
					return true;

				if (ascending)
					return a.getExpiryDate() < b.getExpiryDate();
				else
					return a.getExpiryDate() > b.getExpiryDate();
			});
			break;
		}

		default:
		{
			std::cout << "This is an invalid option. Please try again!" << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			return;
		}
	}
	std::cout << "Congrats! The products sorted successfully!" << std::endl;
	std::cout << "Press Enter to view the sorted product list..." << std::endl;
	std::cin.get();
	clearScreen();
	displayProducts();

	std::cout << "\nPress Enter to return to the Main Menu...";
	std::cin.get();
	clearScreen();
	return;
}
//=======================================================================
//Main Update function
void Inventory::updateProduct()
{
	clearScreen();

	if(!hasProducts())
	{
		pauseScreen("Please Enter to return...");
		clearScreen();
		return;
	}

	int option;
	std::cout << "=======================================================================\n";
	std::cout << "Update Products Menu\n";
	std::cout << "=======================================================================\n";
	std::cout << "1. Update Barcode" << std::endl;
	std::cout << "2. Update Name" << std::endl;
	std::cout << "3. Update Description" << std::endl;
	std::cout << "4. Update Category" << std::endl;
	std::cout << "5. Update Quantity" << std::endl;
	std::cout << "6. Update Price" << std::endl;
	std::cout << "7. Update Supplier" << std::endl;
	std::cout << "8. Update Expiry Date" << std::endl;
	std::cout << "9. Update Manufacture Date" << std::endl;
	std::cout << "10. Return to Main Menu" << std::endl;
	std::cout << "11. Exit" << std::endl;
	std::cout << "=======================================================================\n";
	std::cout << "Please select an option: ";

	option = getMenuOption(1, 11);
	if(option == 10)
	{
		returnProgram();
		return;
	}

	if (option == 11)
	{
		exitProgram();
		return;
	}

	int id;

	while(true)
	{
		std::cout << "Please enter Product ID to update the relevant product: ";
		if(std::cin >> id && id > 0)
		{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		}
		std::cout << "The Product ID is invalid. Please try again!" << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	bool found = false;

	for(auto& product : products)
	{
		if (product.getID() == id)
		{
			found = true;
			std::cout << "\nThe relevant product is found!\n";
			product.display();

			//value update
			switch(option)
			{
				//barcode
				case 1:
				{
					std::string barcode;
					
					while(true)
					{
						barcode = inputString("Please enter a new Product Barcode (9 digits): ");

						if(!isValidBarcode(barcode))
						{
							std::cout << "The Product Barcode format is invalid. Please try again." << std::endl;
							continue;
						}

						if(barcode != product.getBarcode() && isBarcodeExist(barcode))
						{
							std::cout << "Sorry, the Product Barcode entered already exists." << std::endl;
							std::cout << "Please enter a new Product Barcode." << std::endl;
							continue;
						}
						
						if (barcode == product.getBarcode())
						{
							std::cout << "The new Product Barcode is the same as the current Product Barcode." << std::endl;
							continue;
						}

						break;
					}

					product.setBarcode(barcode);
					std::cout << "Congratulations! The Product Barcode is updated successfully!" << std::endl;
					break;
				}

				//name
				case 2:
				{
					std::string name;

					while(true)
					{
						name = inputString("Please enter a new Product Name: ");

						if (!isValidName(name))
						{
							std::cout << "Sorry, the Product Name is invalid." << std::endl;
							std::cout << "Please enter a new Product Name." << std::endl;
							continue;
						}

						if (name == product.getName())
						{
							std::cout << "The new Product Name is the same as the current Product Name." << std::endl;
							continue;
						}

						break;
					}

					product.setName(name);
					std::cout << "Congratulations! The Product Name is updated successfully!" << std::endl;
					break;
				}

				//description
				case 3:
				{
					std::string description;

					while(true)
					{
						description = inputString("Please enter a new Product description: ");

						if (description == product.getDescription())
						{
							std::cout << "The new Product Description is the same as the current Product Description." << std::endl;
							continue;
						}

						break;
					}

					product.setDescription(description);
					std::cout << "Congratulations! The Product Description is updated successfully!" << std::endl;
					break;
				}

				//category
				case 4:
				{
					std::string category;

					while(true)
					{
						category = inputString("Please enter a new Product Category: ");

						if (category == product.getCategory())
						{
							std::cout << "The new Product Category is the same as the current Product Category." << std::endl;
							continue;
						}

						break;
					}

					product.setCategory(category);
					std::cout << "Congratulations! The Product Category is updated successfully!" << std::endl;
					break;
				}

				//quantity
				case 5:
				{
					int quantity;

					while(true)
					{
						quantity = inputPositive("Please enter a new Product Quantity: ");

						if(quantity == product.getQuantity())
						{
							std::cout << "The new Product Quantity is the same as the current Product Quantity." << std::endl;
							continue;
						}

						break;
					}

					product.setQuantity(quantity);
					std::cout << "Congratulations! The Product Quantity is updated successfully!" << std::endl;
					break;
				}

				//price
				case 6:
				{
					double price;

					while(true)
					{
						price = inputPrice("Please enter a new Product Price: RM ");

						if(std::fabs(price - product.getPrice()) < 0.0001)
						{
							std::cout << "The new Product Price is the same as the current Product Price." << std::endl;
							continue;
						}

						break;
					}

					product.setPrice(price);
					std::cout << "Congratulations! The Product Price is updated successfully!" << std::endl;
					break;
				}

				//supplier
				case 7:
				{
					std::string supplier;

					while(true)
					{
						supplier = inputString("Please enter a new Product Supplier name: ");

						if (!isValidSupplier(supplier))
						{
							std::cout << "Sorry, the Product Supplier is invalid." << std::endl;
							std::cout << "Please enter a new Product Supplier." << std::endl;
							continue;
						}


						if(supplier == product.getSupplier())
						{
							std::cout << "The new Product Supplier is the same as the current Product Supplier." << std::endl;
							continue;
						}

						break;
					}

					product.setSupplier(supplier);
					std::cout << "Congratulations! The Product Supplier is updated successfully!" << std::endl;
					break;
				}

				//expiry date
				case 8:
				{
					std::string expiryDate;

					while (true)
					{
						expiryDate = inputDate("Please enter a new Product Expiry Date (YYYY-MM-DD): ");

						if (expiryDate == product.getExpiryDate())
						{
							std::cout << "The new Product Expiry Date is the same as the current Product Expiry Date." << std::endl;
							continue;
						}

						if (!expiryDate.empty() && !product.getManufactureDate().empty()
							&& expiryDate < product.getManufactureDate())
						{
							std::cout << "The Product Expiry Date cannot be earlier than the Product Manufacture Date." << std::endl;
							continue;
						}

						break;
					}

					product.setExpiryDate(expiryDate);
					std::cout << "Congratulations! The Product Expiry Date is updated successfully!" << std::endl;
					break;
				}

				//manufacture date
				case 9:
				{
					std::string manufactureDate;

					while(true)
					{
						manufactureDate = inputDate("Enter a new Product Manufacture Date(YYYY-MM-DD): ");

						if (manufactureDate == product.getManufactureDate())
						{
							std::cout << "The new Product Manufacture Date is the same as the current Product Manufacture Date." << std::endl;
							continue;
						}

						if (!manufactureDate.empty() && !product.getExpiryDate().empty() 
							&& manufactureDate > product.getExpiryDate())
						{
							std::cout
								<< "The Product Manufacture Date cannot be later than the Product Expiry Date." << std::endl;
							continue;
						}

						break;
					}

					product.setManufactureDate(manufactureDate);
					std::cout << "Congratulations! The Product Manufacture Date is updated successfully!" << std::endl;
					break;
				}

				default:
				{
					std::cout << "This is an invalid option. Please try again!" << std::endl;
					std::cout << "Thank you for your understanding." << std::endl;
					return;
				}
			}
			break;
		}
	}

	if (!found)
	{
		std::cout << "Sorry, the product with the ID " << id << " is not found. Please check again and re-enter." << std::endl;
		std::cout << "Thank you for your understanding." << std::endl;
	}

	pauseScreen("Press Enter to return...");
	clearScreen();
}
//Update Case function
//String Input(Barcode, Name, Description, Category)
std::string Inventory::inputString(const std::string& prompt)
{
	std::string input;

	while (true)
	{
		std::cout << prompt;
		std::getline(std::cin, input);
		input = trim(input);

		if (!input.empty())
			return input;

		std::cout << "Sorry, the input cannot be empty. Please try again." << std::endl;
	}
}
//Positive Input(Quantity)
int Inventory::inputPositive(const std::string& prompt)
{
	int value;

	while (true)
	{
		std::cout << prompt;

		if (std::cin >> value && isValidQuantity(value))
		{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return value;
		}
		std::cout << "The value is invalid. Please try again." << std::endl;

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}
//Price Input
double Inventory::inputPrice(const std::string& prompt)
{
	double price;

	while (true)
	{
		std::cout << prompt;

		if (std::cin >> price && isValidPrice(price))
		{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return price;
		}
		std::cout << "The price is invalid. Please try again." << std::endl;

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}
//Date Input(Expiry / Manufacture Date)
std::string Inventory::inputDate(const std::string& prompt)
{
	std::string date;

	while (true)
	{
		std::cout << prompt;
		std::getline(std::cin, date);
		date = trim(date);

		if (isValidDate(date))
			return date;
		std::cout << "The date is invalid. Please try again." << std::endl;
	}
}
//=======================================================================
//Delete function
void Inventory::deleteProduct()
{
	clearScreen();

	if (!hasProducts())
	{
		pauseScreen("Please Enter to return...");
		clearScreen();
		return;
	}

	int id;

	while (true)
	{
		std::cout << "=======================================================================\n";
		std::cout << "Delete Products Menu\n";
		std::cout << "=======================================================================\n";
		std::cout << "Please enter the Product ID to delete: ";

		if (std::cin >> id && id > 0)
		{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		}

		std::cout << "The Product ID is invalid. Please try again." << std::endl;
		
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	for (auto it = products.begin(); it != products.end(); ++it)
	{
		if ((*it).getID() == id)
		{
			std::cout << "\nThe product with the ID is found:\n";
			(*it).display();

			char confirm;

			while (true)
			{
				std::cout << "Are you sure you want to delete this product? (Y/N): ";
				std::cin >> confirm;

				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				confirm = std::tolower(static_cast<unsigned char>(confirm));

				if (confirm == 'y' || confirm == 'n')
				{
					break;
				}
				std::cout << "The input is invalid. Please enter Y or N." << std::endl;
			}

			if (confirm == 'y')
			{
				products.erase(it);

				std::cout << "\nCongratulations! The product is deleted successfully." << std::endl;
			}

			else
			{
				std::cout << "\nOh no, the delete operation is cancelled. Please try again if needed." << std::endl;
			}

			pauseScreen("Press Enter to return...");
			clearScreen();
			return;
		}
	}

	std::cout << "Sorry! The Product ID " << id << " is not found." << std::endl;

	pauseScreen("Press Enter to return...");
	clearScreen();
}
//=======================================================================
//Status Checking function
void Inventory::checkProductStatus()
{
	clearScreen();

	if (!hasProducts())
	{
		pauseScreen("Please Enter to return...");
		clearScreen();
		return;
	}

	std::cout << "=======================================================================\n";
	std::cout << "Product Status Monitoring Menu\n";
	std::cout << "=======================================================================\n";

	for (const auto& product : products)
	{
		std::cout << "Product ID: " << product.getID() << std::endl;
		std::cout << "Product Name: " << product.getName() << std::endl;
		std::cout << "Product Quantity: " << product.getQuantity() << std::endl;

		if (product.getQuantity() == 0)
		{
			std::cout << "Product Status: Out of Stock" << std::endl;
		}

		else if (product.getQuantity() <= LOW_STOCK_THRESHOLD)
		{
			std::cout << "Product Status: Low Stock" << std::endl;
		}

		else
		{
			std::cout << "Product Status: In Stock" << std::endl;
		}

		std::cout << "=======================================================================\n";
	}

	pauseScreen("Press Enter to return...");
	clearScreen();
}
