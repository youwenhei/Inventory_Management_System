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

	bool isBarcodeExist(const std::string& barcode) const;
	bool hasProducts() const;
	int getMenuOption(int min, int max) const;

	void addProduct(); //function overloading
	void addProduct(const Product& product);

	void displayProducts() const;

	void searchProduct() const;
	void searchProductByID() const;
	void searchProductByBarcode() const;
	void searchProductByName() const;
	void searchProductByCategory() const;
	void searchBySupplier() const;
	void searchByPriceRange() const;

	void sortProducts();
	void sortByPriceAscending() const;
	void sortByPriceDescending() const;
	void sortByExpiryDate() const;

	void updateProduct();

	void deleteProduct();

	void checkProductStatus();
	static const int LOW_STOCK_THRESHOLD = 10;
};

#endif
