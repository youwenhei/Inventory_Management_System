//include guards
//#ifndef PRODUCT_H
//#define PRODUCT_H

#pragma once
#include <string>

class Product
{
private: //cannot be edited
	//product attributes
  int product_ID;
  int product_Barcode;
  string product_Name;
  string product_Description;
  string product_Category;

	//product stock and price
  int product_Quantity;
  double product_Price;

	//product supplier and dates
  string product_Supplier;
  string product_ExpiryDate;
  string product_ManufactureDate;


public: //product creation
	Product(int product_ID, int product_Barcode, string product_Name, string product_Description, string product_Category, 
			int product_Quantity, double product_Price, 
			string product_Supplier, string product_ExpiryDate, string product_ManufactureDate);

  //system function
  void display();

#endif
