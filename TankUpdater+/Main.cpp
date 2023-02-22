#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include "Server.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define PATH "\\C:\\Users\\Kevin Burns\\Documents\\VERSION.tanks"

#elif __APPLE__ || __linux__
    #define PATH "Users/kevinburns/Documents/VERSION.tanks"
	#include <arpa/inet.h>
	#include <sys/socket.h>
	#include <unistd.h>
#endif

std::string version = "1.0";

bool isLatestVersion(std::string latestVersion) {
	using namespace std;

	string content;
	ifstream file(PATH);
	while (getline(file, content)) {
		if (content != latestVersion) {
			return false;
		}
	}
	file.close();
	return true;
}

int main() {
	if (isLatestVersion("1.0")) {
		UnixServer us;
		us.StartClient();
	} else {
		std::cout << "Needs Update" << std::endl;
		return -1;
	}
	return -1;
}