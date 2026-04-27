#include "OptionsScreen.h"
#include "StartMenuScreen.h"
#include "../../Minecraft.h"
#include "../../../AppPlatform.h"
#include "../../../locale/I18n.h"
#include "../components/OptionsPane.h"
#include "../components/ImageButton.h"
#include "../components/CategoryButton.h"
#include "../components/OptionsGroup.h"
#include "../components/NinePatch.h"

OptionsScreen::OptionsScreen(int initialCategory)
	: btnClose(NULL)
	, bHeader(NULL)
	, selectedCategory(0)
	, initialCategory(initialCategory)
	, currentOptionPane(NULL)
	, normalBg(NULL)
	, selectedBg(NULL)
	, selectedCategoryButton(NULL) {
}

OptionsScreen::~OptionsScreen() {
	if (btnClose != NULL) {
		delete btnClose;
		btnClose = NULL;
	}
	if (bHeader != NULL) {
		delete bHeader;
		bHeader = NULL;
	}
	if (normalBg != NULL) {
		delete normalBg;
		normalBg = NULL;
	}
	if (selectedBg != NULL) {
		delete selectedBg;
		selectedBg = NULL;
	}
	for (auto btn : categoryButtons) {
		if (btn != NULL) delete btn;
	}
	for (auto pane : optionPanes) {
		if (pane != NULL) delete pane;
	}
	categoryButtons.clear();
	optionPanes.clear();
}

void OptionsScreen::init() {
	bHeader = new Touch::THeader(0, "Options");
	
	btnClose = new ImageButton(1, "");
	ImageDef def;
	def.name = "gui/touchgui.png";
	def.width = 34;
	def.height = 26;
	def.setSrc(IntRectangle(150, 0, (int)def.width, (int)def.height));
	btnClose->setImageDef(def, true);

	createCategoryButtons();

	buttons.push_back(bHeader);
	buttons.push_back(btnClose);
	for (auto btn : categoryButtons) {
		buttons.push_back(btn);
		tabButtons.push_back(btn);
	}
	
	generateOptionScreens();
}

void OptionsScreen::createCategoryButtons() {
	NinePatchFactory a1(minecraft->textures, "gui/spritesheet.png");
	int btnSize = btnClose->height;
	normalBg = a1.createSymmetrical(IntRectangle(8, 32, 8, 8), 2, 2, (float)btnSize, (float)btnSize);
	selectedBg = a1.createSymmetrical(IntRectangle(0, 32, 8, 8), 2, 2, (float)btnSize, (float)btnSize);

	ImageDef v11;
	v11.name = "gui/touchgui2.png";
	v11.width = (float)btnSize;
	v11.height = (float)btnSize;

	auto addCat = [&](int id, int sx, int sy) {
		CategoryButton* btn = new CategoryButton(id, normalBg, selectedBg, &selectedCategoryButton);
		ImageDef idf = v11;
		idf.setSrc(IntRectangle(sx, sy, 28, 28));
		btn->setImageDef(idf, true);
		btn->width = (float)btnSize;
		btn->height = (float)btnSize;
		categoryButtons.push_back(btn);
	};

	addCat(2, 134, 0);  // Game
	addCat(3, 106, 0);  // Controls
	addCat(4, 134, 28); // Graphics
	addCat(5, 106, 28); // Audio
}

void OptionsScreen::setupPositions() {
	int buttonHeight = btnClose->height;
	
	btnClose->x = 4;
	btnClose->y = 4;
	
	bHeader->x = 0;
	bHeader->y = 0;
	bHeader->width = width;
	bHeader->height = buttonHeight + 8;
	
	int catSize = categoryButtons.size();
	int catHeight = 28;
	int v3 = (height - bHeader->height - 3 - (catSize * catHeight + 2 * (catSize - 1))) / 2;
	
	for (int i = 0; i < catSize; ++i) {
		auto btn = categoryButtons[i];
		btn->x = 5;
		btn->y = bHeader->height + 3 + v3 + 29 * i;
		btn->selected = false;
	}
	
	for (auto pane : optionPanes) {
		if (!categoryButtons.empty() && categoryButtons[0] != NULL) {
			pane->x = categoryButtons[0]->width + 20;
			pane->y = bHeader->height + 3;
			pane->width = width - categoryButtons[0]->width - 20;
			pane->height = height - bHeader->height - 3;
			pane->setupPositions();
		}
	}
	
	selectCategory(initialCategory);
}

void OptionsScreen::render(int xm, int ym, float a) {
	if (minecraft->isLevelGenerated()) {
		renderMenuBackground(a);
	} else {
		renderBackground(0);
	}
	
	fill(0, 0, 38, height, 0xff958782);
	
	super::render(xm, ym, a);
	
	int xmm = xm * width / minecraft->width;
	int ymm = ym * height / minecraft->height - 1;
	
	if (currentOptionPane != NULL) {
		currentOptionPane->render(minecraft, xmm, ymm);
	}
}

void OptionsScreen::buttonClicked(Button* button) {
	if (button == btnClose) {
		minecraft->options.save();
		minecraft->screenChooser.setScreen(SCREEN_STARTMENU);
	} else if (button->id >= 2 && button->id <= 5) {
		selectCategory(button->id - 2);
	}
}

void OptionsScreen::selectCategory(int index) {
	if (index >= 0 && index < (int)categoryButtons.size()) {
		selectedCategoryButton = categoryButtons[index];
	}
	selectedCategory = index;
	if (index < (int)optionPanes.size()) {
		currentOptionPane = optionPanes[index];
	}
}

void OptionsScreen::generateOptionScreens() {
	optionPanes.push_back(new OptionsPane());
	optionPanes.push_back(new OptionsPane());
	optionPanes.push_back(new OptionsPane());
	optionPanes.push_back(new OptionsPane());

	// Game
	optionPanes[0]->createOptionsGroup("options.group.game")
		.addOptionItem(&Options::Option::NAME, minecraft)
		.addOptionItem(&Options::Option::DIFFICULTY, minecraft)
		.addOptionItem(&Options::Option::THIRD_PERSON, minecraft)
		.addOptionItem(&Options::Option::SERVER_VISIBLE, minecraft);

	// Controls
	optionPanes[1]->createOptionsGroup("options.group.input")
		.addOptionItem(&Options::Option::SENSITIVITY, minecraft)
		.addOptionItem(&Options::Option::INVERT_MOUSE, minecraft)
		.addOptionItem(&Options::Option::LEFT_HANDED, minecraft)
		.addOptionItem(&Options::Option::USE_TOUCHSCREEN, minecraft)
		.addOptionItem(&Options::Option::USE_TOUCH_JOYPAD, minecraft)
		.addOptionItem(&Options::Option::PIXELS_PER_MILLIMETER, minecraft);
	
	if (minecraft->platform()->supportsVibration()) {
		optionPanes[1]->createOptionsGroup("options.group.feedback")
			.addOptionItem(&Options::Option::DESTROY_VIBRATION, minecraft);
	}

	// Graphics
	optionPanes[2]->createOptionsGroup("options.group.graphics")
		.addOptionItem(&Options::Option::RENDER_DISTANCE, minecraft)
		.addOptionItem(&Options::Option::GRAPHICS, minecraft)
		.addOptionItem(&Options::Option::GUI_SCALE, minecraft)
		.addOptionItem(&Options::Option::FOV, minecraft)
		.addOptionItem(&Options::Option::FANCY_SKIES, minecraft)
		.addOptionItem(&Options::Option::ANIMATE_TEXTURES, minecraft);
	
	optionPanes[2]->createOptionsGroup("options.group.graphics.experimental")
		.addOptionItem(&Options::Option::HIDE_GUI, minecraft);

	// Audio
	optionPanes[3]->createOptionsGroup("options.group.audio")
		.addOptionItem(&Options::Option::SOUND, minecraft)
		.addOptionItem(&Options::Option::MUSIC, minecraft);
}

void OptionsScreen::mouseClicked(int x, int y, int buttonNum) {
	if (currentOptionPane != NULL) {
		currentOptionPane->mouseClicked(minecraft, x, y, buttonNum);
	}
	super::mouseClicked(x, y, buttonNum);
}

void OptionsScreen::mouseReleased(int x, int y, int buttonNum) {
	if (currentOptionPane != NULL) {
		currentOptionPane->mouseReleased(minecraft, x, y, buttonNum);
	}
	super::mouseReleased(x, y, buttonNum);
}

void OptionsScreen::tick() {
	if (currentOptionPane != NULL) {
		currentOptionPane->tick(minecraft);
	}
	super::tick();
}

void OptionsScreen::removed() {
}
