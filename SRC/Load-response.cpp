#include "WebServer.h"
#include <fstream>

int LoadResponse(std::string& result, const char* dirName) {

	std::ifstream file{ dirName };

	if (!file.is_open())
	{
		return -1;
	}

	std::string buf{};

	while (std::getline(file, buf))
	{
		result += buf;
	}

	file.close();
	
	return 0;
}
