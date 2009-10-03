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
 *  $Id: window.cpp 3271 2007-04-15 19:19:01Z b_lindeijer $
 */

#include "elektro/window_iki.h"

#include <guichan/exception.hpp>
#include <guichan/widgets/icon.hpp>

//#include "gui/gccontainer.h"

#include "gui/widgets/resizegrip.h"
#include "gui/widgets/windowcontainer.h"
#include "../configlistener.h"
#include "../configuration.h"
#include "../graphics.h"
#include "../log.h"


#include "../resources/image.h"
#include "../resources/resourcemanager.h"

ConfigListener *Windowiki::windowikiConfigListener = 0;
WindowContainer *Windowiki::windowikiContainer = 0;
int Windowiki::instancesiki = 0;
ImageRect Windowiki::borderiki;
Image *Windowiki::closeImage = NULL;
Image *Windowiki::solustImage = NULL;
Image *Windowiki::sagustImage = NULL;
Image *Windowiki::sagaltImage = NULL;
Image *Windowiki::solaltImage = NULL;

class WindowikiConfigListener : public ConfigListener
{

    void optionChanged(const std::string &)
    {
        for_each(Windowiki::borderiki.grid, Windowiki::borderiki.grid + 9,
                std::bind2nd(std::mem_fun(&Image::setAlpha),
                    config.getValue("guialpha", 0.8)));
    }

};

Windowiki::Windowiki(const std::string& caption, bool modal, Windowiki *parent):
    gcn::Window(caption),
    mGrip(0),
    mParent(parent),
    mWindowName("Window iki"),
    mShowTitle(true),
    mModal(modal),
    mResizable(true),
    mMouseResize(0),
    mSticky(false),
    mMinWinWidth(125),
    mMinWinHeight(250),
    mMaxWinWidth(INT_MAX),
    mMaxWinHeight(INT_MAX),
    mCloseButton(false)
{



   if (!windowikiContainer) {
        throw GCN_EXCEPTION("Window::Window. no windowContainer set");
   }
    if (instancesiki == 0)
    {
        // Load static resources
        ResourceManager *resman = ResourceManager::getInstance();
        Image *dBorders = resman->getImage("graphics/gui/ppp.png");
        borderiki.grid[0] = dBorders->getSubImage(0, 0, 15, 24); //sol üst taraf
        borderiki.grid[1] = dBorders->getSubImage(50, 0, 5, 48); //üst tarafýn devamlýlýðý
        borderiki.grid[2] = dBorders->getSubImage(15, 0, 15, 25); // sað üst taraf
        borderiki.grid[3] = dBorders->getSubImage(0, 110, 44, 5); // sol taraf devamlýlýk
        borderiki.grid[4] = resman->getImage("graphics/gui/ff.png");
        borderiki.grid[5] = dBorders->getSubImage(115, 210, 42, 10); // sað taraf devamlýlýk
        borderiki.grid[6] = dBorders->getSubImage(0, 30, 15, 30); //sol alt taraf
        borderiki.grid[7] = dBorders->getSubImage(89, 0, 5, 61); //alt taraf devamlýlýk
        borderiki.grid[8] = dBorders->getSubImage(0, 15, 0, 55); //sað alt taraf
        dBorders->decRef();

        closeImage = resman->getImage("graphics/gui/close_button.png");
        solustImage = resman->getImage("graphics/gui/solust.png");
        sagustImage = resman->getImage("graphics/gui/sagust.png");
        solaltImage = resman->getImage("graphics/gui/solalt.png");
        sagaltImage = resman->getImage("graphics/gui/sagalt.png");

        windowikiConfigListener = new WindowikiConfigListener();
        // Send GUI alpha changed for initialization
//þeffaf pencereyi kapattým.
//        windowikiConfigListener->optionChanged("guialpha");
//        config.addListener("guialpha", windowikiConfigListener);
    }
    instancesiki++;
    setPadding(5);
    setTitleBarHeight(30);
    // Add chrome
//    mChrome = new GCContainer();
//    mChrome->setOpaque(false);
//    gcn::Window::add(mChrome);

    // Add this window to the window container

    windowikiContainer->add(this);

    if (mModal)
    {
        requestModalFocus();
    }

    // Windows are invisible by default
    setVisible(false);
}

Windowiki::~Windowiki()
{
    logger->log("Window::~Window(\"%s\")", getCaption().c_str());
    const std::string &name = mWindowName;

    // Saving X, Y and Width and Height for resizables in the config
    config.setValue(name + "WinX", getX());
    config.setValue(name + "WinY", getY());

    if (mResizable)
    {
        config.setValue(name + "WinWidth", getWidth());
        config.setValue(name + "WinHeight", getHeight());
    }

    instancesiki--;

    if (instancesiki == 0)
    {
        config.removeListener("guialpha", windowikiConfigListener);
        delete windowikiConfigListener;
        windowikiConfigListener = NULL;

        // Clean up static resources
        delete borderiki.grid[0];
        delete borderiki.grid[1];
        delete borderiki.grid[2];
        delete borderiki.grid[3];
        borderiki.grid[4]->decRef();
        delete borderiki.grid[5];
        delete borderiki.grid[6];
        delete borderiki.grid[7];
        delete borderiki.grid[8];
        closeImage->decRef();
        solustImage->decRef();
        sagustImage->decRef();
        solaltImage->decRef();
        sagaltImage->decRef();

    }

    delete mChrome;
    delete mGrip;
}

void Windowiki::setWindowContainer(WindowContainer *wc)
{
    windowikiContainer = wc;
}

void
Windowiki::action(const gcn::ActionEvent &event)
{

}
void Windowiki::draw(gcn::Graphics *graphics)
{
    Graphics *g = static_cast<Graphics*>(graphics);

    g->drawImageRect(0, 0, getWidth(), getHeight(), borderiki);

    // Draw title
    if (mShowTitle)
    {
        graphics->setFont(getFont());
        graphics->drawText(getCaption(), 7, 5, gcn::Graphics::LEFT);
    }

    g->drawImage(sagustImage, getWidth() - sagustImage->getWidth(),
            0);
    if (mCloseButton)
    {
        g->drawImage(closeImage,
            getWidth() - closeImage->getWidth(),
            0
        );
    }
    g->drawImage(solustImage, 0,0);

    g->drawImage(sagaltImage, getWidth() - sagaltImage->getWidth(),
            getHeight() - sagaltImage->getHeight());
    g->drawImage(solaltImage, 0,getHeight() - solaltImage->getHeight() );
    drawChildren(graphics);
}

void Windowiki::setContentWidth(int width)
{
//    mChrome->setWidth(width);
    setWidth(width + 2 * getPadding());
}

void Windowiki::setContentHeight(int height)
{
//    mChrome->setHeight(height);
    setHeight(height + getPadding() + getTitleBarHeight());
}

void Windowiki::setContentSize(int width, int height)
{
    setContentWidth(width);
    setContentHeight(height);
}

void Windowiki::setWidth(int width)
{
    gcn::Window::setWidth(width);

    if (mGrip)
    {
        mGrip->setX(getWidth() - mGrip->getWidth() - getChildrenArea().x);
    }
}

void Windowiki::setHeight(int height)
{
    gcn::Window::setHeight(height);

    if (mGrip)
    {
        mGrip->setY(getHeight() - mGrip->getHeight() - getChildrenArea().y);
    }
}

void Windowiki::setDimension(const gcn::Rectangle &dimension)
{
    gcn::Window::setDimension(dimension);

    if (mGrip)
    {
        mGrip->setX(getWidth() - mGrip->getWidth() - getChildrenArea().x);
        mGrip->setY(getHeight() - mGrip->getHeight() - getChildrenArea().y);
    }
}

void Windowiki::setLocationRelativeTo(gcn::Widget* widget)
{
    int wx, wy;
    int x, y;

    widget->getAbsolutePosition(wx, wy);
    getAbsolutePosition(x, y);

    setPosition(getX() + (wx + (widget->getWidth() - getWidth()) / 2 - x),
                getY() + (wy + (widget->getHeight() - getHeight()) / 2 - y));
}

void Windowiki::setMinWidth(unsigned int width)
{
    mMinWinWidth = width;
}

void Windowiki::setMinHeight(unsigned int height)
{
    mMinWinHeight = height;
}

void Windowiki::setMaxWidth(unsigned int width)
{
    mMaxWinWidth = width;
}

void Windowiki::setMaxHeight(unsigned int height)
{
    mMaxWinHeight = height;
}

void Windowiki::setResizable(bool r)
{
    mResizable = r;

    if (mResizable)
   {
        mGrip = new ResizeGrip();
        mGrip->setX(getWidth() - mGrip->getWidth() - getChildrenArea().x);
        mGrip->setY(getHeight() - mGrip->getHeight() - getChildrenArea().y);
        gcn::Window::add(mGrip);
    }
    else
    {
        delete mGrip;
        mGrip = 0;
    }
}

bool Windowiki::isResizable()
{
    return mResizable;
}

void Windowiki::setSticky(bool sticky)
{
    mSticky = sticky;
}

void Windowiki::setCloseButton(bool flag)
{
    mCloseButton = flag;
}

bool Windowiki::isSticky()
{
    return mSticky;
}

void Windowiki::setVisible(bool visible)
{
    if (isSticky())
    {
        gcn::Window::setVisible(true);
    }
    else
    {
        gcn::Window::setVisible(visible);
    }
}

void Windowiki::scheduleDelete()
{
    windowikiContainer->scheduleDelete(this);
}

void Windowiki::add(gcn::Widget *w, bool delChild)
{
    mChrome->add(w, delChild);
}

void Windowiki::add(gcn::Widget *w, int x, int y, bool delChild)
{
    mChrome->add(w, x, y, delChild);
}

void Windowiki::mousePressed(gcn::MouseEvent &event)
{
    // Let Guichan move window to top and figure out title bar drag
    gcn::Window::mousePressed(event);

    const int x = event.getX();
    const int y = event.getY();
    mMouseResize = 0;

    // Activate resizing handles as appropriate
    if (event.getSource() == this && isResizable() &&
            event.getButton() == gcn::MouseEvent::LEFT &&
            !getChildrenArea().isPointInRect(x, y))
    {
        mMouseResize |= (x > getWidth() - resizeBorderWidthiki) ? RIGHT :
                        (x < resizeBorderWidthiki) ? LEFT : 0;
        mMouseResize |= (y > getHeight() - resizeBorderWidthiki) ? BOTTOM :
                        (y < resizeBorderWidthiki) ? TOP : 0;
    }
    else if (event.getSource() == mGrip)
    {
        mDragOffsetX = x + mGrip->getX();
        mDragOffsetY = y + mGrip->getY();
        mMouseResize |= BOTTOM | RIGHT;
        mIsMoving = false;
    }

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        const int x = event.getX();
        const int y = event.getY();

        // Handle close button
        if (mCloseButton)
        {
            gcn::Rectangle closeButtonRect(
                getWidth() - closeImage->getWidth() - getPadding(),
                getPadding(),
                closeImage->getWidth(),
                closeImage->getHeight());

            if (closeButtonRect.isPointInRect(x, y))
            {
                setVisible(false);
            }
        }

        // Handle window resizing
    }
}

void Windowiki::mouseDragged(gcn::MouseEvent &event)
{
    // Let Guichan handle title bar drag
    gcn::Window::mouseDragged(event);

    // Keep guichan window inside screen when it may be moved
    if (isMovable() && mIsMoving)
    {
        int newX = std::max(0, getX());
        int newY = std::max(0, getY());
        newX = std::min(windowikiContainer->getWidth() - getWidth(), newX);
        newY = std::min(windowikiContainer->getHeight() - getHeight(), newY);
        setPosition(newX, newY);
    }

    if (mMouseResize && !mIsMoving)
    {
        const int dx = event.getX() - mDragOffsetX +
            ((event.getSource() == mGrip) ? mGrip->getX() : 0);
        const int dy = event.getY() - mDragOffsetY +
            ((event.getSource() == mGrip) ? mGrip->getY() : 0);
        gcn::Rectangle newDim = getDimension();

        if (mMouseResize & (TOP | BOTTOM))
        {
            int newHeight = newDim.height + ((mMouseResize & TOP) ? -dy : dy);
            newDim.height = std::min(mMaxWinHeight,
                                     std::max(mMinWinHeight, newHeight));

            if (mMouseResize & TOP)
            {
                newDim.y -= newDim.height - getHeight();
            }
        }

        if (mMouseResize & (LEFT | RIGHT))
        {
            int newWidth = newDim.width + ((mMouseResize & LEFT) ? -dx : dx);
            newDim.width = std::min(mMaxWinWidth,
                                    std::max(mMinWinWidth, newWidth));

            if (mMouseResize & LEFT)
            {
                newDim.x -= newDim.width - getWidth();
            }
        }

        // Keep guichan window inside screen (supports resizing any side)
        if (newDim.x < 0)
        {
            newDim.width += newDim.x;
            newDim.x = 0;
        }
        if (newDim.y < 0)
        {
            newDim.height += newDim.y;
            newDim.y = 0;
        }
        if (newDim.x + newDim.width > windowikiContainer->getWidth())
        {
            newDim.width = windowikiContainer->getWidth() - newDim.x;
        }
        if (newDim.y + newDim.height > windowikiContainer->getHeight())
        {
            newDim.height = windowikiContainer->getHeight() - newDim.y;
        }

        // Update mouse offset when dragging bottom or right border
        if (mMouseResize & BOTTOM)
        {
            mDragOffsetY += newDim.height - getHeight();
        }
        if (mMouseResize & RIGHT)
        {
            mDragOffsetX += newDim.width - getWidth();
        }

        // Set the new window and content dimensions
        setDimension(newDim);
        const gcn::Rectangle area = getChildrenArea();
        mChrome->setSize(area.width, area.height);
    }
}

void
Windowiki::loadWindowState()
{
    const std::string &name = mWindowName;

    setPosition((int) config.getValue(name + "WinX", getX()),
                (int) config.getValue(name + "WinY", getY()));

    if (mResizable)
    {
        setSize((int) config.getValue(name + "WinWidth", getWidth()),
                (int) config.getValue(name + "WinHeight", getHeight()));

        const gcn::Rectangle area = getChildrenArea();
        mChrome->setSize(area.width, area.height);
    }
}

void Windowiki::setDefaultSize(int defaultX, int defaultY,
                            int defaultWidth, int defaultHeight)
{
    mDefaultX = defaultX;
    mDefaultY = defaultY;
    mDefaultWidth = defaultWidth;
    mDefaultHeight = defaultHeight;

    setPosition(mDefaultX, mDefaultY);
    setContentSize(mDefaultWidth, mDefaultHeight);
}

void Windowiki::resetToDefaultSize()
{
    setPosition(mDefaultX, mDefaultY);
    setContentSize(mDefaultWidth, mDefaultHeight);
}
