/*
 *  The Mana World
 *  Copyright (C) 2004  The Mana World Development Team
 *
 *  This file is part of The Mana World.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef GUI_H
#define GUI_H

#include <guichan/gui.hpp>

#include "guichanfwd.h"

class Graphics;
class GuiConfigListener;
class ImageSet;
class SDLInput;
class Viewport;

/**
 * \defgroup GUI Core GUI related classes (widgets)
 */

/**
 * \defgroup Interface User interface related classes (windows, dialogs)
 */

/**
 * Main GUI class.
 *
 * \ingroup GUI
 */
class Gui : public gcn::Gui
{
    public:
        /**
         * Constructor.
         */
        Gui(Graphics *screen);

        /**
         * Destructor.
         */
        ~Gui();

        /**
         * Performs logic of the GUI. Overridden to track mouse pointer
         * activity.
         */
        void logic();

        /**
         * Draws the whole Gui by calling draw functions down in the
         * Gui hierarchy. It also draws the mouse pointer.
         */
        void draw();

        gcn::FocusHandler *getFocusHandler() const
        { return mFocusHandler; }

        /**
         * Return game font.
         */
        gcn::Font *getFont() const
        { return mGuiFont; }

        /**
         * Return the Font used for "Info Particles", i.e. ones showing, what
         * you picked up, etc.
         */
        gcn::Font *getInfoParticleFont() const
        { return mInfoParticleFont; }

        /**
         * Sets whether a custom cursor should be rendered.
         */
        void setUseCustomCursor(bool customCursor);

        /**
         * Sets which cursor should be used.
         */
        void setCursorType(int index)
        { mmCursorType = index; }

        /**
         * Cursors are in graphic order from left to right.
         * CURSOR_POINTER should be left untouched.
         * CURSOR_TOTAL should always be last.
         */
        enum {
            CURSOR_POINTER = 0,
            CURSOR_RESIZE_ACROSS,
            CURSOR_RESIZE_DOWN,
            CURSOR_RESIZE_DOWN_LEFT,
            CURSOR_RESIZE_DOWN_RIGHT,
            CURSOR_TOTAL
        };

    protected:
        void handleMouseMoved(const gcn::MouseInput &mouseInput);

    private:
        GuiConfigListener *mConfigListener;
        gcn::Font *mGuiFont;                  /**< The global GUI font */
        gcn::Font *mInfoParticleFont;         /**< Font for Info Particles*/
        bool mCustomCursor;                   /**< Show custom cursor */
        ImageSet *mMouseCursors;              /**< Mouse cursor images */
        float mMouseCursorAlpha;
        int mMouseInactivityTimer;
        int mmCursorType;
};

extern Gui *gui;                              /**< The GUI system */
extern SDLInput *guiInput;                    /**< GUI input */

/**
 * Bolded text font
 */
extern gcn::Font *boldFont;
extern gcn::Font *font_bas_1_20;
extern gcn::Font *font_bas_1_18;
extern gcn::Font *font_bas_1_16;
extern gcn::Font *font_bas_1_14;
extern gcn::Font *font_bas_2_20;
extern gcn::Font *font_bas_2_18;
extern gcn::Font *font_bas_2_16;
extern gcn::Font *font_bas_2_14;
extern gcn::Font *font_bas_3_18;
extern gcn::Font *font_bas_3_16;
extern gcn::Font *font_bas_3_14;
extern gcn::Font *font_bas_3_12;
extern gcn::Font *font_bas_4_16;
extern gcn::Font *font_bas_4_14;
extern gcn::Font *font_bas_4_12;
extern gcn::Font *font_bas_4_10;
extern gcn::Font *font_bas_4_8;
extern gcn::Font *font_bas_5_16;
extern gcn::Font *font_bas_5_14;
extern gcn::Font *font_bas_5_12;
extern gcn::Font *font_bas_5_10;
extern gcn::Font *font_bas_5_8;
extern gcn::Font *font_bas_b_1_20;
extern gcn::Font *font_bas_b_1_18;
extern gcn::Font *font_bas_b_1_16;
extern gcn::Font *font_bas_b_1_14;
extern gcn::Font *font_bas_b_2_20;
extern gcn::Font *font_bas_b_2_18;
extern gcn::Font *font_bas_b_2_16;
extern gcn::Font *font_bas_b_2_14;
extern gcn::Font *font_el_1_14;
extern gcn::Font *font_el_1_12;
extern gcn::Font *font_el_1_10;
extern gcn::Font *font_el_2_14;
extern gcn::Font *font_el_2_12;
extern gcn::Font *font_el_2_10;
extern gcn::Font *font_el_3_14;
extern gcn::Font *font_el_3_12;
extern gcn::Font *font_el_3_10;
extern gcn::Font *font_el_b_1_14;
extern gcn::Font *font_el_b_1_12;
extern gcn::Font *font_el_b_1_10;
extern gcn::Font *font_el_b_2_14;
extern gcn::Font *font_el_b_2_12;
extern gcn::Font *font_el_b_2_10;
extern gcn::Font *font_txt_1_14;
extern gcn::Font *font_txt_1_12;
extern gcn::Font *font_txt_1_10;
extern gcn::Font *font_txt_2_14;
extern gcn::Font *font_txt_2_12;
extern gcn::Font *font_txt_2_10;
extern gcn::Font *font_txt_3_14;
extern gcn::Font *font_txt_3_12;
extern gcn::Font *font_txt_3_10;
extern gcn::Font *font_txt_4_14;
extern gcn::Font *font_txt_4_12;
extern gcn::Font *font_txt_4_10;
extern gcn::Font *font_txt_5_14;
extern gcn::Font *font_txt_5_12;
extern gcn::Font *font_txt_5_10;
extern gcn::Font *font_txt_6_14;
extern gcn::Font *font_txt_6_12;
extern gcn::Font *font_txt_6_10;
extern gcn::Font *font_txt_b_1_14;
extern gcn::Font *font_txt_b_1_12;
extern gcn::Font *font_txt_b_1_10;
extern gcn::Font *font_txt_b_2_14;
extern gcn::Font *font_txt_b_2_12;
extern gcn::Font *font_txt_b_2_10;
extern gcn::Font *font_txt_b_3_14;
extern gcn::Font *font_txt_b_3_12;
extern gcn::Font *font_txt_b_3_10;
extern gcn::Font *font_calibri_14;
extern gcn::Font *font_calibri_11;
extern gcn::Font *font_calibri_10;
extern gcn::Font *font_i_calibri_14;
extern gcn::Font *font_i_calibri_11;
extern gcn::Font *font_i_calibri_10;
extern gcn::Font *font_b_calibri_14;
extern gcn::Font *font_b_calibri_11;
extern gcn::Font *font_b_calibri_10;
extern gcn::Font *font_b_i_calibri_14;
extern gcn::Font *font_b_i_calibri_11;
extern gcn::Font *font_b_i_calibri_10;


#endif // GUI_H
