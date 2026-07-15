#include <jdbc/cppconn/exception.h>
#include <iostream>
#include "Database.h"

Database::Database() 
{
	try
	{
		driver = sql::mysql::get_mysql_driver_instance();

	con = driver->connect(
		"tcp://127.0.0.1:3306",
		"root",
		"YOUR_PASSWORD"
	);

	con->setSchema("inventory_db");
	}

	catch (sql::SQLException& e)
	{
		std::cout << "Database Error: " << e.what() << std::endl;
	}
}

Database::~Database() 
{
	delete con;
}

sql::Connection* Database::getConnection()
{
	return con;
}
