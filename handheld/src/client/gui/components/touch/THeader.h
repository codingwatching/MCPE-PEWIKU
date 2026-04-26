#ifndef NET_MINECRAFT_CLIENT_GUI_COMPONENTS_TOUCH__THeader_H__
#define NET_MINECRAFT_CLIENT_GUI_COMPONENTS_TOUCH__THeader_H__

#define IN_TOUCH_COMPONENT
#include "../Button.h"
#undef IN_TOUCH_COMPONENT

namespace Touch {

// "Header" in Touchscreen mode
class THeader : public Button {
	typedef Button super;
public:
	THeader(int id, const std::string& msg);
	THeader(int id, int x, int y, const std::string& msg);
	THeader(int id, int x, int y, int w, int h, const std::string& msg);
protected:
	virtual void renderBg(Minecraft* minecraft, int xm, int ym);
	virtual void render(Minecraft* minecraft, int xm, int ym);
public:
	int xText;
};

}

#endif /*NET_MINECRAFT_CLIENT_GUI_COMPONENTS__Touch_THeader_H__*/
