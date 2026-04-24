#include "OptionsScreen.h"

#include "StartMenuScreen.h"
#include "DialogDefinitions.h"
#include "../../Minecraft.h"
#include "../../User.h"
#include "../../../AppPlatform.h"
#include "../../../util/StringUtils.h"
#include "../../../platform/input/Keyboard.h"

#include "../components/OptionsPane.h"
#include "../components/ImageButton.h"
#include "../components/OptionsGroup.h"

#if defined(WIN32)
class Win32UsernameScreen : public Screen {
	typedef Screen super;
public:
	Win32UsernameScreen()
	: bSave(1, "Save"),
	  bCancel(2, "Cancel"),
	  bEditField(3, 0, 0, 0, 0),
	  username(""),
	  frame(0) {
	}

	void init() {
		username = minecraft->options.username;
		buttons.push_back(&bSave);
		buttons.push_back(&bCancel);
		buttons.push_back(&bEditField);
		tabButtons.push_back(&bEditField);
		tabButtons.push_back(&bSave);
		tabButtons.push_back(&bCancel);
	}

	void setupPositions() {
		const int fieldW = 240;
		const int fieldH = 20;
		bEditField.width = fieldW;
		bEditField.height = fieldH;
		bEditField.x = (width - fieldW) / 2;
		bEditField.y = height / 2 - 10;

		bSave.width = 96;
		bCancel.width = 96;
		bSave.x = width / 2 - bSave.width - 4;
		bCancel.x = width / 2 + 4;
		bSave.y = bCancel.y = bEditField.y + 34;
	}

	void tick() {
		++frame;
	}

	void render(int xm, int ym, float a) {
		renderBackground();
		drawCenteredString(font, "Multiplayer Username", width / 2, bEditField.y - 24, 0xffffff);
		drawCenteredString(font, "Used for LAN hosting and joining", width / 2, bEditField.y - 12, 0xffa0a0a0);

		const bool fieldSelected = bEditField.selected;
		const int borderColor = fieldSelected ? 0xfff0f0f0 : 0xff777777;
		fill(bEditField.x - 1, bEditField.y - 1, bEditField.x + bEditField.width + 1, bEditField.y + bEditField.height + 1, borderColor);
		fill(bEditField.x, bEditField.y, bEditField.x + bEditField.width, bEditField.y + bEditField.height, 0xff101010);

		std::string shown = username;
		if (fieldSelected && ((frame / 8) & 1) == 0)
			shown += "_";
		drawString(font, shown, bEditField.x + 4, bEditField.y + 6, 0xffffffff);

		super::render(xm, ym, a);
	}

	void buttonClicked(Button* button) {
		if (button == &bCancel) {
			minecraft->setScreen(new OptionsScreen(1));
			return;
		}
		if (button == &bEditField) {
			return;
		}
		if (button == &bSave) {
			std::string trimmed = Util::stringTrim(username);
			if (trimmed.empty()) {
				trimmed = "Steve";
			}

			minecraft->options.username = trimmed;
			minecraft->options.save();
			if (minecraft->user) {
				minecraft->user->name = trimmed;
			}

			minecraft->setScreen(new OptionsScreen(1));
		}
	}

	void keyPressed(int eventKey) {
		if (eventKey == Keyboard::KEY_ESCAPE) {
			minecraft->setScreen(new OptionsScreen(1));
			return;
		}
		if (eventKey == Keyboard::KEY_RETURN) {
			buttonClicked(&bSave);
			return;
		}
		if (eventKey == Keyboard::KEY_BACKSPACE) {
			if (!username.empty())
				username.erase(username.size() - 1, 1);
			return;
		}
		super::keyPressed(eventKey);
	}

	void keyboardNewChar(char inputChar) {
		if (inputChar < 32 || inputChar > 126)
			return;
		if (username.size() >= 16)
			return;
		username.push_back(inputChar);
	}

private:
	Button bSave;
	Button bCancel;
	BlankButton bEditField;
	std::string username;
	int frame;
};
#endif

OptionsScreen::OptionsScreen(int initialCategory)
: btnClose(NULL),
#if defined(WIN32)
  btnUsername(NULL),
#endif
  bHeader(NULL),
  selectedCategory(0),
  initialCategory(initialCategory) {
}

OptionsScreen::~OptionsScreen() {
	if(btnClose != NULL) {
		delete btnClose;
		btnClose = NULL;
	}
	if(bHeader != NULL) {
		delete bHeader,
		bHeader = NULL;
	}
#if defined(WIN32)
	if (btnUsername != NULL) {
		delete btnUsername;
		btnUsername = NULL;
	}
#endif
	for(std::vector<Touch::TButton*>::iterator it = categoryButtons.begin(); it != categoryButtons.end(); ++it) {
		if(*it != NULL) {
			delete *it;
			*it = NULL;
		}
	}
	for(std::vector<OptionsPane*>::iterator it = optionPanes.begin(); it != optionPanes.end(); ++it) {
		if(*it != NULL) {
			delete *it;
			*it = NULL;
		}
	}
	categoryButtons.clear();
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

	categoryButtons.push_back(new Touch::TButton(2, "Audio"));
	categoryButtons.push_back(new Touch::TButton(3, "Game"));
	categoryButtons.push_back(new Touch::TButton(4, "Controls"));
	categoryButtons.push_back(new Touch::TButton(5, "Graphics"));
	buttons.push_back(bHeader);
	buttons.push_back(btnClose);
#if defined(WIN32)
	btnUsername = new Button(7, "Change Username");
	buttons.push_back(btnUsername);
	tabButtons.push_back(btnUsername);
#endif
	for(std::vector<Touch::TButton*>::iterator it = categoryButtons.begin(); it != categoryButtons.end(); ++it) {
		buttons.push_back(*it);
		tabButtons.push_back(*it);
	}
	generateOptionScreens();

}
void OptionsScreen::setupPositions() {
	int buttonHeight = btnClose->height;
	btnClose->x = width - btnClose->width;
	btnClose->y = 0;
	int offsetNum = 1;
	for(std::vector<Touch::TButton*>::iterator it = categoryButtons.begin(); it != categoryButtons.end(); ++it) {
		(*it)->x = 0;
		(*it)->y = offsetNum * buttonHeight;
		(*it)->selected = false;
		offsetNum++;
	}
	bHeader->x = 0;
	bHeader->y = 0;
	bHeader->width = width - btnClose->width;
	bHeader->height = btnClose->height;
	for(std::vector<OptionsPane*>::iterator it = optionPanes.begin(); it != optionPanes.end(); ++it) {
		if(categoryButtons.size() > 0 && categoryButtons[0] != NULL) {
			(*it)->x = categoryButtons[0]->width;
			(*it)->y = bHeader->height;
			(*it)->width = width - categoryButtons[0]->width;
			(*it)->setupPositions();
		}
	}
#if defined(WIN32)
	if (btnUsername != NULL) {
		btnUsername->width = 140;
		btnUsername->x = categoryButtons[0]->width + ((width - categoryButtons[0]->width) - btnUsername->width) / 2;
		btnUsername->y = bHeader->height + 86;
	}
#endif
	selectCategory(initialCategory);
}

void OptionsScreen::render( int xm, int ym, float a ) {
	renderBackground();
	super::render(xm, ym, a);
	int xmm = xm * width / minecraft->width;
	int ymm = ym * height / minecraft->height - 1;
	if(currentOptionPane != NULL)
		currentOptionPane->render(minecraft, xmm, ymm);
#if defined(WIN32)
	if (selectedCategory == 1 && btnUsername != NULL) {
		drawString(minecraft->font, "Username: " + minecraft->options.username, btnUsername->x, btnUsername->y - 12, 0xffb0b0b0);
	}
#endif
}

void OptionsScreen::removed()
{
}
void OptionsScreen::buttonClicked( Button* button ) {
	if(button == btnClose) {
		minecraft->reloadOptions();
		minecraft->screenChooser.setScreen(SCREEN_STARTMENU);
#if defined(WIN32)
	} else if(button == btnUsername) {
		minecraft->setScreen(new Win32UsernameScreen());
#endif
	} else if(button->id > 1 && button->id < 7) {
		// This is a category button
		int categoryButton = button->id - categoryButtons[0]->id;
		selectCategory(categoryButton);
	}
}

void OptionsScreen::selectCategory( int index ) {
	int currentIndex = 0;
	for(std::vector<Touch::TButton*>::iterator it = categoryButtons.begin(); it != categoryButtons.end(); ++it) {
		if(index == currentIndex) {
			(*it)->selected = true;
		} else {
			(*it)->selected = false;
		}
		currentIndex++;
	}
	selectedCategory = index;
	if(index < (int)optionPanes.size())
		currentOptionPane = optionPanes[index];
#if defined(WIN32)
	if (btnUsername != NULL) {
		const bool isGameCategory = (index == 1);
		btnUsername->visible = isGameCategory;
		btnUsername->active = isGameCategory;
	}
#endif
}

void OptionsScreen::generateOptionScreens() {
	optionPanes.push_back(new OptionsPane());
	optionPanes.push_back(new OptionsPane());
	optionPanes.push_back(new OptionsPane());
	optionPanes.push_back(new OptionsPane());

	// Audio
	optionPanes[0]->createOptionsGroup("options.group.mojang")
		.addOptionItem(&Options::Option::MUSIC, minecraft)
		.addOptionItem(&Options::Option::SOUND, minecraft);

	// Game
	optionPanes[1]->createOptionsGroup("options.group.mojang")
		.addOptionItem(&Options::Option::DIFFICULTY, minecraft)
		.addOptionItem(&Options::Option::THIRD_PERSON, minecraft)
		.addOptionItem(&Options::Option::SERVER_VISIBLE, minecraft);

	// Controls
	optionPanes[2]->createOptionsGroup("options.group.mojang")
		.addOptionItem(&Options::Option::SENSITIVITY, minecraft)
		.addOptionItem(&Options::Option::INVERT_MOUSE, minecraft)
		.addOptionItem(&Options::Option::LEFT_HANDED, minecraft)
		.addOptionItem(&Options::Option::USE_TOUCHSCREEN, minecraft)
		.addOptionItem(&Options::Option::USE_TOUCH_JOYPAD, minecraft)
		.addOptionItem(&Options::Option::DESTROY_VIBRATION, minecraft);

	// Graphics
	optionPanes[3]->createOptionsGroup("options.group.mojang")
		.addOptionItem(&Options::Option::GRAPHICS, minecraft)
		.addOptionItem(&Options::Option::GUI_SCALE, minecraft)
		.addOptionItem(&Options::Option::FOV, minecraft);
}

void OptionsScreen::mouseClicked( int x, int y, int buttonNum ) {
	if(currentOptionPane != NULL)
		currentOptionPane->mouseClicked(minecraft, x, y, buttonNum);
	super::mouseClicked(x, y, buttonNum);
}

void OptionsScreen::mouseReleased( int x, int y, int buttonNum ) {
	if(currentOptionPane != NULL)
		currentOptionPane->mouseReleased(minecraft, x, y, buttonNum);
	super::mouseReleased(x, y, buttonNum);
}

void OptionsScreen::tick() {
	if(currentOptionPane != NULL)
		currentOptionPane->tick(minecraft);
	super::tick();
}
