#include "Utility.h"
#include <iostream>
#include <cstdlib>

void clearScreen()
{
	system("cls");
}

void pauseScreen(const std::string& message)
{
	std::cout << '\n' << message;
	std::cin.get();
}
