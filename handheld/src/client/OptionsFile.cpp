#include "OptionsFile.h"
#include <stdio.h>
#include <string.h>
#include "../util/StringUtils.h"

OptionsFile::OptionsFile() {
#ifdef __APPLE__
	settingsPath = "./Documents/options.txt";
#elif defined(ANDROID)
	settingsPath = "options.txt";
#else
	settingsPath = "options.txt";
#endif
}

void OptionsFile::save(const StringVector& settings) {
	FILE* pFile = fopen(settingsPath.c_str(), "w");
	if(pFile != NULL) {
		for(StringVector::const_iterator it = settings.begin(); it != settings.end(); ++it) {
			fprintf(pFile, "%s\n", it->c_str());
		}
		fclose(pFile);
	}
}

StringVector OptionsFile::getOptionStrings() {
	StringVector returnVector;
	FILE* pFile = fopen(settingsPath.c_str(), "r");
	if(pFile != NULL) {
		char lineBuff[512];
		while(fgets(lineBuff, sizeof lineBuff, pFile)) {
			std::string line = Util::stringTrim(std::string(lineBuff));
			if (line.empty() || line[0] == '#')
				continue;

			const std::string::size_type colonPos = line.find(':');
			if (colonPos == std::string::npos)
				continue;

			std::string key = Util::stringTrim(line.substr(0, colonPos));
			std::string value = Util::stringTrim(line.substr(colonPos + 1));
			if (!key.empty()) {
				returnVector.push_back(key);
				returnVector.push_back(value);
			}
		}
		fclose(pFile);
	}
	return returnVector;
}
