#pragma once
#include <string>

void clearScreen();
void pauseScreen(const std::string& message = "Press Enter to continue...");

int getMenuOption(int min, int max);

void displayTitle(const std::string& title);
