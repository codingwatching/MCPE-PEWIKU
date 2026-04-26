#include "THeader.h"
#include "../../../Minecraft.h"
#include "../../../renderer/Textures.h"
#include "../../../renderer/Tesselator.h"
#include "../../../gui/Font.h"
#include "../../../renderer/gles.h"

namespace Touch {

//
// THeader
//

THeader::THeader(int id, const std::string& msg)
:	super(id, msg),
	xText(-99999)
{
	active = false;
	width = 66;
	height = 26;
}

THeader::THeader(int id, int x, int y, const std::string& msg)
:	super(id, x, y, msg),
	xText(-99999)
{
	active = false;
	width = 66;
	height = 26;
}

THeader::THeader(int id, int x, int y, int w, int h, const std::string& msg)
:	super(id, x, y, w, h, msg),
	xText(-99999)
{
	active = false;
}

void THeader::render(Minecraft* minecraft, int xm, int ym) {
	if (!visible) return;
	Font* font = minecraft->font;
	renderBg(minecraft, xm, ym);
	
	int xx = x + width / 2;
	if (xText != -99999)
		xx = xText;
	drawCenteredString(font, msg, xx, y + (height - 8) / 2, 0xe0e0e0);
}

void THeader::renderBg(Minecraft* minecraft, int xm, int ym) {
	minecraft->textures->loadAndBindTexture("gui/touchgui.png");
	glColor4f2(1, 1, 1, 1);

	// Left cap
	blit(x, y, 150, 26, 2, height - 1, 2, 25);
	// Middle
	blit(x + 2, y, 153, 26, width - 3, height - 1, 8, 25);
	// Right cap
	blit(x + width - 2, y, 162, 26, 2, height - 1, 2, 25);
	
	// Shadow
	glEnable2(GL_BLEND);
	glBlendFunc2(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	blit(x, y + height - 1, 153, 52, width, 3, 8, 3);
}

}
