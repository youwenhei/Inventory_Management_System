#include <iostream>
#include <jdbc/cppconn/exception.h>
#include "Database.h"

Database::Database() 
{
	driver = nullptr;
	con = nullptr;

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
		std::cout << "Database Error: " << e.what() 
				  << "\nError Code: " << e.getErrorCode()
			      << "\nSQL State: " << e.getSQLState()
				  << std::endl;
	}
}

Database::~Database() 
{
	delete con;

	con = nullptr;
	driver = nullptr;
}

sql::Connection* Database::getConnection() const
{
	return con;
}
