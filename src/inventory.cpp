#include <iostream>
#include <cctype>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <cmath>
#include <string>
#include <memory>
#include <iomanip>
#include <ostream>
#include <fstream>
#include <jdbc/cppconn/connection.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/datatype.h>
#include "Inventory.h"
#include "Utility.h"
#include "Database.h"

Inventory::Inventory()
{
	newProductID = 1;
	loadProducts();
}
//=======================================================================
//Date validation
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
//Barcode validation
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
//Name validation
bool isValidName(const std::string& name)
{
	if (name.empty())
	{
		return false;
	}

for (char c : name)
{
	if (!std::isalnum(static_cast<unsigned char>(c))
		&& c != ' '
		&& c != '&'
		&& c != '-'
		&& c != '\''
		&& c != '.'
		&& c != '/'
		&& c != '+'
		&& c != '('
		&& c != ')'
		&& c != ',')
	{
		return false;
	}
}
return true;
}
//=======================================================================
//Category validation
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
//Quantity validation
bool isValidQuantity(int quantity)
{
	return quantity >= 0;
}
//=======================================================================
//Price validation
bool isValidPrice(double price)
{
	return std::isfinite(price) && price >= 0;
}
//=======================================================================
//Supplier validation
bool isValidSupplier(const std::string& supplier)
{
	if (supplier.empty())
	{
		return false;
	}

	for (char c : supplier)
	{
		if (!std::isalnum(static_cast<unsigned char>(c))
			&& c != ' '
			&& c != '&'
			&& c != '-'
			&& c != '\''
			&& c != '.'
			&& c != '/'
			&& c != '+'
			&& c != '('
			&& c != ')'
			&& c != ',')
		{
			return false;
		}
	}
	return true;
}
//=======================================================================
//RFID validation
bool isValidRFID(const std::string& rfid)
{
	if (rfid.length() != 8)
		return false;

	for (char c : rfid)
	{
		if (!std::isxdigit(static_cast<unsigned char>(c)))
			return false;
	}

	return true;
}
//=======================================================================
//RFID uniqueness
bool Inventory::isRFIDExist(const std::string& rfid) const
{
	auto toUpper = [](std::string& str)
	{
		std::transform(
			str.begin(),
			str.end(),
			str.begin(),
			[](unsigned char c)
			{
				return std::toupper(c);
			}
		);
	};

	std::string inputRFID = rfid;
	toUpper(inputRFID);

	for (const auto& product : products)
	{
		std::string existingRFID = product.getRFID();
		toUpper(existingRFID);

		if (existingRFID == inputRFID)
		{
			return true;
		}
	}

	return false;
}
//=======================================================================
//Check for the existence of barcode
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
//Change case
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
//Check if there is product
bool Inventory::hasProducts() const
{
	return !products.empty();
}
//=======================================================================
//Display product function
void Inventory::displayProducts() const
{
	if (products.empty())
	{
		std::cout << "No products found." << std::endl;
		return;
	}

	const int PAGE_SIZE = 10;
	int totalPages = static_cast<int>(products.size() + PAGE_SIZE - 1) / PAGE_SIZE;
	int currentPage = 0;

	while (currentPage < totalPages)
	{
		clearScreen();

		int start = currentPage * PAGE_SIZE;
		int end = std::min(start + PAGE_SIZE, (int)products.size());

		for (int i = start; i < end; i++)
		{
			products[i].display();
		}

		currentPage++;

		if (currentPage < totalPages)
		{
			std::cout << "\nPress Enter for next page...";
			std::cin.get();
		}
	}
}
//=======================================================================
//Exit program
void exitProgram()
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
//=======================================================================
//Return program
void returnProgram()
{
	std::cout << "You are now returning to main menu..." << std::endl;
	return;
}
//=======================================================================
//Save program
bool Inventory::insertProductToDatabase(const Product&  product)
{

	try
	{
		auto con = db.getConnection();

		if (con == nullptr)
		{
			return false;
		}

		con->setAutoCommit(false);

		std::unique_ptr<sql::PreparedStatement> pstmt(
			con->prepareStatement(
				"INSERT INTO products "
				"(product_ID, barcode, name, description, category, quantity, price, supplier, expiry_date, manufacture_date, rfid_uid) "
				"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"
			)
		);

		pstmt->setInt(1, product.getID());
		pstmt->setString(2, product.getBarcode());
		pstmt->setString(3, product.getName());
		pstmt->setString(4, product.getDescription());
		pstmt->setString(5, product.getCategory());
		pstmt->setInt(6, product.getQuantity());
		pstmt->setDouble(7, product.getPrice());
		pstmt->setString(8, product.getSupplier());
			
		if (product.getExpiryDate().empty())
		{
			pstmt->setNull(9, sql::DataType::DATE);
		}
		else
		{
			pstmt->setString(9, product.getExpiryDate());
		}

		if (product.getManufactureDate().empty())
		{
			pstmt->setNull(10, sql::DataType::DATE);
		}
		else
		{
			pstmt->setString(10, product.getManufactureDate());
		}

		if (product.getRFID().empty())
		{
			pstmt->setNull(11, sql::DataType::VARCHAR);
		}
		else
		{
			pstmt->setString(11, product.getRFID());
		}

		int rows = pstmt->executeUpdate();

		if (rows > 0)
		{
			con->commit();
			con->setAutoCommit(true);
			return true;
		}
		else
		{
			con->rollback();
			con->setAutoCommit(true);
			return false;
		}
	}

	catch (sql::SQLException& e)
	{
		try
		{
			auto con = db.getConnection();
			con->rollback();
			con->setAutoCommit(true);
		}

		catch (sql::SQLException& e)
		{

		}

		std::cout << "Database Error: " << e.what() 
				  << "\nError Code: " << e.getErrorCode() 
			      << "\nSQL State: " << e.getSQLState()
			      << std::endl;
		return false;
	}
}
//=======================================================================
//Load program
bool Inventory::loadProducts()
{
	products.clear();
	products.reserve(100);

	try 
	{
		auto con = db.getConnection();

		std::unique_ptr<sql::PreparedStatement> pstmt(
			con->prepareStatement(
				"SELECT product_ID, barcode, rfid_uid, name, description, category, quantity, price, supplier, expiry_date, manufacture_date "
				"FROM products"
			)
		);

		std::unique_ptr<sql::ResultSet> res(
			pstmt->executeQuery()
		);

		int maxID = 0;

		while (res->next())
		{
			int id = res->getInt("product_ID");

			std::string barcode = res->getString("barcode");

			std::string rfid;
			if (!res->isNull("rfid_uid"))
			{
				rfid = res->getString("rfid_uid");
			}

			std::string name = res->getString("name");
			std::string description = res->getString("description");
			std::string category = res->getString("category");

			int quantity = res->getInt("quantity");
			double price = res->getDouble("price");

			std::string supplier = res->getString("supplier");

			std::string expiryDate;
			std::string manufactureDate;

			if (!res->isNull("expiry_date"))
			{
				expiryDate = res->getString("expiry_date");
			}

			if (!res->isNull("manufacture_date"))
			{
				manufactureDate = res->getString("manufacture_date");
			}

			Product product(
				id,
				barcode,
				rfid,
				name,
				description,
				category,
				quantity,
				price,
				supplier,
				expiryDate,
				manufactureDate
			);

			products.push_back(product);

			if (id > maxID)
			{
				maxID = id;
			}
		}

		newProductID = maxID + 1;

		return true;
	}

	catch (sql::SQLException& e)
	{
		std::cout << "Database Error: " << e.what()
			<< "\nError Code: " << e.getErrorCode()
			<< "\nSQL State: " << e.getSQLState()
			<< std::endl;
		return false;
	}
}

//=======================================================================
//Functions in Main Menu
//=======================================================================

//Main Add function
void Inventory::addProduct()
{
	int quantity;
	double price;
	std::string barcode, rfid, name, description, category, supplier, expiryDate, manufactureDate;

	displayTitle("Add Product Menu");


	//barcode validation
	while (true)
	{
		std::cout << "Please enter the Product Barcode (9 digits): ";
		std::getline(std::cin, barcode);
		barcode = trim(barcode);

		if (!isValidBarcode(barcode))
		{
			std::cout << "The Product Barcode format is invalid. Please re-enter the Product Barcode with 9 digits." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			continue;
		}

		else if (isBarcodeExist(barcode))
		{
			std::cout << "The Product Barcode is already in use. Please enter a unique Product Barcode." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			continue;
		}
		break;
	}

	//name validation
	while (true)
	{
		std::cout << "Please enter the Product Name: ";
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
		std::cout << "Please enter the Product Description: ";
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
		std::cout << "Please enter the Product Category: ";
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
		std::cout << "Please enter the Product Quantity: ";
		if (!(std::cin >> quantity) || !isValidQuantity(quantity))
		{
			std::cout << "The Product Quantity format is invalid. Please re-enter a valid Product Quantity." << std::endl;
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
		std::cout << "Please enter the Product Price: RM ";
		if (!(std::cin >> price) || !isValidPrice(price))
		{
			std::cout << "The Product Price format is invalid. Please re-enter the Product Price with a non-negative number." << std::endl;
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
		std::cout << "Please enter the Product Supplier: ";
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
		std::cout << "Please enter the Product Expiry Date (YYYY-MM-DD, press Enter if none): ";
		std::getline(std::cin, expiryDate);
		expiryDate = trim(expiryDate);

		if (!isValidDate(expiryDate))
		{
			std::cout << "The Product Expiry Date format is invalid. Please re-enter the Product Expiry Date in YYYY-MM-DD format." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			continue;
		}
		break;
	}

	while (true)
	{
		//manufacture date validation
		std::cout << "Please enter the Product Manufacture Date (YYYY-MM-DD, press Enter if none): ";
		std::getline(std::cin, manufactureDate);
		manufactureDate = trim(manufactureDate);

		if (!isValidDate(manufactureDate))
		{
			std::cout << "The Product Manufacture Date format is invalid. Please re-enter the Product Manufacture Date in YYYY-MM-DD format." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			continue;
		}
		break;
	}

	//rfid validation
	while (true)
	{
		std::cout << "Please eneter the Product RFID UID (8 hex digits): ";
		std::getline(std::cin, rfid);
		rfid = trim(rfid);

		if (rfid.empty())
		{
			break;
		}

		if (!isValidRFID(rfid))
		{
			std::cout << "The RFID format is invalid. Please try again!" << std::endl;
			continue;
		}

		if (isRFIDExist(rfid))
		{
			std::cout << "The RFID already exists. Please try again!" << std::endl;
			continue;
		}

		break;
	}

	//check for expiry date and manufacture date
	//YYYY-MM-DD format allow lexicographical comparison (string comparison)
	while (true)
	{
		if (!expiryDate.empty() && !manufactureDate.empty() && expiryDate < manufactureDate)
		{
			std::cout << "The Product Expiry Date cannot be earlier than the Product Manufacture Date. Please re-enter the dates." << std::endl;
			std::cout << "Thank you for your understanding." << std::endl;
			return;
		}

		break;
	}

	Product newProduct(
		newProductID,
		barcode,
		rfid,
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

	try
	{
		if (insertProductToDatabase(newProduct))
		{
			newProductID++;

			std::cout << "=======================================================================\n";
			std::cout << "Congratulations! The product is added successfully!\n";
		}

		else
		{
			products.pop_back();

			std::cout << "=======================================================================\n";
			std::cout << "Sorry! The product is failed to add due to a database error.\n";
			std::cout << "Please try again!\n";
		}
	}

	catch (const std::exception& e)
	{
		products.pop_back();

		std::cout << "Unexpected Error: " << e.what() << std::endl;
	}
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
			clearScreen();
			found = searchByID();
			break;

		case 2:
			clearScreen();
			found = searchByBarcode();
			break;

		case 3:
			clearScreen();
			found = searchByName();
			break;

		case 4:
			clearScreen();
			found = searchByCategory();
			break;

		case 5:
			clearScreen();
			found = searchByPriceRange();
			break;

		case 6:
			clearScreen();
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
	displayTitle("Search Product Menu");
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
	displayTitle("Search Product By ID");
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
		displayTitle("Search Product By Barcode");
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
		displayTitle("Search Product By Name");
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
		displayTitle("Search Product By Category");
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
		displayTitle("Search Product By Price Range");
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
	displayTitle("Search Product By Supplier");
		std::string supplier;
		bool found = false;

		while (true)
		{
			std::cout << "Please enter Product Supplier to search: ";
			std::getline(std::cin, supplier);
			supplier = trim(supplier);

			if (!isValidSupplier(supplier))
			{
				std::cout << "The Product Supplier is invalid. Please enter a Product Supplier." << std::endl;
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
	displayTitle("Sort Products Menu");
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
	displayTitle("Update Product Menu");
	std::cout << "1. Update Barcode" << std::endl;
	std::cout << "2. Update Name" << std::endl;
	std::cout << "3. Update Description" << std::endl;
	std::cout << "4. Update Category" << std::endl;
	std::cout << "5. Update Quantity" << std::endl;
	std::cout << "6. Update Price" << std::endl;
	std::cout << "7. Update Supplier" << std::endl;
	std::cout << "8. Update Expiry Date" << std::endl;
	std::cout << "9. Update Manufacture Date" << std::endl;
	std::cout << "10. Update RFID UID" << std::endl;
	std::cout << "11. Return to Main Menu" << std::endl;
	std::cout << "12. Exit" << std::endl;
	std::cout << "=======================================================================\n";
	std::cout << "Please select an option: ";

	option = getMenuOption(1, 12);
	if(option == 11)
	{
		returnProgram();
		return;
	}

	if (option == 12)
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
			clearScreen();
			std::cout << "\nThe relevant product is found!\n\n";
			product.display();

			pauseScreen("Press Enter to continue...");
			clearScreen();

			//value update
			switch(option)
			{
				//barcode
				case 1:
				{
					displayTitle("Update Product Barcode");

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

					std::string oldBarcode = product.getBarcode();

					product.setBarcode(barcode);


					if (!updateProductInDatabase(product))
					{
						product.setBarcode(oldBarcode);
					}

					break;
				}

				//name
				case 2:
				{
					displayTitle("Update Product Name");

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

					std::string oldName = product.getName();

					product.setName(name);


					if (!updateProductInDatabase(product))
					{
						product.setName(oldName);
					}

					break;
				}

				//description
				case 3:
				{
					displayTitle("Update Product Description");

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

					std::string oldDescription = product.getDescription();

					product.setDescription(description);


					if (!updateProductInDatabase(product))
					{
						product.setDescription(oldDescription);
					}

					break;
				}

				//category
				case 4:
				{
					displayTitle("Update Product Category");

					std::string category;

					while(true)
					{
						category = inputString("Please enter a new Product Category: ");

						if (category == product.getCategory())
						{
							std::cout << "The new Product Category is the same as the current Product Category." << std::endl;
							continue;
						}

						if (!isValidCategory(category))
						{
							std::cout << "The Product Category is invalid. Please try again." << std::endl;
							continue;
						}

						break;
					}

					std::string oldCategory = product.getCategory();

					product.setCategory(category);


					if (!updateProductInDatabase(product))
					{
						product.setCategory(oldCategory);
					}

					break;
				}

				//quantity
				case 5:
				{
					displayTitle("Update Product Quantity");

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

					int oldQuantity = product.getQuantity();

					product.setQuantity(quantity);


					if (!updateProductInDatabase(product))
					{
						product.setQuantity(oldQuantity);
					}

					break;
				}

				//price
				case 6:
				{
					displayTitle("Update Product Price");

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

					double oldPrice = product.getPrice();

					product.setPrice(price);


					if (!updateProductInDatabase(product))
					{
						product.setPrice(oldPrice);
					}

					break;
				}

				//supplier
				case 7:
				{
					displayTitle("Update Product Supplier");

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

					std::string oldSupplier = product.getSupplier();

					product.setSupplier(supplier);


					if (!updateProductInDatabase(product))
					{
						product.setSupplier(oldSupplier);
					}

					break;
				}

				//expiry date
				case 8:
				{
					displayTitle("Update Product Expiry Date");

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

					std::string oldExpiryDate = product.getExpiryDate();

					product.setExpiryDate(expiryDate);


					if (!updateProductInDatabase(product))
					{
						product.setExpiryDate(oldExpiryDate);
					}

					break;
				}

				//manufacture date
				case 9:
				{
					displayTitle("Update Product Manufacture Date");

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

					std::string oldManufactureDate = product.getManufactureDate();

					product.setManufactureDate(manufactureDate);


					if (!updateProductInDatabase(product))
					{
						product.setManufactureDate(oldManufactureDate);
					}

					break;
				}

				//RFID
				case 10:
				{
					displayTitle("Update Product RFID UID");

					std::string newRFID;

					while (true)
					{
						newRFID = inputString("Enter a new RFID UID: ");

						if (newRFID.empty())
						{
							break;
						}

						if (!newRFID.empty() && !isValidRFID(newRFID))
						{
							std::cout << "The RFID format is invalid. Please try again.\n";
							continue;
						}

						if (newRFID != product.getRFID() && isRFIDExist(newRFID))
						{
							std::cout << "The Product RFID UID already exists. Please enter another RFID UID." << std::endl;
							continue;
						}

						if (newRFID == product.getRFID())
						{
							std::cout << "The new Product RFID UID is the same as the current Product RFID UID." << std::endl;
							continue;
						}

						break;
					}

					std::string oldRFID = product.getRFID();

					product.setRFID(newRFID);


					if (!updateProductInDatabase(product))
					{
						product.setRFID(oldRFID);
					}
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
		displayTitle("Delete Product Menu");
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
				if (deleteProductFromDatabase(id))
				{
					products.erase(it);

					std::cout << "\nCongratulations! The product is deleted successfully." << std::endl;
				}
				else
				{
					std::cout << "\nFailed to delete product from database." << std::endl;
				}
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

	displayTitle("Product Status Monitoring Menu");

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

//=======================================================================
//Database
//=======================================================================

//Update database
bool Inventory::updateProductInDatabase(const Product& product) const
{
	try
	{
		auto con = db.getConnection();

		if (con == nullptr)
		{
			return false;
		}

		con->setAutoCommit(false);

		std::unique_ptr<sql::PreparedStatement> pstmt(
			con->prepareStatement(
				"UPDATE products "
				"SET barcode=?, "
				"name=?, "
				"description=?, "
				"category=?, "
				"quantity=?, "
				"price=?, "
				"supplier=?, "
				"expiry_date=?, "
				"manufacture_date=?, "
				"rfid_uid=? "
				"WHERE product_ID=?"
			)
		);

		pstmt->setString(1, product.getBarcode());
		pstmt->setString(2, product.getName());
		pstmt->setString(3, product.getDescription());
		pstmt->setString(4, product.getCategory());
		pstmt->setInt(5, product.getQuantity());
		pstmt->setDouble(6, product.getPrice());
		pstmt->setString(7, product.getSupplier());

		if (product.getExpiryDate().empty())
		{
			pstmt->setNull(8, sql::DataType::DATE);
		}
		else
		{
			pstmt->setString(8, product.getExpiryDate());
		}

		if (product.getManufactureDate().empty())
		{
			pstmt->setNull(9, sql::DataType::DATE);
		}
		else
		{
			pstmt->setString(9, product.getManufactureDate());
		}

		if (product.getRFID().empty())
		{
			pstmt->setNull(10, sql::DataType::VARCHAR);
		}
		else
		{
			pstmt->setString(10, product.getRFID());
		}

		pstmt->setInt(11, product.getID());

		int rows = pstmt->executeUpdate();

		if (rows > 0)
		{
			con->commit();
			con->setAutoCommit(true);
			return true;
		}
		else
		{
			con->rollback();
			con->setAutoCommit(true);
			return false;
		}
	}

	catch (sql::SQLException& e)
	{
		try
		{
			auto con = db.getConnection();
			con->rollback();
			con->setAutoCommit(true);
		}

		catch (sql::SQLException& e)
		{

		}

		std::cout << "Database Error: " << e.what()
			      << "\nError Code: " << e.getErrorCode()
			      << "\nSQL State: " << e.getSQLState() 
			      << std::endl;
		return false;
	}
}
//=======================================================================
//Delete database
bool Inventory::deleteProductFromDatabase(int productID)
{
	try
	{
		auto con = db.getConnection();

		if (con == nullptr)
		{
			return false;
		}

		con->setAutoCommit(false);

		std::unique_ptr<sql::PreparedStatement> pstmt(
			con->prepareStatement(
				"DELETE FROM products WHERE product_ID=?"
			)
		);

		pstmt->setInt(1, productID);

		int rows = pstmt->executeUpdate();

		if (rows > 0)
		{
			con->commit();
			con->setAutoCommit(true);
			return true;
		}
		else
		{
			con->rollback();
			con->setAutoCommit(true);
			return false;
		}
	}

	catch (sql::SQLException& e)
	{
		try
		{
			auto con = db.getConnection();
			con->rollback();
			con->setAutoCommit(true);
		}

		catch (sql::SQLException& e)
		{

		}

		std::cout << "Database Error: " << e.what()
			      << "\nError Code: " << e.getErrorCode()
			      << "\nSQL State: " << e.getSQLState()
			      << std::endl;
		return false;
	}
}

//=======================================================================
//Additional Function
//=======================================================================

//Refresh product
void Inventory::refreshProducts()
{
	clearScreen();
	displayTitle("Refresh Product List");

	if (loadProducts())
	{
		std::cout << "Refresh success.\n";
	}
	else
	{
		std::cout << "Refresh failed.\n";
	}

	pauseScreen("Press Enter to return...");
}
//=======================================================================
//Display dashboard
void Inventory::displayDashboard() const
{
	clearScreen();

	if (products.empty())
	{
		displayTitle("Smart Inventory Dashboard");
		std::cout << "No products found.\n";
		pauseScreen("Press Enter to return...");
		return;
	}

	int expiryCount = 0;
	int noRFIDCount = 0;
	int expiredCount = 0;
	int lowStockCount = 0;
	int outOfStockCount = 0;
	int totalQuantity = 0;
	double totalValue = 0;
	double maxPrice = products[0].getPrice();
	double minPrice = products[0].getPrice();

	std::string today = getCurrentDate();

	for (const auto& product : products)
	{
		totalQuantity += product.getQuantity();
		totalValue += product.getPrice() * product.getQuantity();
		if (product.getPrice() > maxPrice)
			maxPrice = product.getPrice();
		if (product.getPrice() < minPrice)
			minPrice = product.getPrice();

		if (!product.getExpiryDate().empty())
		{
			expiryCount++;

			if (product.getExpiryDate() < today)
			{
				expiredCount++;
			}
		}

		if (product.getRFID().empty())
		{
			noRFIDCount++;
		}

		if (product.getQuantity() == 0)
		{
			outOfStockCount++;
		}
		else if (product.getQuantity() <= LOW_STOCK_THRESHOLD)
		{
			lowStockCount++;
		}

	}

	double rfidRate = ((double)(products.size() - noRFIDCount)
						/ products.size()) * 100;
	double averagePrice = 0;

	if (totalQuantity > 0)
	{
		averagePrice = totalValue / totalQuantity;
	}

	displayTitle("Inventory Dashboard");
	std::cout << std::left;
	std::cout << std::setw(35) << "Total Products" << ": " << products.size() << std::endl;
	std::cout << std::setw(35) << "Total Quantity" << ": " << totalQuantity << std::endl;
	std::cout << std::setw(35) << "Inventory Value" << ": RM " << std::fixed << std::setprecision(2) << totalValue << std::endl;

	displayTitle("Inventory Monitoring");
	std::cout << std::setw(35) << "Low Stock Products" << ": " << lowStockCount << std::endl;
	std::cout << std::setw(35) << "Out of Stock Products" << ": " << outOfStockCount << std::endl;
	std::cout << std::setw(35) << "Products with Expiry Date" << ": " << expiryCount << std::endl;
	std::cout << std::setw(35) << "Expired Products" << ": " << expiredCount << std::endl;

	displayTitle("RFID Monitoring");
	std::cout << std::setw(35) << "Products without RFID" << ": " << noRFIDCount << std::endl;
	std::cout << std::setw(35) << "RFID Registration Rate" << ": " << std::fixed << std::setprecision(1) << rfidRate << "%" << std::endl;

	displayTitle("Price Analysis");
	std::cout << std::setw(35) << "Average Product Price" << ": RM " << averagePrice << std::endl;
	std::cout << std::setw(35) << "Highest Product Price" << ": RM " << maxPrice << std::endl;
	std::cout << std::setw(35) << "Lowest Product Price" << ": RM " << minPrice << std::endl;

	displayTitle("System");
	std::cout << std::setw(35) << "Database Status" << ":"
		      << (db.getConnection() != nullptr ? "Connected" : "Disconnected")
		      << std::endl;

	std::cout << std::setw(35) << "RFID Module" << ": Not Connected"
		      << std::endl;

	std::cout << std::setw(35) << "ESP32 Status" << ": Not Connected"
		      << std::endl;

	std::cout << "=======================================================================\n";

	pauseScreen("Press Enter to return...");
}
//=======================================================================
//Export report menu
void Inventory::exportReport()
{
	clearScreen();

	displayTitle("Export Report");

	if (exportCSV())
	{
		std::cout << "Congratulations! The CSV report exported successfully.\n";
	}
	else
	{
		std::cout << "Sorry, the CSV export failed. Please try again.\n";
	}

	if (exportTXT())
	{
		std::cout << "Congratulations! The TXT report exported successfully.\n";
	}
	else
	{
		std::cout << "Sorry, the TXT export failed. Please try again.\n";
	}

	pauseScreen("Press Enter to return...");
}
//=======================================================================
//Export CSV
bool Inventory::exportCSV() const
{
	std::ofstream file("inventory_report.csv");

	if (!file)
	{
		return false;
	}

	file << "ID," << "Barcode," << "RFID," << "Name," << "Category,"
		<< "Description," << "Quantity," << "Price," << "Supplier,"
		<< "Expiry Date," << "Manufacture Date\n";

	for (const auto& product : products)
	{
		file << product.getID() << ","
			 << product.getBarcode() << ","
			 << product.getRFID() << ","
			 << product.getName() << ","
			 << product.getCategory() << ","
			 << product.getDescription() << ","
			 << product.getQuantity() << ","
			 << product.getPrice() << ","
			 << product.getSupplier() << ","
			 << product.getExpiryDate() << ","
			 << product.getManufactureDate() << ","
			 << '\n';
	}

	return true;
}
//Export TXT
bool Inventory::exportTXT() const
{
	std::ofstream file("inventory_report.txt");

	if (!file)
	{
		return false;
	}

	displayTitle("Smart Inventory Report");

	for (const auto& product : products)
	{
		file << "Product ID       : " << product.getID() << "\n";
		file << "Barcode          : " << product.getBarcode() << "\n";
		file << "RFID UID         : ";

		if (product.getRFID().empty())
			file << "Not Assigned\n";
		else
			file << product.getRFID() << "\n";

		file << "Product Name     : " << product.getName() << "\n";
		file << "Description      : " << product.getDescription() << "\n";
		file << "Category         : " << product.getCategory() << "\n";
		file << "Quantity         : " << product.getQuantity() << "\n";
		file << "Price            : RM "
			<< std::fixed << std::setprecision(2)
			<< product.getPrice() << "\n";

		file << "Supplier         : " << product.getSupplier() << "\n";

		file << "Expiry Date      : ";

		if (product.getExpiryDate().empty())
			file << "N/A\n";
		else
			file << product.getExpiryDate() << "\n";


		file << "Manufacture Date : ";

		if (product.getManufactureDate().empty())
			file << "N/A\n";
		else
			file << product.getManufactureDate() << "\n";


		file << "--------------------------------------------\n\n";
	}

	displayTitle("End of Inventory Report");

	return true;
}
