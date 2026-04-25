#ifndef NET_MINECRAFT_CLIENT_GUI__GuiElement_H__
#define NET_MINECRAFT_CLIENT_GUI__GuiElement_H__
#include "../GuiComponent.h"
#include <string>

class Tesselator;
class Minecraft;
class IntRectangle;
class NinePatchLayer;

class GuiElement : public GuiComponent {
public:
	GuiElement(bool active=false, bool visible=true, int x = 0, int y = 0, int width=24, int height=24);
    virtual ~GuiElement() {}

	void clearBackground();
	void setActiveAndVisibility(bool b);
	void setActiveAndVisibility(bool act, bool vis);
	void setBackground(Minecraft* minecraft, const std::string&, const IntRectangle&, int, int);
	void setBackground(unsigned int color);
	void setVisible(bool visible);

    virtual void tick(Minecraft* minecraft) {}
    virtual void render(Minecraft* minecraft, int xm, int ym) { }
	virtual void topRender(Minecraft* minecraft, int xm, int ym) {}
	virtual void setupPositions() {}
	virtual void mouseClicked(Minecraft* minecraft, int x, int y, int buttonNum) {}
	virtual void mouseReleased(Minecraft* minecraft, int x, int y, int buttonNum) {}
	virtual void focusuedMouseClicked(Minecraft* minecraft, int x, int y, int buttonNum) {}
	virtual void focusuedMouseReleased(Minecraft* minecraft, int x, int y, int buttonNum) {}
	virtual void keyPressed(Minecraft* minecraft, int key) {}
	virtual void keyboardNewChar(Minecraft* minecraft, std::string text, bool b) {}
	virtual bool backPressed(Minecraft* minecraft, bool b) { return false; }
	virtual bool pointInside(int x, int y);
	virtual bool suppressOtherGUI() { return false; }
	virtual void setTextboxText(const std::string& text) {}

	bool active;
	bool visible;
	int x;
	int y;
	int width;
	int height;
	unsigned int color;
	NinePatchLayer* backgroundNinePatch;
};

#endif /*NET_MINECRAFT_CLIENT_GUI__GuiElement_H__*/
