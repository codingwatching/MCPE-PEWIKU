#ifndef NET_MINECRAFT_CLIENT_GUI_COMPONENTS_TOUCH__TButton_H__
#define NET_MINECRAFT_CLIENT_GUI_COMPONENTS_TOUCH__TButton_H__

#define IN_TOUCH_COMPONENT
#include "../Button.h"
#undef IN_TOUCH_COMPONENT

namespace Touch {

class TButton : public Button {
	typedef Button super;
public:
	TButton(int id, const std::string& msg);
	TButton(int id, int x, int y, const std::string& msg);
	TButton(int id, int x, int y, int w, int h, const std::string& msg);
protected:
	virtual void renderBg(Minecraft* minecraft, int xm, int ym);
};


}

#endif /*NET_MINECRAFT_CLIENT_GUI_COMPONENTS__Touch_TButton_H__*/
