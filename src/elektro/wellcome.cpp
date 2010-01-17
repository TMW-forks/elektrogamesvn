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
    Window("Hoşgeldin :)")
{
    setResizable(false);
    setDefaultSize(50, 50, 420, 430);
    loadWindowState();
    setResizable(false);
    ResourceManager *resman = ResourceManager::getInstance();
    mSlide = resman->getImage("graphics/sunular/basla1/g_00.png");

    setVisible(true);
    mNext = new Button("İleri","next",this);
    mPrev = new Button("Geri","prev",this);
    mFinish = new Button("Kapat","close",this);

    mNext->setPosition(205, getHeight()- mNext->getHeight() - getTitleBarHeight());
    mNext->setWidth(100);
    add(mNext);

    mPrev->setPosition(105,getHeight()- mPrev->getHeight() - getTitleBarHeight());
    mPrev->setWidth(100);
    add(mPrev);

    mFinish->setPosition(330,0);
    mFinish->setWidth(100);
    add(mFinish);

    mAgain = new CheckBox("Girişte göster.",false);
    mAgain->setPosition(mNext->getX() + mNext->getWidth() + 2 ,getHeight()- mAgain->getHeight() - getTitleBarHeight()-5);
    add(mAgain);

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
    addActionListener(this);

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
        if (mAgain->isSelected())
        {
            config.setValue("Wellcome", 1);
            logger->log("Wellcome : 1");
        }
        else
        {
            config.setValue("Wellcome", 0);
            logger->log("Wellcome : 0");
        }
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
    if (mCurrentSlide == mList.size()-1) mNext->setEnabled(false);
    else  mNext->setEnabled(true);

    if (mCurrentSlide == 0) mPrev->setEnabled(false);
    else  mPrev->setEnabled(true);
}

void
Wellcome::mouseMoved(gcn::MouseEvent &event)
{
    Window::mouseMoved(event);
}

void
Wellcome::mousePressed(gcn::MouseEvent &event)
{
    gcn::Rectangle d1, d2, d3, d4;
    d1.setAll(mNext->getDimension().x,
              mNext->getDimension().y + getTitleBarHeight(),
              mNext->getDimension().x + mNext->getDimension().width,
              mNext->getDimension().y + mNext->getDimension().height + getTitleBarHeight());
    d2.setAll(mPrev->getDimension().x,
              mPrev->getDimension().y + getTitleBarHeight(),
              mPrev->getDimension().x + mPrev->getDimension().width,
              mPrev->getDimension().y + mPrev->getDimension().height + getTitleBarHeight());
    d3.setAll(mFinish->getDimension().x,
              mFinish->getDimension().y + getTitleBarHeight(),
              mFinish->getDimension().x + mFinish->getDimension().width,
              mFinish->getDimension().y + mFinish->getDimension().height + getTitleBarHeight());
    d4.setAll(mAgain->getDimension().x,
              mAgain->getDimension().y + getTitleBarHeight(),
              mAgain->getDimension().x + mAgain->getDimension().width,
              mAgain->getDimension().y + mAgain->getDimension().height + getTitleBarHeight());

    if(d1.isPointInRect(event.getX(), event.getY()) ||
       d2.isPointInRect(event.getX(), event.getY()) ||
       d3.isPointInRect(event.getX(), event.getY()) ||
       d4.isPointInRect(event.getX(), event.getY()) ||
       event.getY()<getTitleBarHeight())
    {
        Window::mousePressed(event);
        return;
    }
    setActionEventId("next");
    distributeActionEvent();

}

void
Wellcome::mouseExited(gcn::MouseEvent &event)
{
}
