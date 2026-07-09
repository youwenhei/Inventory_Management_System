#ifndef INVENTORY_H
#define INVENTORY_H

#pragma once
#include <vector>
#include "Product.h"

class Inventory
{
private:
	std::vector<Product> products; // Vector to store the list for products dynamically
	int newProductID; // Initialize the product ID counter

public:
	Inventory();
	void addProduct(); //function overloading
	void addProduct(const Product& product);

	void displayProducts() const;
	void searchProduct() const;

	void sortByPriceAscending();
	void sortByPriceDescending();
	void sortByExpiryDate();

	bool isBarcodeExist(const std::string& barcode) const;
};

#endif
