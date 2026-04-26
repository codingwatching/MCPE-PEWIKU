#include "CategoryButton.h"
#include "../../renderer/Tesselator.h"

CategoryButton::CategoryButton(int id, NinePatchLayer* fbg, NinePatchLayer* fselectedBg, Button** fselectedButtonPtr)
    : CategoryButton("", id, fbg, fselectedBg, fselectedButtonPtr)
{
}

CategoryButton::CategoryButton(const std::string& msg, int id, NinePatchLayer* fbg, NinePatchLayer* fselectedBg, Button** fselectedButtonPtr)
    : ImageButton(id, msg)
{
    bg = fbg;
    selectedBg = fselectedBg;
    selectedButtonPtr = fselectedButtonPtr;
}

CategoryButton::~CategoryButton()
{
}

void CategoryButton::renderBg(Minecraft* minecraft, int xm, int ym)
{
    NinePatchLayer* layer;
    if ((active && _currentlyDown && hovered(minecraft, xm, ym)) || (selectedButtonPtr && *selectedButtonPtr == this)) {
        layer = selectedBg;
    } else {
        layer = bg;
    }

    if (layer) {
        layer->draw(Tesselator::instance, (float)x, (float)y);
    }
}

bool CategoryButton::isSecondImage(bool hovered)
{
    return false;
}
