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
 *  $Id: debugwindow.h 3290 2007-05-04 13:09:25Z crush_tmw $
 */

#ifndef _TMW_HITABE_H
#define _TMW_HITABE_H

#include <iosfwd>

#include <guichan/widgets/label.hpp>
#include <guichan/actionlistener.hpp>

#include "../resources/imageset.h"
#include "../resources/resourcemanager.h"

#include "window_iki.h"
#include "gui/widgets/button.h"
#include "gui/widgets/window.h"

#include "../guichanfwd.h"
#include "net/ea/npchandler.h"


/**
 * The hitabe window.
 *
 * \ingroup npc window
 */
class Hitabe : public Window, public gcn::ActionListener
{
    public:
        /**
         * Constructor.
         */
        Hitabe();

        /**
         * Logic (updates components' size and infos)
         */
        void logic();

        /**
         * Performs action.
         */
        void action(const gcn::ActionEvent &event);

        /**
         * Draw the window.
         */
        void draw(gcn::Graphics* graphics);


    private:
// TODO (pulsar#1#): Dalgalanan Türk Bayraðý eklenecek

        // ImageSet *mSet1;
        Image *mBase;
};

#endif
