#ifndef NET_MINECRAFT_SharedConstants_H__
#define NET_MINECRAFT_SharedConstants_H__

#include <string>

enum LevelGeneratorVersion
{
	LGV_ORIGINAL = 0,
};



namespace SharedConstants
{
	const int MajorVersion = 0;
	const int MinorVersion = 6;
	const int PatchVersion = 3;
	const int RevisionVersion = 0;
	const int IsBeta = 0;

    const int NetworkProtocolVersion = 9;
	const int NetworkProtocolLowestSupportedVersion = 9;
	const int GameProtocolVersion = 1;
	const int GameProtocolLowestSupportedVersion = 1;

	const int StorageVersion = 3;

	const int MaxChatLength = 100;
    
	const int TicksPerSecond = 20;

	const int GeneratorVersion = (int)LGV_ORIGINAL;

	int getVersionCode();
	int getVersionCode(int a, int b, int c, int d);
}

#endif /*NET_MINECRAFT_SharedConstants_H__*/
