#include "Slider.h"
#include "../../Minecraft.h"
#include "../../renderer/Textures.h"
#include "../Screen.h"
#include "../../../util/Mth.h"
#include <algorithm>
#include <assert.h>
Slider::Slider(Minecraft* minecraft, const Options::Option* option,  float progressMin, float progressMax)
: sliderType(SliderProgress), mouseDownOnElement(false), option(option), numSteps(0), progressMin(progressMin), progressMax(progressMax) {
	if(option != NULL) {
		percentage = (minecraft->options.getProgressValue(option) - progressMin) / (progressMax - progressMin);
	}
}

Slider::Slider(Minecraft* minecraft, const Options::Option* option, const std::vector<int>& stepVec )
: sliderType(SliderStep),
  curStepValue(0),
  curStep(0),
  sliderSteps(stepVec),
  mouseDownOnElement(false),
  option(option),
  percentage(0),
  progressMin(0.0f),
  progressMax(1.0) {
	assert(stepVec.size() > 1);
	numSteps = sliderSteps.size();
	curStepValue = sliderSteps[0];
	curStep = 0;

	if (option != NULL) {
		curStepValue = minecraft->options.getIntValue(option);
		std::vector<int>::const_iterator currentItem = std::find(sliderSteps.begin(), sliderSteps.end(), curStepValue);
		if (currentItem != sliderSteps.end()) {
			curStep = (int)(currentItem - sliderSteps.begin());
		} else {
			curStepValue = sliderSteps[0];
			curStep = 0;
		}
	}

	percentage = float(curStep) / float(numSteps - 1);
}

void Slider::render( Minecraft* minecraft, int xm, int ym ) {
	int xSliderStart = x + 5;
	int xSliderEnd = x + width - 5;
	int ySliderStart = y + 6;
	int ySliderEnd = y + 9;
	int handleSizeX = 9;
	int handleSizeY = 15;
	int barWidth = xSliderEnd - xSliderStart;
	//fill(x, y + 8, x + (int)(width * percentage), y + height, 0xffff00ff);
	fill(xSliderStart, ySliderStart, xSliderEnd, ySliderEnd, 0xff606060);
	if(sliderType == SliderStep) {
		int stepDistance = barWidth / (numSteps -1);
		for(int a = 0; a <= numSteps - 1; ++a) {
			int renderSliderStepPosX = xSliderStart + a * stepDistance + 1;
			fill(renderSliderStepPosX - 1, ySliderStart - 2, renderSliderStepPosX + 1, ySliderEnd + 2, 0xff606060);
		}
	}
	minecraft->textures->loadAndBindTexture("gui/touchgui.png");
	blit(xSliderStart + (int)(percentage * barWidth) - handleSizeX / 2, y, 226, 126, handleSizeX, handleSizeY, handleSizeX, handleSizeY);
}

void Slider::mouseClicked( Minecraft* minecraft, int x, int y, int buttonNum ) {
	if (buttonNum != MouseAction::ACTION_LEFT)
		return;

	if (!pointInside(x, y))
		return;

	mouseDownOnElement = true;
	updateFromMouse(minecraft, x);
	setOption(minecraft);
}

void Slider::mouseReleased( Minecraft* minecraft, int x, int y, int buttonNum ) {
	if (buttonNum != MouseAction::ACTION_LEFT)
		return;

	mouseDownOnElement = false;
	if(sliderType == SliderStep) {
		curStep = Mth::floor((percentage * (numSteps-1) + 0.5f));
		curStepValue = sliderSteps[Mth::Min(curStep, numSteps-1)];
		percentage = float(curStep) / (numSteps - 1);
		setOption(minecraft);
	}
}

void Slider::tick(Minecraft* minecraft) {
	if(minecraft->screen != NULL) {
		int xm = Mouse::getX();
		int ym = Mouse::getY();
		minecraft->screen->toGUICoordinate(xm, ym);
		if (!Mouse::isButtonDown(MouseAction::ACTION_LEFT))
			mouseDownOnElement = false;

		if (!mouseDownOnElement)
			return;

		updateFromMouse(minecraft, xm);
		setOption(minecraft);
	}
}

void Slider::updateFromMouse(Minecraft* minecraft, int mouseX) {
	(void)minecraft;
	const int xSliderStart = x + 5;
	const int xSliderEnd = x + width - 5;
	const int sliderWidth = Mth::Max(1, xSliderEnd - xSliderStart);

	percentage = float(mouseX - xSliderStart) / float(sliderWidth);
	percentage = Mth::clamp(percentage, 0.0f, 1.0f);

	if (sliderType == SliderStep) {
		curStep = Mth::floor((percentage * (numSteps - 1)) + 0.5f);
		curStep = Mth::clamp(curStep, 0, numSteps - 1);
		curStepValue = sliderSteps[curStep];
		percentage = float(curStep) / float(numSteps - 1);
	}
}

void Slider::setOption( Minecraft* minecraft ) {
	if(option != NULL) {
		if(sliderType == SliderStep) {
			if(minecraft->options.getIntValue(option) != curStepValue) {
				minecraft->options.set(option, curStepValue);
			}
		} else {
			if(minecraft->options.getProgressValue(option) != percentage * (progressMax - progressMin) + progressMin) {
				minecraft->options.set(option, percentage *  (progressMax - progressMin) + progressMin);
			}
		}
	}
}
