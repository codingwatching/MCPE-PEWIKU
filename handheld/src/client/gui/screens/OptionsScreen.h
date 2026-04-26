#ifndef NET_MINECRAFT_CLIENT_GUI_SCREENS__OptionsScreen_H__
#define NET_MINECRAFT_CLIENT_GUI_SCREENS__OptionsScreen_H__

#include "../Screen.h"
#include "../components/Button.h"
#include "../components/touch/TButton.h"
#include "../components/touch/THeader.h"
#include <vector>

class ImageButton;
class OptionsPane;
class CategoryButton;
class NinePatchLayer;

class OptionsScreen: public Screen
{
	typedef Screen super;
	void init();

	void generateOptionScreens();
	void createCategoryButtons();

public:
	OptionsScreen(int initialCategory = 0);
	~OptionsScreen();
	void setupPositions();
	void buttonClicked( Button* button );
	void render(int xm, int ym, float a);
	void removed();
	void selectCategory(int index);

	virtual void mouseClicked( int x, int y, int buttonNum );
	virtual void mouseReleased( int x, int y, int buttonNum );
	virtual void tick();
private:
	Touch::THeader* bHeader;
	ImageButton* btnClose;
	
	std::vector<CategoryButton*> categoryButtons;
	std::vector<OptionsPane*> optionPanes;
	OptionsPane* currentOptionPane;
	int selectedCategory;
	int initialCategory;

	NinePatchLayer* normalBg;
	NinePatchLayer* selectedBg;
	Button* selectedCategoryButton;
};

#endif /*NET_MINECRAFT_CLIENT_GUI_SCREENS__OptionsScreen_H__*/
