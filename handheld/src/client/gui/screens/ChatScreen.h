#ifndef NET_MINECRAFT_CLIENT_GUI_SCREENS__ChatScreen_H__
#define NET_MINECRAFT_CLIENT_GUI_SCREENS__ChatScreen_H__

#include "../Screen.h"

class ChatScreen: public Screen
{
	typedef Screen super;
public:
	ChatScreen() {}
	virtual ~ChatScreen() {}

	void init();

	void render(int xm, int ym, float a);

	void keyPressed( int eventKey );
	void buttonClicked(Button* button) {};

private:
};

#endif /*NET_MINECRAFT_CLIENT_GUI_SCREENS__ChatScreen_H__*/
