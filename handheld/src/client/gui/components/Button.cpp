#include "Button.h"
#include "../../Minecraft.h"
#include "../../renderer/Textures.h"

Button::Button(int id, const std::string& msg)
:	GuiElement(true, true, 0, 0, 200, 24),
    id(id),
	msg(msg),
	selected(false),
    _currentlyDown(false)
{
}

Button::Button( int id, int x, int y, const std::string& msg )
:	GuiElement(true, true, x, y, 200, 24),
    id(id),
	msg(msg),
	selected(false),
    _currentlyDown(false)
{
}

Button::Button( int id, int x, int y, int w, int h, const std::string& msg )
:	GuiElement(true, true, x, y, w, h),
    id(id),
	msg(msg),
	selected(false),
    _currentlyDown(false)
{
}

void Button::render( Minecraft* minecraft, int xm, int ym )
{
	if (!visible) return;

	/*
	minecraft->textures->loadAndBindTexture("gui/gui.png");
	glColor4f2(1, 1, 1, 1);

	//printf("ButtonId: %d - Hovered? %d (cause: %d, %d, %d, %d, <> %d, %d)\n", id, hovered, x, y, x+w, y+h, xm, ym);
	int yImage = getYImage(hovered || selected);

	blit(x, y, 0, 46 + yImage * 20, w / 2, h, 0, 20);
	blit(x + w / 2, y, 200 - w / 2, 46 + yImage * 20, w / 2, h, 0, 20);
	*/

	renderBg(minecraft, xm, ym);
	renderFace(minecraft, xm , ym);
}

void Button::released( int mx, int my ) {
    _currentlyDown = false;
}

bool Button::clicked( Minecraft* minecraft, int mx, int my )
{
	return active && mx >= x && my >= y && mx < x + width && my < y + height;
}

void Button::setPressed() {
    _currentlyDown = true;
}

int Button::getYImage( bool hovered )
{
	int res = 1;
	if (!active) res = 0;
	else if (hovered) res = 2;
	return res;
}

void Button::renderFace(Minecraft* mc, int xm, int ym) {
	Font* font = mc->font;
	if (!active) {
		drawCenteredString(font, msg, x + width / 2, y + (height - 8) / 2, 0xffa0a0a0);
	} else {
		if (hovered(mc, xm, ym) || selected) {
			drawCenteredString(font, msg, x + width / 2, y + (height - 8) / 2, 0xffffa0);
		} else {
			drawCenteredString(font, msg, x + width / 2, y + (height - 8) / 2, 0xe0e0e0);
		}
	}
}

void Button::renderBg( Minecraft* minecraft, int xm, int ym )
{
	minecraft->textures->loadAndBindTexture("gui/gui.png");
	glColor4f2(1, 1, 1, 1);

	//printf("ButtonId: %d - Hovered? %d (cause: %d, %d, %d, %d, <> %d, %d)\n", id, hovered, x, y, x+w, y+h, xm, ym);
	int yImage = getYImage(selected || hovered(minecraft, xm, ym));;

	blit(x, y, 0, 46 + yImage * 20, width / 2, height, 0, 20);
	blit(x + width / 2, y, 200 - width / 2, 46 + yImage * 20, width / 2, height, 0, 20);
}

bool Button::hovered(Minecraft* minecraft, int xm , int ym) {
	return _currentlyDown && isInside(xm, ym);
}

bool Button::isInside( int xm, int ym ) {
	return xm >= x && ym >= y && xm < x + width && ym < y + height;
}

//
// BlankButton
//
BlankButton::BlankButton(int id)
:	super(id, "")
{
	visible = false;
}

BlankButton::BlankButton(int id, int x, int y, int w, int h)
:	super(id, x, y, w, h, "")
{
	visible = false;
}


