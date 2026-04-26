#ifndef NET_MINECRAFT_CLIENT__Options_H__
#define NET_MINECRAFT_CLIENT__Options_H__

// package net.minecraft.client;

//#include "locale/Language.h"

#include <string>
#include <cstdio>
#include "KeyMapping.h"
#include "../platform/input/Keyboard.h"
#include "../util/StringUtils.h"
#include "OptionsFile.h"

class Minecraft;
typedef std::vector<std::string> StringVector;

class Options
{
public:
	class Option
	{
		const bool _isProgress;
		const bool _isBoolean;
		const std::string _captionId;
		const int _ordinal;

	public:
		static const Option MUSIC;
		static const Option SOUND;
		static const Option INVERT_MOUSE;
		static const Option SENSITIVITY;
		static const Option RENDER_DISTANCE;
		static const Option VIEW_BOBBING;
		static const Option ANAGLYPH;
		static const Option LIMIT_FRAMERATE;
		static const Option DIFFICULTY;
		static const Option GRAPHICS;
		static const Option AMBIENT_OCCLUSION;
		static const Option GUI_SCALE;

		static const Option THIRD_PERSON;
		static const Option HIDE_GUI;
		static const Option SERVER_VISIBLE;
		static const Option LEFT_HANDED;
		static const Option USE_TOUCHSCREEN;
		static const Option USE_TOUCH_JOYPAD;
		static const Option DESTROY_VIBRATION;

		static const Option FANCY_SKIES;
		static const Option ANIMATE_TEXTURES;
		static const Option PIXELS_PER_MILLIMETER;
		static const Option FOV;
		static const Option NAME;

		/*
		static Option* getItem(int id) {
			for (Option item : Option.values()) {
				if (item.getId() == id) {
					return item;
				}
			}
			return NULL;
		}
		*/

		Option(int ordinal, const std::string& captionId, bool hasProgress, bool isBoolean)
			: _captionId(captionId),
			  _isProgress(hasProgress),
			  _isBoolean(isBoolean),
			  _ordinal(ordinal) {
		}

		bool isProgress() const {
			return _isProgress;
		}

		bool isBoolean() const {
			return _isBoolean;
		}

		bool isInt() const {
			return (!_isBoolean && !_isProgress);
		}

		int getId() {
			return _ordinal;
		}

		std::string getCaptionId() const {
			return _captionId;
		}
	};

private:
	static const float SOUND_MIN_VALUE;
	static const float SOUND_MAX_VALUE;
	static const float MUSIC_MIN_VALUE;
	static const float MUSIC_MAX_VALUE;
	static const float SENSITIVITY_MIN_VALUE;
	static const float SENSITIVITY_MAX_VALUE;
	static const float PIXELS_PER_MILLIMETER_MIN_VALUE;
	static const float PIXELS_PER_MILLIMETER_MAX_VALUE;
	static const float FOV_MIN_VALUE;
	static const float FOV_MAX_VALUE;
	static const char* RENDER_DISTANCE_NAMES[];
	static const char* DIFFICULTY_NAMES[];
	static const char* GUI_SCALE[];
	static const int DIFFICULY_LEVELS[];

public:
	static bool debugGl;

	float music;
	float sound;
	// note: sensitivity is transformed in Options::update
	float sensitivity;
	bool invertYMouse;
	int viewDistance;
	bool bobView;
	bool isSplitControls;	
	bool anaglyph3d;
	bool limitFramerate;
	bool fancyGraphics;
	bool ambientOcclusion;
	int fov;
	bool useMouseForDigging;
	bool isLeftHanded;
	bool destroyVibration;
	// std::string skin;

	KeyMapping keyUp;
	KeyMapping keyLeft;
	KeyMapping keyDown;
	KeyMapping keyRight;
	KeyMapping keyJump;
	KeyMapping keyBuild;
	KeyMapping keyDrop;
	KeyMapping keyChat;
	KeyMapping keyFog;
	KeyMapping keySneak;
	KeyMapping keyCraft;
	KeyMapping keyDestroy;
	KeyMapping keyUse;
	KeyMapping keyInventory;

	KeyMapping keyMenuNext;
	KeyMapping keyMenuPrevious;
	KeyMapping keyMenuOk;
	KeyMapping keyMenuCancel;

	KeyMapping* keyMappings[16];

	/*protected*/ Minecraft* minecraft;
	///*private*/ File optionsFile;

	int difficulty;
	bool hideGui;
	bool thirdPersonView;
	float brightness;
	bool renderDebug;

	bool isFlying;
	bool fixedCamera;
	float flySpeed;
	float cameraSpeed;
	int guiScale;
	std::string username;

	bool serverVisible;
	bool isJoyTouchArea;
	bool useTouchScreen;
	float pixelsPerMillimeter;

	bool fancySkies;
	bool animateTextures;

	int major;
	int minor;
	int patch;
	int beta;

	std::vector<const Option*> hiddenOptions;

	static std::vector<int> DIFFICULTY_LEVELS;
	static std::vector<int> RENDER_DISTANCE_LEVELS;

	void init(Minecraft* minecraft, const std::string& workingDirectory);
	Options(Minecraft* minecraft, const std::string& workingDirectory)
		: minecraft(minecraft) {
		// optionsFile = /*new*/ File(workingDirectory, "options.txt");
		initDefaultValues();

		load();
	}

	Options()
		: minecraft(NULL) {
	}

	void initDefaultValues();
	void validateVersion();

	std::string getKeyDescription(int i);
	// Language language = Language.getInstance();
	// return language.getElement(keyMappings[i].name);
	std::string getKeyMessage(int i);
	void setKey(int i, int key);

	void set(const Option* item, float value);
	void set(const Option* item, int value);
	void set(const Option* item, const std::string& value);

	void toggle(const Option* option, int dir);

	int getIntValue(const Option* item);
	float getProgressValue(const Option* item);
	bool getBooleanValue(const Option* item);
	float getProgrssMin(const Option* item);
	float getProgrssMax(const Option* item);

	bool hideOption(const Option* option);
	bool canModify(const Option* option);
	std::vector<int> getValues(const Option* option);
	std::string getStringValue(const Option* option);
	void setAdditionalHiddenOptions(const std::vector<const Option*>& options);

	std::string getMessage(const Option* item);

	void update();
	void load();
	void save();
	void addOptionToSaveOutput(StringVector& stringVector, std::string name, bool boolValue);
	void addOptionToSaveOutput(StringVector& stringVector, std::string name, float floatValue);
	void addOptionToSaveOutput(StringVector& stringVector, std::string name, int intValue);
	void addOptionToSaveOutput(StringVector& stringVector, std::string name, const std::string& stringValue);
	void notifyOptionUpdate(const Option* option, bool value);
	void notifyOptionUpdate(const Option* option, float value);
	void notifyOptionUpdate(const Option* option, int value);

private:
	static bool readFloat(const std::string& string, float& value);
	static bool readInt(const std::string& string, int& value);
	static bool readBool(const std::string& string, bool& value);

private:
	OptionsFile optionsFile;
};

#endif /*NET_MINECRAFT_CLIENT__Options_H__*/
