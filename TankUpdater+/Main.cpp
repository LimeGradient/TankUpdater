#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <cpr/cpr.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define PATH "\\C:\\Users\\Kevin Burns\\Documents\\VERSION.tanks"
#elif __APPLE__ || __linux__
    #define PATH "/Users/kevinburns/Documents/TankUpdater/TankUpdater+/VERSION.tanks"
#endif

std::string latestVersion;
char *version;

void GetLatestVersion() {
	cpr::Response r = cpr::Get(cpr::Url{"https://tank-version-manager.herokuapp.com"});
	std::cout << r.text << std::endl;
	latestVersion = r.text;
}

bool isLatestVersion() {
	using namespace std;

	string content;
	ifstream file(PATH);
	GetLatestVersion();
	getline(file, content);
	if (content != latestVersion) {
		return false;
	}
	return true;
	file.close();
}

int main() {
	if (isLatestVersion()) {
		std::cout << "Latest Version Installed" << std::endl;
		return 0;
	} else {
		std::cout << "Needs Update" << std::endl;
		return -1;
	}
	return -1;
}