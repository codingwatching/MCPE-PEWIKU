#ifndef NET_MINECRAFT_CLIENT_GUI_COMPONENTS__TextBox_H__
#define NET_MINECRAFT_CLIENT_GUI_COMPONENTS__TextBox_H__

//package net.minecraft.client.gui;

#include <string>
#include "GuiElement.h"
#include "../../Options.h"

class Font;
class Minecraft;

class TextBox: public GuiElement
{
public:
	TextBox(int id, const std::string& msg);
    TextBox(int id, int x, int y, const std::string& msg);
    TextBox(int id, int x, int y, int w, int h, const std::string& msg);
	TextBox(Minecraft* mc, const Options::Option* option, const std::string& msg);

	virtual void setFocus(Minecraft* minecraft);
	virtual bool loseFocus(Minecraft* minecraft);

    virtual void render(Minecraft* minecraft, int xm, int ym);
	
public:
	std::string text;
	int id;
	bool focused;
	const Options::Option* option;
};

#endif /*NET_MINECRAFT_CLIENT_GUI_COMPONENTS__TextBox_H__*/
