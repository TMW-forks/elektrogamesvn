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
#include "gui/ministatus.h"

#include "gui/gui.h"
#include "gui/statuswindow.h"

#include "gui/widgets/progressbar.h"

#include "animatedsprite.h"
#include "configuration.h"
#include "graphics.h"
#include "localplayer.h"

#include "utils/stringutils.h"
#include "utils/gettext.h"
#include "gui/gui.h"

extern volatile int tick_time;

MiniStatusWindow::MiniStatusWindow():
    Popup("MiniStatus")
{
    mHpBar = new ProgressBar((float) player_node->getHp()
                             / (float) player_node->getMaxHp(),
                             213, 20, gcn::Color(0, 171, 34));
    mMpBar = new ProgressBar((float) player_node->getMaxMP()
                             / (float) player_node->getMaxMP(),
                             100, 20, gcn::Color(26, 102, 230));
    mXpBar = new ProgressBar((float) player_node->getExp()
                             / player_node->getExpNeeded(),
                             100, 20, gcn::Color(143, 192, 211));
    mHpBar->setPosition(14, 3);
    mMpBar->setPosition(14, mHpBar->getHeight()+3);
    mXpBar->setPosition(mMpBar->getX() + mMpBar->getWidth() + 13, mHpBar->getHeight()+3);

    mHpLabel = new gcn::Label(_("HP"));
    mMpLabel = new gcn::Label(_("MP"));
    mXpLabel = new gcn::Label(_("XP"));

    mHpLabel->setForegroundColor(gcn::Color(255, 255, 255));
    mMpLabel->setForegroundColor(gcn::Color(255, 255, 255));
    mXpLabel->setForegroundColor(gcn::Color(255, 255, 255));

    mHpLabel->setFont(font_b_calibri);
    mMpLabel->setFont(font_b_calibri);
    mXpLabel->setFont(font_b_calibri);

    mHpLabel->setPosition(0,4);
    mMpLabel->setPosition(0,mMpBar->getY()+3);
    mXpLabel->setPosition(mXpBar->getX()-12,mMpLabel->getY());

    add(mHpBar);
    add(mMpBar);
    add(mXpBar);
    add(mHpLabel);
    add(mMpLabel);
    add(mXpLabel);

    setContentSize(mXpBar->getX() + mXpBar->getWidth()+10,
                   mXpBar->getY() + mXpBar->getHeight()+ mHpBar->getHeight());

    setVisible((bool) config.getValue(getPopupName() + "Visible", true));
}

void MiniStatusWindow::setIcon(int index, AnimatedSprite *sprite)
{
    if (index >= (int) mIcons.size())
        mIcons.resize(index + 1, NULL);

    if (mIcons[index])
        delete mIcons[index];

    mIcons[index] = sprite;
}

void MiniStatusWindow::eraseIcon(int index)
{
    mIcons.erase(mIcons.begin() + index);
}

void MiniStatusWindow::drawIcons(Graphics *graphics)
{
    // Draw icons
    int icon_x = mXpBar->getX() + mXpBar->getWidth() + 4;
    for (unsigned int i = 0; i < mIcons.size(); i++) {
        if (mIcons[i]) {
            mIcons[i]->draw(graphics, icon_x, 3);
            icon_x += 2 + mIcons[i]->getWidth();
        }
    }
}

void MiniStatusWindow::update(int id)
{
    if (id == StatusWindow::HP)
    {
        StatusWindow::updateHPBar(mHpBar);
    }
    else if (id == StatusWindow::MP)
    {
        StatusWindow::updateMPBar(mMpBar);
    }
    else if (id == StatusWindow::EXP)
    {
        StatusWindow::updateXPBar(mXpBar);
    }
}

void MiniStatusWindow::logic()
{
    Popup::logic();

    // Displays the number of monsters to next lvl
    // (disabled for now but interesting idea)
    /*
    if (config.getValue("xpBarMonsterCounterExp", 0)!=0)
    {
        updatedText << " | "
            << (int)(((float)player_node->mXpForNextLevel - (float)player_node->mXp)
             / (float)config.getValue("xpBarMonsterCounterExp", 0))
            << " "
            << config.getValue("xpBarMonsterCounterName", "Monsters") <<" left...";
    }
    */

    for (unsigned int i = 0; i < mIcons.size(); i++)
        if (mIcons[i])
            mIcons[i]->update(tick_time * 10);
}
