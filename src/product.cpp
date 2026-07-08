#include "Product.h"
#include <iostream>
#include <utility>

Product::Product(int product_ID, std::string product_Barcode, std::string product_Name, std::string product_Description, std::string product_Category,
    int product_Quantity, double product_Price,
    std::string product_Supplier, std::string product_ExpiryDate, std::string product_ManufactureDate)

    : product_ID(product_ID),
    product_Barcode(product_Barcode),
    product_Name(std::move(product_Name)),
    product_Description(std::move(product_Description)),
    product_Category(std::move(product_Category)),
    product_Quantity(product_Quantity),
    product_Price(product_Price),
    product_Supplier(std::move(product_Supplier)),
    product_ExpiryDate(std::move(product_ExpiryDate)),
    product_ManufactureDate(std::move(product_ManufactureDate))

{
}

void Product::display() const
{
    std::cout << "====================================================\n";
    std::cout << "Product Details:\n";
    std::cout << "====================================================\n";
    std::cout << "Product ID: " << product_ID << "\n"
        << "Product Barcode: " << product_Barcode << "\n"
        << "Product Name: " << product_Name << "\n"
        << "Product Description: " << product_Description << "\n"
        << "Product Category: " << product_Category << "\n"
        << "Product Quantity: " << product_Quantity << "\n"
        << "Product Price: RM " << product_Price << "\n"
        << "Product Supplier: " << product_Supplier << "\n"
        << "Product Expiry Date: " << product_ExpiryDate << "\n"
        << "Product Manufacture Date: " << product_ManufactureDate << "\n";
    std::cout << "====================================================\n";
}

int Product::getID() const
{
	return product_ID;
}

std::string Product::getBarcode() const
{
	return product_Barcode;
}
