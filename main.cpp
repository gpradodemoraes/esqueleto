#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>

std::string getHiddenInput() {
	std::string input;
	char ch;

	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	SetConsoleMode(hStdin, mode & ~ENABLE_ECHO_INPUT);

	while ((ch = _getch()) != '\r') { // '\r' = Enter key on Windows
		if (ch == '\b') {			  // Handle backspace
			if (!input.empty()) {
				input.pop_back();
				std::cout << "\b \b";
			}
		} else {
			input += ch;
			std::cout << '*'; // Print * instead of the character
		}
	}

	SetConsoleMode(hStdin, mode); // Restore original console mode
	std::cout << std::endl;
	return input;
}

int main() {
	std::string username, password;

	std::cout << "Username: ";
	std::cin >> username;

	std::cout << "Password: ";
	password = getHiddenInput();

	std::cout << "\nLogged in as: " << username << "(" << password << ")" << std::endl;

	return 0;
}
