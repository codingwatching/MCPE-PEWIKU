#ifndef NET_MINECRAFT_CLIENT_GUI_SCREENS__InBedScreen_H__
#define NET_MINECRAFT_CLIENT_GUI_SCREENS__InBedScreen_H__

#include "../Screen.h"
class Button;

class InBedScreen: public Screen
{
	typedef Screen super;
public:
	InBedScreen();

	virtual ~InBedScreen();

	void init();

	void setupPositions();

	void render(int xm, int ym, float a);

	void keyPressed( int eventKey );
	void buttonClicked(Button* button);

private:
	Button* bWakeUp;
};

#endif /*NET_MINECRAFT_CLIENT_GUI_SCREENS__InBedScreen_H__*/
