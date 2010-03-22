/*
 *  The Mana World
 *  Copyright 2004 The Mana World Development Team
 *
 *  This file is part of The Mana World.
 *
 *  The Mana World is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  The Mana World is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with The Mana World; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  $Id: debugwindow.cpp 3290 2007-05-04 13:09:25Z crush_tmw $
 */

#include "rescolorwindow.h"




#include "gui/gui.h"
#include "gui/viewport.h"

#include "game.h"
#include "engine.h"
#include "particle.h"
#include "map.h"
#include "graphics.h"
#include "resources/image.h"
#include <guichan/listmodel.hpp>
#include "gui/widgets/scrollarea.h"



#include "utils/stringutils.h"
#include "elektro/lang_tr.h"
#include "../npc.h"
#include "net/ea/npchandler.h"

extern int current_npc;

class ModeListModelZ : public gcn::ListModel
{
    public:
        ModeListModelZ()
            {
                mVideoModes.push_back("0-Siyah");
                mVideoModes.push_back("1-Kahverengi");
                mVideoModes.push_back("2-Kirmizi");
                mVideoModes.push_back("3-Turuncu");
                mVideoModes.push_back("4-Sari");
                mVideoModes.push_back("5-Yesil");
                mVideoModes.push_back("6-Mavi");
                mVideoModes.push_back("7-Mor");
                mVideoModes.push_back("8-Gri");
                mVideoModes.push_back("9-Beyaz");
            }
        virtual ~ModeListModelZ() { }
        int getNumberOfElements() { return mVideoModes.size(); }
        std::string getElementAt(int i) { return mVideoModes[i]; }
    private:
        std::vector<std::string> mVideoModes;
};

class ModeListModelY : public gcn::ListModel
{
    public:
        ModeListModelY()
            {
                mVideoModes.push_back("+/-%5-Altin");
                mVideoModes.push_back("+/-%10-Gumus");
                mVideoModes.push_back("+/-%20-Yok");
            }
        virtual ~ModeListModelY() { }
        int getNumberOfElements() { return mVideoModes.size(); }
        std::string getElementAt(int i) { return mVideoModes[i]; }
    private:
        std::vector<std::string> mVideoModes;
};

ResColorWindow::ResColorWindow():
    Window("Direnc Renkleri")
{
    setWindowName("Direnc renkleri");

    setResizable(false);
    setDefaultSize(150, 150, 480, 400);
    loadWindowState();

    ResourceManager *resman = ResourceManager::getInstance();
    mSet1 = resman->getImageSet("graphics/elektrik/res1set.png",21,61);
    mSet2 = resman->getImageSet("graphics/elektrik/res2set.png",21,61);
    mBase = resman->getImage("graphics/elektrik/emptyres.png");

    mCol1=0;
    mCol2=0;
    mCol3=0;
    mCol4=0;

    mModeListModelZ=new ModeListModelZ();
    mModeList1= new ListBox(mModeListModelZ);
    mModeList2= new ListBox(mModeListModelZ);
    mModeList3= new ListBox(mModeListModelZ);

    mModeListModelY=new ModeListModelY();
    mModeList4= new ListBox(mModeListModelY);

    ScrollArea *scrollArea1 = new ScrollArea(mModeList1);
    ScrollArea *scrollArea2 = new ScrollArea(mModeList2);
    ScrollArea *scrollArea3 = new ScrollArea(mModeList3);
    ScrollArea *scrollArea4 = new ScrollArea(mModeList4);

    mModeList1->setEnabled(true);
    mModeList2->setEnabled(true);
    mModeList3->setEnabled(true);
    mModeList4->setEnabled(true);

    mModeList1->setSelected(0);
    mModeList2->setSelected(0);
    mModeList3->setSelected(0);
    mModeList4->setSelected(0);

    int w=80,h=135;

    mModeList1->setDimension(gcn::Rectangle(0, 0, w, h));
    scrollArea1->setDimension(gcn::Rectangle(55, 175, w, h));

    mModeList2->setDimension(gcn::Rectangle(0, 0, w, h));
    scrollArea2->setDimension(gcn::Rectangle(165, 175, w, h));

    mModeList3->setDimension(gcn::Rectangle(0, 0, w, h));
    scrollArea3->setDimension(gcn::Rectangle(255, 175, w, h));

    mModeList4->setDimension(gcn::Rectangle(0, 0, w, 50));
    scrollArea4->setDimension(gcn::Rectangle(345, 175, w, 50));

    mValueLabel = new gcn::Label("0 Ω ± %10");
    mValueLabel->setFont(font_bas_2_20);
    mValueLabel->setForegroundColor(gcn::Color(150,120,70));
    mValueLabel->setPosition(200,30);
    mValueLabel->adjustSize();

    Button *closeButton = new Button(BTNCLOSE,"close",this);
    closeButton->setPosition(230,330);
    add(closeButton);
    add(scrollArea1);
    add(scrollArea2);
    add(scrollArea3);
    add(scrollArea4);
    add(mValueLabel);
}

void
ResColorWindow::logic()
{
    std::stringstream temp;
    if (mModeList1->getSelected()!=0) temp<<mModeList1->getSelected();
    temp<<mModeList2->getSelected();
    switch (mModeList3->getSelected())
    {
    case 0 :break;
    case 1 :temp<<"0 ";break;
    case 2 :temp<<"00 ";break;
    case 3 :temp<<" K";break;
    case 4 :temp<<"0 K";break;
    case 5 :temp<<"00 K";break;
    case 6 :temp<<" M";break;
    case 7 :temp<<"0 M";break;
    case 8 :temp<<"00 M";break;
    case 9 :temp<<"000 M";break;
    }
    switch (mModeList4->getSelected())
    {
    case 0 :temp<<"Ω ± %5";break;
    case 1 :temp<<"Ω ± %10";break;
    case 2 :temp<<"Ω ± %20";break;
    }


 value = temp.str();
 mValueLabel->setCaption(value.c_str());
 mValueLabel->adjustSize();
}

void
ResColorWindow::draw(gcn::Graphics *graphics)
{

    Window::draw(graphics);

    Graphics *g = static_cast<Graphics*>(graphics);
    int x=198;
    g->drawImage(this->mBase,80,70);
    g->drawImage(this->mSet1->get(mModeList1->getSelected()),x,80); x+=21;
    g->drawImage(this->mSet1->get(mModeList2->getSelected()),x,80); x+=21;
    g->drawImage(this->mSet1->get(mModeList3->getSelected()),x,80); x+=35;
    g->drawImage(this->mSet2->get(mModeList4->getSelected()),x,80);
}

void
ResColorWindow::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "close")
    {
        setVisible(false);
        current_npc=0;
        NPC::isTalking = false;
        scheduleDelete();

    }
}
