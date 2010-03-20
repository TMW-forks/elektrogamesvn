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

#include "hitabe.h"




#include "gui/gui.h"
#include "gui/viewport.h"

#include "game.h"
#include "engine.h"
#include "graphics.h"
#include "resources/image.h"



#include "utils/stringutils.h"
#include "npc.h"
#include "sound.h"

//extern NPC *current_npc;
extern int current_npc;
Hitabe::Hitabe():
    Window("Uclu Set")
{
    setWindowName("Üçlü Set");
    setCloseButton(false);

    setResizable(false);
//!
//işe yaramıyor
//    setDefaultSize(10, 10, 718, 410);

    ResourceManager *resman = ResourceManager::getInstance();
    mBase = resman->getImage("graphics/images/hitabemars.png");

    sound.fadeInMusic("mars.ogg",100);

    Button *closeButton = new Button(_("close"),"close",this);
    closeButton->setPosition(320,360);
    setSize(700,420);
    setPosition((getWidth()-mBase->getWidth())/2,(getHeight()-mBase->getHeight())/2);
    add(closeButton);
}

void
Hitabe::logic()
{
//
}

Hitabe::~Hitabe()
{
    ResourceManager *resman = ResourceManager::getInstance();
    resman->release(mBase);
//
}

void
Hitabe::draw(gcn::Graphics *graphics)
{
    Window::draw(graphics);

    Graphics *g = static_cast<Graphics*>(graphics);
    g->drawImage(mBase,25,25);
}

void
Hitabe::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "close")
    {
        Net::getNpcHandler()->closeDialog(current_npc);
        setVisible(false);
        //if (current_npc) Net::getNpcHandler()->nextDialog(current_npc);
        current_npc = 0;
        sound.stopMusic();
        NPC::isTalking = false;
        scheduleDelete();
    }
}
