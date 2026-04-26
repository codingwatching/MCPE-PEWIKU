#include "TButton.h"
#include "../../../Minecraft.h"
#include "../../../renderer/Textures.h"
#include "../../../renderer/Tesselator.h"
#include "../../../gui/Font.h"
#include "../../../renderer/gles.h"

namespace Touch {

//
// TButton
//

TButton::TButton(int id, const std::string& msg)
:	super(id, msg)
{
	width = 66;
	height = 26;
}

TButton::TButton(int id, int x, int y, const std::string& msg)
:	super(id, x, y, msg)
{
	width = 66;
	height = 26;
}

TButton::TButton(int id, int x, int y, int w, int h, const std::string& msg)
:	super(id, x, y, w, h, msg)
{
}

void TButton::renderBg(Minecraft* minecraft, int xm, int ym) {
	bool isHovered = active && _currentlyDown && xm >= x && ym >= y && xm < x + width && ym < y + height;

	minecraft->textures->loadAndBindTexture("gui/touchgui.png");

	if (active)
		glColor4f2(1, 1, 1, 1);
	else
		glColor4f2(0.5f, 0.5f, 0.5f, 1);

	blit(x, y, isHovered ? 66 : 0, 0, width, height, 66, 26);
}

}
