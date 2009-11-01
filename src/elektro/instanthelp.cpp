#include "instanthelp.h"

#include <SDL_mouse.h>

#include <guichan/widgets/label.hpp>

#include "gui/widgets/button.h"
#include "gui/gui.h"
#include "gui/viewport.h"
#include "elektro/bitbutton.h"

#include "../game.h"
#include "../engine.h"
#include "../particle.h"
#include "../map.h"

#include "utils/stringutils.h"
#include "elektro/lang_tr.h"
#include "../resources/resourcemanager.h"
#include "../graphics.h"

#include "../resources/image.h"

InstantHelp::InstantHelp():
    Window("Yardım")
{
    setResizable(false);
    setDefaultSize(1010-220, 754-420, 210, 410);
    loadWindowState();

    ResourceManager *resman = ResourceManager::getInstance();
    sayfaImg = resman->getImage("graphics/images/help1.png");
    sayfa=1;

    s1=new BitButton("com_rotate_btn.png","Hint", "s1", this);
    s1->setPosition(60,0);
    s1->setEnabled(true);

    s2=new BitButton("com_rotate_btn.png","Hint", "s2", this);
    s2->setPosition(80,0);
    s2->setEnabled(true);

    s3=new BitButton("com_rotate_btn.png","Hint", "s3", this);
    s3->setPosition(100,0);
    s3->setEnabled(true);

    s4=new BitButton("com_rotate_btn.png","Hint", "s4", this);
    s4->setPosition(120,0);
    s4->setEnabled(true);

    s5=new BitButton("com_rotate_btn.png","Hint", "s5", this);
    s5->setPosition(140,0);
    s5->setEnabled(true);

    add(s1);
    add(s2);
    add(s3);
    add(s4);
    add(s5);
}

void
InstantHelp::logic()
{

}

void
InstantHelp::draw(gcn::Graphics* graphics)
{
    Window::draw(graphics);
    Graphics *g = static_cast<Graphics*>(graphics);
    g->drawImage(sayfaImg,10,40);
}
void
InstantHelp::action(const gcn::ActionEvent &event)
{
    ResourceManager *resman = ResourceManager::getInstance();

    if (event.getId() == "close")
    {
        setVisible(false);
    } else
    if (event.getId() == "s1")
    {
        resman->release(sayfaImg);
        sayfaImg = resman->getImage("graphics/images/help1.png");
        sayfa = 1;
    } else
    if (event.getId() == "s2")
    {
        resman->release(sayfaImg);
        sayfaImg = resman->getImage("graphics/images/help2.png");
        sayfa = 2;
    } else
    if (event.getId() == "s3")
    {
        resman->release(sayfaImg);
        sayfaImg = resman->getImage("graphics/images/help3.png");
        sayfa = 3;
    } else
    if (event.getId() == "s4")
    {
        resman->release(sayfaImg);
        sayfaImg = resman->getImage("graphics/images/help4.png");
        sayfa = 4;
    } else
    if (event.getId() == "s5")
    {
        resman->release(sayfaImg);
        sayfaImg = resman->getImage("graphics/images/help5.png");
        sayfa = 5;
    }
}
