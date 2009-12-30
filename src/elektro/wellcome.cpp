#include "wellcome.h"

#include <SDL_mouse.h>

#include <guichan/widgets/label.hpp>

#include "gui/widgets/button.h"
#include "gui/gui.h"
#include "gui/viewport.h"
#include "elektro/bitbutton.h"
#include "elektro/imagewidget.h"

#include "../game.h"

#include "utils/stringutils.h"
#include "../resources/resourcemanager.h"
#include "../graphics.h"
#include "log.h"
#include "net/ea/npchandler.h"

#include "../npc.h"

#include "utils/xml.h"
#include <libxml/xmlwriter.h>
#include "configuration.h"

Wellcome::Wellcome():
    Window("Wellcome")
{
    setResizable(false);
    setDefaultSize(200, 100, 420, 420);
    loadWindowState();
    setResizable(false);
    ResourceManager *resman = ResourceManager::getInstance();
    mSlide = resman->getImage("graphics/sunular/basla1/g_00.png");

    setVisible(true);
    mNext = new Button("İleri","next",this);
    mPrev = new Button("Geri","prev",this);
    mFinish = new Button("Kapat","close",this);

    mNext->setPosition(210,0);
    add(mNext);

    mPrev->setPosition(180,0);
    add(mPrev);

    mFinish->setPosition(370,0);
    add(mFinish);

    mList.push_back("graphics/sunular/basla1/g_00.png");
    mList.push_back("graphics/sunular/basla1/g_01.png");
    mList.push_back("graphics/sunular/basla1/g_02.png");
    mList.push_back("graphics/sunular/basla1/g_03.png");
    mList.push_back("graphics/sunular/basla1/g_04.png");
    mList.push_back("graphics/sunular/basla1/g_05.png");
    mList.push_back("graphics/sunular/basla1/g_06.png");
    mList.push_back("graphics/sunular/basla1/g_07.png");
    mList.push_back("graphics/sunular/basla1/g_08.png");
    mList.push_back("graphics/sunular/basla1/g_09.png");
    mList.push_back("graphics/sunular/basla1/g_10.png");
    mCurrentSlide = 0;
    current_npc = 0;
    NPC::isTalking = false;
}

void
Wellcome::draw(gcn::Graphics* graphics)
{
    Window::draw(graphics);
    Graphics *g = static_cast<Graphics*>(graphics);
    g->drawImage(mSlide,10,20);
    drawChildren(graphics);
}

void
Wellcome::action(const gcn::ActionEvent &event)
{
    //ResourceManager *resman = ResourceManager::getInstance();
    if (event.getId() == "close")
    {
        config.setValue("Wellcome", false);
        scheduleDelete();
    }
    else if (event.getId() == "next")
    {
        if (mCurrentSlide< mList.size()-1)
        {
            mCurrentSlide++;
            load();
        }
    }    else if (event.getId() == "prev")
    {
        if (mCurrentSlide> 0)
        {
            mCurrentSlide--;
            load();
        }
    }
}

void
Wellcome::load()
{
    ResourceManager *resman = ResourceManager::getInstance();
    mSlide->decRef();
    mSlide = resman->getImage(mList.at(mCurrentSlide));
}

void
Wellcome::mouseMoved(gcn::MouseEvent &event)
{
    Window::mouseMoved(event);
}

void
Wellcome::mouseExited(gcn::MouseEvent &event)
{
}
