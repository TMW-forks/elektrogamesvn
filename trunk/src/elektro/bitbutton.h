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
 *  $Id: bitbutton.h 3152 2007-02-27 16:31:34Z crush_tmw $
 */

#ifndef _TMW_BIT_BUTTON_H
#define _TMW_BIT_BUTTON_H

#include <iosfwd>

#include <guichan/widgets/button.hpp>
#include "guichan/mouseevent.hpp"
#include "guichan/platform.hpp"
#include "guichan/widget.hpp"
#include "guichan/keylistener.hpp"
#include "../resources/imageset.h"

class ImageRect;

/**
 * Button widget. Same as the Guichan button but with custom look.
 *
 * \ingroup GUI
 */
class BitButton : public gcn::Button {
    public:
        /**
         * Constructor, sets the caption of the button to the given string.
         */
        BitButton(const std::string& caption, const std::string& hint,
                  const std::string &actionEventId,gcn::ActionListener *listener);

        /**
         * Destructor.
         */
        virtual ~BitButton();

        /**
         * Draws the button.
         */
        virtual void draw(gcn::Graphics* graphics);

        /**
         * Enable/Disable highlighting
         */
        void setLogged(bool enable)
        { mIsLogged = enable; }

        /**
         * set image set name
         */
        void setImageName(const std::string& name);

        /**
         * set imageset
         */
        void setImageSet (ImageSet& imgset);

        /**
         * set hint string
         */
        void setHint(const std::string& h);

        /**
         * get hint
         */
        std::string getHint();

        /**
         * for hint
         */

        void setInterval (int inter);

        int getInterval ();

        virtual void mouseEntered(gcn::MouseEvent& mouseEvent);

        virtual void mouseExited(gcn::MouseEvent& mouseEvent);

        virtual void mouseReleased(gcn::MouseEvent& mouseEvent);

        virtual void mousePressed(gcn::MouseEvent& mouseEvent);

        virtual void mouseDragged(gcn::MouseEvent& mouseEvent);

        virtual void setScroll (bool scr);

    private:
        int firstX;
        int firstY;
        int mInterval;

        std::string mHint;
        std::string mAction;
        ImageSet *mImageSet;
        static int mInstances;         /**< Number of button instances */
        bool mIsLogged;                /**< Makes the button appear pressed all the time */
        bool mScroll;
        bool mDragging;
};

#endif
