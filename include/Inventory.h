#ifndef INVENTORY_H
#define INVENTORY_H

#pragma once
#include <vector>
#include "Product.h"

class Inventory
{
private:
	std::vector<Product> products; // Vector to store the list for products dynamically

public:
	void addProduct(const Product& product);
	void displayProducts() const;
};

#endif
