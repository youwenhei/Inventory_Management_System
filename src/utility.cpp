#include "Utility.h"
#include <iostream>
#include <cstdlib>
#include <limits>

void clearScreen()
{
	system("cls");
}

void pauseScreen(const std::string& message)
{
	std::cout << '\n' << message;
	std::cin.get();
}

int getMenuOption(int min, int max)
{
	int option;

	while (true)
	{
		if (std::cin >> option)
		{
			if (option >= min && option <= max)
			{
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				return option;
			}
		}

		std::cout << "The input is invalid. Please try again." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

void displayTitle(const std::string& title)
{
	std::cout << "=======================================================================\n";
	std::cout << title << std::endl;
	std::cout << "=======================================================================\n";
}
