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
 *  $Id: button.cpp 3152 2007-02-27 16:31:34Z crush_tmw $
 */

#include "bitbutton.h"

#include <guichan/exception.hpp>
#include <guichan/graphics.hpp>
#include <guichan/imagefont.hpp>
#include "guichan/mouseevent.hpp"
#include "guichan/mouselistener.hpp"

#include "../graphics.h"

#include "../resources/image.h"
#include "../resources/resourcemanager.h"

#include "../utils/dtor.h"
#include "../utils/stringutils.h"
#include "../log.h"

int BitButton::mInstances = 0;
//extern std::string globalHint;

BitButton::BitButton(const std::string& caption,const std::string& hint,
            const std::string &actionEventId,
            gcn::ActionListener *listener):
    mHint(hint),
    mIsLogged(false),
    mScroll(false),
    mDragging(false),
    mInterval(10)
{
    if (caption != "")
    {
        // Load the skin
        ResourceManager *resman = ResourceManager::getInstance();

        Image *temp = resman->getImage("graphics/elektrik/"+caption);
        resman->release(temp);

        mImageSet = resman->getImageSet("graphics/elektrik/"+caption,temp->getWidth()/4,temp->getHeight());
        setSize(temp->getWidth()/4,temp->getHeight());
    }


    mInstances++;
    setActionEventId(actionEventId);
    mAction = actionEventId;
    if (listener)
    {
        addActionListener(listener);
    }
}

BitButton::~BitButton()
{
    mInstances--;

    if (mInstances == 0)
    {
//        for (int mode = 0; mode < 4; mode++)
//        {
//            for_each(button[mode].grid, button[mode].grid + 9, dtor<Image*>());
//        }
    }
}

void
BitButton::draw(gcn::Graphics *graphics)
{
    int mode;

    if (!isEnabled()) {
        mode = 3;
    }
    else if (isPressed() || mIsLogged) {
        mode = 2;
    }
    else if (mDragging) {
        mode = 2;
     }
    else if (mHasMouse) {
        mode = 1;
     }
    else {
        mode = 0;
    }

    Graphics *g = static_cast<Graphics*>(graphics);
    g->drawImage(this->mImageSet->get(mode),0,0);
}

void
BitButton::setImageName(const std::string& name)
{
 if (name != "")
    {
        // Load the skin
        ResourceManager *resman = ResourceManager::getInstance();
        mImageSet = resman->getImageSet("graphics/gui/"+name,40,40);
    }
}

void BitButton::setImageSet (ImageSet& imgset)
{
    mImageSet = &imgset;
}

void BitButton::setHint(const std::string& h)
{
    mHint = h;
}

std::string BitButton::getHint()
{
    return mHint;
}

void BitButton::mouseEntered(gcn::MouseEvent& mouseEvent)
{
//        globalHint=mHint;
        gcn::Button::mouseEntered(mouseEvent);
}

void BitButton::mouseExited(gcn::MouseEvent& mouseEvent)
{
        if (!mDragging)
        {
//            globalHint="";
            gcn::Button::mouseExited(mouseEvent);
        }
}

void BitButton::mouseReleased(gcn::MouseEvent& mouseEvent)
{
//        globalHint="";
        mDragging=false;
        mHasMouse = false;
        gcn::Button::mouseReleased(mouseEvent);
}

void BitButton::mousePressed(gcn::MouseEvent& mouseEvent)
{
        mDragging=true;
        firstX = mouseEvent.getX();
        firstY = mouseEvent.getY();
        setActionEventId(mAction);
//        generateAction();
        gcn::Button::mousePressed(mouseEvent);
}

void BitButton::mouseDragged(gcn::MouseEvent& mouseEvent)
{
    if (mScroll)
    {
        if (mouseEvent.getY()-firstY>getInterval())
            {
                const std::string &actionEventId=mAction+"_y-";
                setActionEventId(actionEventId);
//                generateAction();
                firstY = mouseEvent.getY();
            }

        if (mouseEvent.getY()-firstY<-getInterval())
            {
                const std::string &actionEventId=mAction+"_y+";
                setActionEventId(actionEventId);
//                generateAction();
                firstY = mouseEvent.getY();
            }
        if (mouseEvent.getX()-firstX<-getInterval())
            {
                const std::string &actionEventId=mAction+"_x-";
                setActionEventId(actionEventId);
//                generateAction();
                firstX = mouseEvent.getX();
            }
        if (mouseEvent.getX()-firstX>getInterval())
            {
                const std::string &actionEventId=mAction+"_x+";
                setActionEventId(actionEventId);
//                generateAction();
                firstX = mouseEvent.getX();
            }
        setActionEventId("");
        gcn::Button::mouseDragged(mouseEvent);
    }
}

void BitButton::setScroll (bool scr)
{
    mScroll = scr;
}

void BitButton::setInterval (int inter)
{
    mInterval=inter;
}

int BitButton::getInterval ()
{
    return mInterval;
}

// TODO (Nihal#1#): mask eklenerek button'un aktif alaný belirlenecek
