#include "OptionsItem.h"
#include "../../Minecraft.h"
#include "../../../util/Mth.h"
#include "Slider.h"
#include <sstream>
OptionsItem::OptionsItem( std::string label, GuiElement* element )
: GuiElementContainer(false, true, 0, 0, 24, 12),
  label(label) {
	  addChild(element);
}

void OptionsItem::setupPositions() {
	int currentHeight = 0;
	for(std::vector<GuiElement*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->x = x + width - (*it)->width - 15;
		(*it)->y = y + currentHeight;
		currentHeight += (*it)->height;
	}
	height = currentHeight;
}

void OptionsItem::render( Minecraft* minecraft, int xm, int ym ) {
	int yOffset = (height - 8) / 2;
	std::string displayLabel = label;
	if (!children.empty()) {
		Slider* slider = dynamic_cast<Slider*>(children[0]);
		if (slider && slider->getOption() == &Options::Option::FOV) {
			std::stringstream ss;
			ss << label << ": " << minecraft->options.fov;
			displayLabel = ss.str();
		} else if (slider && slider->getOption() == &Options::Option::GUI_SCALE) {
			std::stringstream ss;
			ss << label << ": ";
			if (minecraft->options.guiScale <= 0) ss << "Auto";
			else ss << minecraft->options.guiScale << "x";
			displayLabel = ss.str();
		}
	}
	minecraft->font->draw(displayLabel, (float)x, (float)y + yOffset, 0xffffffff, true);
	super::render(minecraft, xm, ym);
}
