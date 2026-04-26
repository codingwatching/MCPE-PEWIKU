#include "TextBox.h"
#include "../../Minecraft.h"
#include "../../../AppPlatform.h"

// @port: port it for text box (chat, username, etc)

TextBox::TextBox( int id, const std::string& msg )
 : GuiElement(true, true, 0, 0, 0, 0), id(id), text(msg), focused(false), option(NULL) {
}

TextBox::TextBox( int id, int x, int y, const std::string& msg ) 
 : GuiElement(true, true, x, y, 0, 0), id(id), text(msg), focused(false), option(NULL) {
}

TextBox::TextBox( int id, int x, int y, int w, int h, const std::string& msg )
 : GuiElement(true, true, x, y, w, h), id(id), text(msg), focused(false), option(NULL) {
}

TextBox::TextBox(Minecraft* mc, const Options::Option* option, const std::string& msg)
 : GuiElement(true, true, 0, 0, 100, 20), id(0), text(mc->options.getStringValue(option)), focused(false), option(option) {
}

void TextBox::setFocus(Minecraft* minecraft) {
	if(!focused) {
		minecraft->platform()->showKeyboard();
		focused = true;
	}
}

bool TextBox::loseFocus(Minecraft* minecraft) {
	if(focused) {
		minecraft->platform()->hideKeyboard(); // 0.8 uses hideKeyboard here??
		focused = false;
		return true;
	}
	return false;
}

void TextBox::render( Minecraft* minecraft, int xm, int ym ) {
	// 0.6 implementation was empty, 0.8 renders the box
	// OptionsItem container might handle some of it
	// @port: add it later
}
