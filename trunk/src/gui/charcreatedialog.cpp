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

#include "gui/charcreatedialog.h"

#include "gui/charselectdialog.h"
#include "gui/confirmdialog.h"
#include "gui/okdialog.h"
#include "gui/playerbox.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/radiobutton.h"
#include "gui/widgets/slider.h"
#include "gui/widgets/textfield.h"

#include "gui/widgets/layout.h"
#include "gui/gui.h"

#include "game.h"
#include "localplayer.h"
#include "main.h"
#include "units.h"

#include "net/charhandler.h"
#include "net/messageout.h"
#include "net/net.h"

#include "resources/colordb.h"
#include "resources/resourcemanager.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <guichan/font.hpp>

CharCreateDialog::CharCreateDialog(Window *parent, int slot):
    Window(_("Create Character"), true, parent),
    mSlot(slot)
{
    mPlayer = new Player(0, 0, NULL);
    mPlayer->setGender(GENDER_MALE);

    ResourceManager *resman = ResourceManager::getInstance();
    mBackGround = resman->getImage("graphics/elektrik/gui_login_window.png");

    gcn::Label *girisLabel = new gcn::Label(_("-=KARAKTER OLUŞTUR=-"));
    girisLabel->setPosition(150,140);
    girisLabel->setFont(font_bas_b_1_16);
    girisLabel->setForegroundColor(gcn::Color(0xaa,0xbb,0xcc));
    girisLabel->adjustSize();
    add(girisLabel);

    int numberOfHairColors = ColorDB::size();

    mHairStyle = rand() % mPlayer->getNumOfHairstyles();
    mHairColor = rand() % numberOfHairColors;
    updateHair();

    mNameField = new TextField("");
    mNameLabel = new Label(_("Name:"));
    // TRANSLATORS: This is a narrow symbol used to denote 'next'.
    // You may change this symbol if your language uses another.
    mNextHairColorButton = new Button(_(">"), "nextcolor", this);
    // TRANSLATORS: This is a narrow symbol used to denote 'previous'.
    // You may change this symbol if your language uses another.
    mPrevHairColorButton = new Button(_("<"), "prevcolor", this);
    mHairColorLabel = new Label(_("Hair color:"));
    mNextHairStyleButton = new Button(_(">"), "nextstyle", this);
    mPrevHairStyleButton = new Button(_("<"), "prevstyle", this);
    mHairStyleLabel = new Label(_("Hair style:"));
    mCreateButton = new Button(_("Create"), "create", this);
    mCancelButton = new Button(_("Cancel"), "cancel", this);
    mMale = new RadioButton(_("Male"), "gender");
    mFemale = new RadioButton(_("Female"), "gender");

    // Default to a Male character
    mMale->setSelected(true);

    mMale->setActionEventId("gender");
    mFemale->setActionEventId("gender");

    mMale->addActionListener(this);
    mFemale->addActionListener(this);

    mPlayerBox = new PlayerBox(mPlayer);

    mPlayerBox->setWidth(74);

    mNameField->setActionEventId("create");
    mNameField->addActionListener(this);

    mAttributesLeft = new Label(strprintf(_("Please distribute %d points"), 99));

    int w = 200;
    int h = 330;

    mPlayerBox->setDimension(gcn::Rectangle(215, 190, 80, 85));
    mNameLabel->setPosition(150, 160);
    mNameField->setDimension(
            gcn::Rectangle(195, 160, 120, mNameField->getHeight()));
    mPrevHairColorButton->setPosition(189, 195);
    mNextHairColorButton->setPosition(305, 195);
    mHairColorLabel->setPosition(115, 200);
    mPrevHairStyleButton->setPosition(189, 224);
    mNextHairStyleButton->setPosition(305, 224);
    mHairStyleLabel->setPosition(115, 225);
    mAttributesLeft->setPosition(105, 300);
    updateSliders();
    mCancelButton->setPosition(
            300 - 5 - mCancelButton->getWidth(),
            400);
    mCreateButton->setPosition(
            mCancelButton->getX() - 5 - mCreateButton->getWidth(),
            400);

    mMale->setPosition(190, 280);
    mFemale->setPosition(260, 280);

    add(mPlayerBox);
    add(mNameField);
    add(mNameLabel);
    add(mNextHairColorButton);
    add(mPrevHairColorButton);
    add(mHairColorLabel);
    add(mNextHairStyleButton);
    add(mPrevHairStyleButton);
    add(mHairStyleLabel);
    add(mAttributesLeft);
    add(mCreateButton);
    add(mCancelButton);

    add(mMale);
    add(mFemale);

    setSize(573,507);
    center();
    setVisible(true);
    mNameField->requestFocus();
}

CharCreateDialog::~CharCreateDialog()
{
    delete mPlayer;

    // Make sure the char server handler knows that we're gone
    Net::getCharHandler()->setCharCreateDialog(0);
}


void
CharCreateDialog::draw(gcn::Graphics *graphics)
{
    Graphics *g = static_cast<Graphics*>(graphics);
    g->drawImage(mBackGround,0,0);

    drawChildren(graphics);
}

void CharCreateDialog::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "create")
    {
        if (getName().length() >= 4)
        {
            // Attempt to create the character
            mCreateButton->setEnabled(false);

            std::vector<int> atts;
//            for (unsigned i = 0; i < mAttributeSlider.size(); i++)
//            {
//                atts.push_back((int) mAttributeSlider[i]->getValue());
//            }
atts.push_back(9);
atts.push_back(9);
atts.push_back(9);
atts.push_back(1);
atts.push_back(1);
atts.push_back(1);
            Net::getCharHandler()->newCharacter(getName(), mSlot,
                                mFemale->isSelected(), mHairStyle,
                                mHairColor, atts);
        }
        else
        {
            new OkDialog(_("Error"),
                         _("Your name needs to be at least 4 characters."),
                         this);
        }
    }
    else if (event.getId() == "cancel")
        scheduleDelete();
    else if (event.getId() == "nextcolor")
    {
        mHairColor++;
        updateHair();
    }
    else if (event.getId() == "prevcolor")
    {
        mHairColor--;
        updateHair();
    }
    else if (event.getId() == "nextstyle")
    {
        mHairStyle++;
        updateHair();
    }
    else if (event.getId() == "prevstyle")
    {
        mHairStyle--;
        updateHair();
    }
    else if (event.getId() == "statslider") {
        updateSliders();
    }
    else if (event.getId() == "gender"){
        if (mMale->isSelected()) {
            mPlayer->setGender(GENDER_MALE);
        } else {
            mPlayer->setGender(GENDER_FEMALE);
        }
    }
}

std::string CharCreateDialog::getName() const
{
    std::string name = mNameField->getText();
    trim(name);
    return name;
}

void CharCreateDialog::updateSliders()
{
//    for (unsigned i = 0; i < mAttributeSlider.size(); i++)
//    {
//        // Update captions
//        mAttributeValue[i]->setCaption(
//                toString((int) (mAttributeSlider[i]->getValue())));
//        mAttributeValue[i]->adjustSize();
//    }
//
//    // Update distributed points
//    int pointsLeft = mMaxPoints - getDistributedPoints();
//    if (pointsLeft == 0)
//    {
//        mAttributesLeft->setCaption(_("Character stats OK"));
//        mCreateButton->setEnabled(true);
//    }
//    else
//    {
//        mCreateButton->setEnabled(false);
//        if (pointsLeft > 0)
//        {
//            mAttributesLeft->setCaption(strprintf(_("Please distribute %d points"), pointsLeft));
//        }
//        else
//        {
//            mAttributesLeft->setCaption(strprintf(_("Please remove %d points"), -pointsLeft));
//        }
//    }
//
//    mAttributesLeft->adjustSize();
}

void CharCreateDialog::unlock()
{
    mCreateButton->setEnabled(true);
}

int CharCreateDialog::getDistributedPoints() const
{
    int points = 0;

    for (unsigned i = 0; i < mAttributeSlider.size(); i++)
    {
        points += (int) mAttributeSlider[i]->getValue();
    }
    return points;
}

void CharCreateDialog::setAttributes(std::vector<std::string> labels,
                                     int available, int min, int max)
{
    mMaxPoints = available;

//    for (unsigned i = 0; i < mAttributeLabel.size(); i++)
//    {
//        remove(mAttributeLabel[i]);
//        delete mAttributeLabel[i];
//        remove(mAttributeSlider[i]);
//        delete mAttributeSlider[i];
//        remove(mAttributeValue[i]);
//        delete mAttributeValue[i];
//    }
//
//    mAttributeLabel.resize(labels.size());
//    mAttributeSlider.resize(labels.size());
//    mAttributeValue.resize(labels.size());
//
//    int w = 200;
//    int h = 330;

//    for (unsigned i = 0; i < labels.size(); i++)
//    {
//        mAttributeLabel[i] = new Label(labels[i]);
//        mAttributeLabel[i]->setWidth(70);
//        mAttributeLabel[i]->setPosition(105, 290 + i*20);
//        add(mAttributeLabel[i]);
//
//        mAttributeSlider[i] = new Slider(min, max);
//        mAttributeSlider[i]->setDimension(gcn::Rectangle(175, 290 + i*20, 100, 10));
//        mAttributeSlider[i]->setActionEventId("statslider");
//        mAttributeSlider[i]->addActionListener(this);
//        add(mAttributeSlider[i]);
//
//        mAttributeValue[i] = new Label(toString(min));
//        mAttributeValue[i]->setPosition(280, 290 + i*20);
//        add(mAttributeValue[i]);
//    }

//    mAttributesLeft->setPosition(105, 480);
    updateSliders();

    mCancelButton->setPosition(
            300 - 5 - mCancelButton->getWidth(),
            440 - 5 - mCancelButton->getHeight());
    mCreateButton->setPosition(
            mCancelButton->getX() - 5 - mCreateButton->getWidth(),
            440 - 5 - mCancelButton->getHeight());

}

void CharCreateDialog::setFixedGender(bool fixed, Gender gender)
{
    if (gender == GENDER_FEMALE)
    {
        mFemale->setSelected(true);
        mMale->setSelected(false);
    }
    else
    {
        mMale->setSelected(true);
        mFemale->setSelected(false);
    }

    mPlayer->setGender(gender);

    if (fixed)
    {
        mMale->setEnabled(false);
        mFemale->setEnabled(false);
    }
}

void CharCreateDialog::updateHair()
{
    mHairStyle %= Being::getNumOfHairstyles();
    if (mHairStyle < 0)
       mHairStyle += Being::getNumOfHairstyles();

    mHairColor %= ColorDB::size();
    if (mHairColor < 0)
       mHairColor += ColorDB::size();

    mPlayer->setSprite(Player::HAIR_SPRITE,
                      mHairStyle * -1, ColorDB::get(mHairColor));
}
