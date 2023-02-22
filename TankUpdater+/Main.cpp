#include <iostream>
#include <fstream>
#include <string>

std::string version = "1.0";

bool isLatestVersion(std::string latestVersion) {
	using namespace std;

	string content;
	ifstream file("\\C:\\Users\\Kevin Burns\\Documents\\VERSION.tanks");
	while (getline(file, content)) {
		if (content == latestVersion) {
			return true;
		}
		else {
			return false;
		}
	}
	file.close();
}

int main() {
	if (isLatestVersion("1.0")) {
		std::cout << "Latest Version Installed" << std::endl;
		return 0;
	}
	return -1;
}