#ifndef NET_MINECRAFT_CLIENT_GUI_COMPONENTS__CategoryButton_H__
#define NET_MINECRAFT_CLIENT_GUI_COMPONENTS__CategoryButton_H__

#include "ImageButton.h"
#include "NinePatch.h"

class CategoryButton : public ImageButton {
    typedef ImageButton super;
public:
    Button** selectedButtonPtr;
    NinePatchLayer *bg, *selectedBg;

    CategoryButton(int id, NinePatchLayer* fbg, NinePatchLayer* fselectedBg, Button** fselectedButtonPtr);
    CategoryButton(const std::string& msg, int id, NinePatchLayer* fbg, NinePatchLayer* fselectedBg, Button** fselectedButtonPtr);
    virtual ~CategoryButton();
    virtual void renderBg(Minecraft* minecraft, int xm, int ym);
    virtual bool isSecondImage(bool hovered);
};

#endif /* NET_MINECRAFT_CLIENT_GUI_COMPONENTS__CategoryButton_H__ */
