#include "GuiElement.h"

GuiElement::GuiElement( bool active/*=false*/, bool visible/*=true*/, int x /*= 0*/, int y /*= 0*/, int width/*=24*/, int height/*=24*/ )
: active(active),
  visible(visible),
  x(x),
  y(y),
  width(width),
  height(height),
  color(0),
  backgroundNinePatch(nullptr) {
		
}

bool GuiElement::pointInside( int px, int py ) {
	if(px >= this->x && px < this->x + this->width) {
		if(py >= this->y && py < this->y + this->height) {
			return true;
		}
	}
	return false;
}

void GuiElement::setVisible(bool visible) {
	this->visible = visible;
}

void GuiElement::clearBackground() {
	// TODO: implement
}

void GuiElement::setActiveAndVisibility(bool b) {
	active = b;
	visible = b;
}

void GuiElement::setActiveAndVisibility(bool act, bool vis) {
	active = act;
	visible = vis;
}

void GuiElement::setBackground(Minecraft* minecraft, const std::string& str, const IntRectangle& rect, int w, int h) {
	// TODO: implement
}

void GuiElement::setBackground(unsigned int color) {
	this->color = color;
}
