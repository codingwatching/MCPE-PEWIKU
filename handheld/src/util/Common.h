#ifndef NET_MINECRAFT_UTIL__Common_H__
#define NET_MINECRAFT_UTIL__Common_H__

#include <string>

namespace Common {
	std::string getGameVersionString();
	std::string getGameVersionStringNet();
	int getRawTimeS();
	long long getRawTimeMs();
}

#endif /*NET_MINECRAFT_UTIL__Common_H__*/
