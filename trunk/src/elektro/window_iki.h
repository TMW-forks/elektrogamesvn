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
 *  $Id: window.h 3270 2007-04-15 01:22:17Z b_lindeijer $
 */

#ifndef _TMW_WINDOWIKI_H__
#define _TMW_WINDOWIKI_H__

#include <guichan/widgets/window.hpp>
#include <guichan/actionlistener.hpp>

#include "../guichanfwd.h"
#include "../gui/widgets/button.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

class ConfigListener;
class GCContainer;
class ImageRect;
class ResizeGrip;
class WindowContainer;
class Image;

/**
 * A window. This window can be dragged around and has a title bar. Windows are
 * invisible by default.
 *
 * \ingroup GUI
 */
class Windowiki : public gcn::Window, public gcn::ActionListener
{
    public:
        friend class WindowikiConfigListener;

        /**
         * Constructor. Initializes the title to the given text and hooks
         * itself into the window container.
         *
         * @param text   The initial window title, "Window" by default.
         * @param modal  Block input to other windows.
         * @param parent The parent window. This is the window standing above
         *               this one in the window hiearchy. When reordering,
         *               a window will never go below its parent window.
         */
        Windowiki(const std::string &caption = "Window", bool modal = false,
                Windowiki *parent = NULL);

        /**
         * Destructor.
         */
        ~Windowiki();

        /**
         * Sets the window container to be used by new windows.
         */
        static void setWindowContainer(WindowContainer *windowikiContainer);

        /**
         * Draws the window.
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Adds a widget to the window.
         */
        void add(gcn::Widget *wi, bool delChild = true);

        /**
         * Adds a widget to the window and also specifices its position.
         */
        void add(gcn::Widget *w, int x, int y, bool delChild = true);

        /**
         * Sets the width of the window contents.
         */
        void setContentWidth(int width);

        void
        action(const gcn::ActionEvent &event);

        /**
         * Sets the height of the window contents.
         */
        void setContentHeight(int height);

        /**
         * Sets the size of this window.
         */
        void setContentSize(int width, int height);

        /**
         * Sets the width of this window.
         */
        void setWidth(int width);

        /**
         * Sets the height of this window.
         */
        void setHeight(int height);

        /**
         * Sets the position and size of this window.
         */
        void setDimension(const gcn::Rectangle &dimension);

        /**
         * Sets the location relative to the given widget.
         */
        void setLocationRelativeTo(gcn::Widget *widget);

        /**
         * Sets whether of not the window can be resized.
         */
        void setResizable(bool resize);

        /**
         * Returns whether the window can be resized.
         */
        bool isResizable();

        /**
         * Sets the minimum width of the window.
         */
        void setMinWidth(unsigned int width);

        /**
         * Sets the minimum height of the window.
         */
        void setMinHeight(unsigned int height);

        /**
         * Sets the maximum width of the window.
         */
        void setMaxWidth(unsigned int width);

        /**
         * Sets the minimum height of the window.
         */
        void setMaxHeight(unsigned int height);

        /**
          * Sets whether the window is sticky.
          * A sticky window will not have its visibility set to false
          * on a general setVisible(false) call.
          */
        void setSticky(bool sticky);

        /**
         * Returns whether the window is sticky.
         */
        bool isSticky();

        /**
         * Overloads window setVisible by guichan to allow sticky window
         * Handling
         */

        void setVisible(bool visible);

        /**
         * Returns the parent window.
         *
         * @return The parent window or <code>NULL</code> if there is none.
         */
        Window*
        getParentWindow() { return mParent; }

        /**
         * Schedule this window for deletion. It will be deleted at the start
         * of the next logic update.
         */
        void scheduleDelete();

        /**
         * Starts window resizing when appropriate.
         */
        void mousePressed(gcn::MouseEvent &event);

        /**
         * Implements window resizing and makes sure the window is not
         * dragged/resized outside of the screen.
         */
        void mouseDragged(gcn::MouseEvent &event);

        /**
         * Sets the name of the window. This is not the window title.
         */
        void
        setWindowName(const std::string &name) { mWindowName = name; }

        /**
         * Returns the name of the window. This is not the window title.
         */
        const std::string&
        getWindowName() { return mWindowName; }

        /**
         * Read the x, y, and width and height for resizables in the config
         * based on the name of the window.
         * That function let the values set with set{X, Y, Height, width}()
         * if no config value is found.
         * Don't forget to set these default values and resizable before
         * calling this function.
         */
        virtual void loadWindowState();
        void setCloseButton(bool flag);
        /**
         * Set the default win pos and size.
         * (which can be different of the actual ones.)
         */
        virtual void setDefaultSize(int defaultX, int defaultY,
                                    int defaultWidth, int defaultHeight);

        /**
         * Reset the win pos and size to default.
         * Don't forget to set defaults first.
         */
        virtual void resetToDefaultSize();

        enum ResizeHandles
        {
            TOP    = 0x01,
            RIGHT  = 0x02,
            BOTTOM = 0x04,
            LEFT   = 0x08
        };



    protected:
        GCContainer *mChrome;      /**< Contained container */
        ResizeGrip *mGrip;         /**< Resize grip */
        Window *mParent;           /**< The parent window */
        std::string mWindowName;   /**< Name of the window */
        bool mShowTitle;           /**< Window has a title bar */
        bool mModal;               /**< Window is modal */
        bool mResizable;           /**< Window can be resized */
        int mMouseResize;          /**< Window is being resized */
        bool mSticky;              /**< Window resists minimization */
        int mMinWinWidth;          /**< Minimum window width */
        int mMinWinHeight;         /**< Minimum window height */
        int mMaxWinWidth;          /**< Maximum window width */
        int mMaxWinHeight;         /**< Maximum window height */
        int mDefaultX;             /**< Default window X position */
        int mDefaultY;             /**< Default window Y position */
        int mDefaultWidth;         /**< Default window width */
        int mDefaultHeight;        /**< Default window height */

        bool mCloseButton;         /**< Window has a close button */

        /** The window container windows add themselves to. */
        static WindowContainer *windowikiContainer;

        /**
         * The config listener that listens to changes relevant to all windows.
         */
        static ConfigListener *windowikiConfigListener;
        static Image *closeImage;  /**< Close Button Image */
        static Image *solustImage;
        static Image *sagustImage;
        static Image *sagaltImage;
        static Image *solaltImage;
        static int instancesiki;      /**< Number of Window instances */
        static ImageRect borderiki;   /**< The window border and background */

        /**
         * The width of the resize border. Is independent of the actual window
         * border width, and determines mostly the size of the corner area
         * where two borders are moved at the same time.
         */
        static const int resizeBorderWidthiki = 10;

};

#endif
