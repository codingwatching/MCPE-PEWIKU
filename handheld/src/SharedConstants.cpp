#include "SharedConstants.h"
#include <sstream>
#include <ctime>
#ifdef __linux__
#include <sys/time.h>
#endif



namespace SharedConstants {
	int getVersionCode() {
		return getVersionCode(MajorVersion, MinorVersion, PatchVersion, IsBeta);
	}

	int getVersionCode(int major, int minor, int patch, int beta) {
		return (major << 24) | (minor << 16) | (patch << 8) | beta;
	}
}
