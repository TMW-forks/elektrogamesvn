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

#include "net/ea/npchandler.h"

#include "net/ea/protocol.h"

#include "net/messagein.h"
#include "net/messageout.h"
#include "net/net.h"
#include "net/npchandler.h"

#include "beingmanager.h"
#include "localplayer.h"
#include "npc.h"

#include "gui/npcdialog.h"

#include "gui/chat.h"
#include "gui/widgets/chattab.h"
#include "utils/stringutils.h"

#include <SDL_types.h>

#include "elektro/hitabe.h"
#include "elektro/testwindow.h"
#include "elektro/circuitwindow.h"
#include "elektro/slaytwindow.h"
#include "elektro/similasyonpenceresi.h"
#include "elektro/missionwindow.h"

#include "log.h"

//NPC id:
//100-149 test
//150-199 açýklama yapanlar
//200-249 güzel söz vs
//250-299 öðretenler
//300-349 makinalar

Net::NpcHandler *npcHandler;
extern TestDialog *testDialog;
extern CircuitWindow *circuitWindow;
extern SlaytWindow *slaytWindow;
extern SimilasyonPenceresi *similasyonPenceresi;
extern MissionWindow *missionWindow;
namespace EAthena {

std::string temp;
static std::stringstream npcText;
NpcHandler::NpcHandler()
{
    static const Uint16 _messages[] = {
        SMSG_NPC_CHOICE,
        SMSG_NPC_MESSAGE,
        SMSG_NPC_NEXT,
        SMSG_NPC_CLOSE,
        SMSG_NPC_INT_INPUT,
        SMSG_NPC_STR_INPUT,
        0
    };
    handledMessages = _messages;
    npcHandler = this;
}

void NpcHandler::handleMessage(MessageIn &msg)
{
    int id;
    bool resetPlayer = false;
    Being *being;

    switch (msg.getId())
    {
        case SMSG_NPC_CHOICE:
            msg.readInt16();  // length
            current_npc = msg.readInt32();
            being = beingManager->findBeing(current_npc);
            if (being->getJob()>=150 && being->getJob()<=199)
            {
                npcDialog->setNpc(current_npc);
                npcDialog->choiceRequest();
                npcDialog->parseListItems(msg.readString(msg.getLength() - 8));
                npcDialog->setVisible(true);
                resetPlayer = true;
            }
            break;

        case SMSG_NPC_MESSAGE:
            msg.readInt16();  // length
            current_npc = msg.readInt32();
            being = beingManager->findBeing(current_npc);
            temp = msg.readString(msg.getLength() - 8);
            resetPlayer = true;
            npcText<<temp;
            if (being->getJob() == 299)
            {
                if (temp=="</mission>")
                {
                    missionWindow->setVisible(true);
                    missionWindow->parse(npcText.str());
                    npcText.str("");
                }
            }
            else if (being->getJob()>=100 && being->getJob()<=149)
            {
                if (temp=="</test>")
                {
                     testDialog->setVisible(true);
                     testDialog->setDoc(npcText.str());
                     testDialog->parse();
                     npcText.str("");
                }
            }
            else if (being->getJob()>=150 && being->getJob()<=199)
            {
                npcDialog->setNpc(current_npc);
                npcDialog->addText(temp);
                npcDialog->setVisible(true);
            }
            else if (being->getJob()>=200 && being->getJob()<=249)
            {
                if (temp=="</presentation>")
                {
                    slaytWindow->parseXML(npcText.str());
                    npcText.str("");
                }
            }
            else if (being->getJob()>=250 && being->getJob()<=299)
            {
                if (being->getJob()==251)
                {
                    Hitabe *hitabe = new Hitabe();
                    hitabe->setVisible(true);
                    npcText.str("");
                }
                else if (being->getJob()==253)
                {
                    if(temp == "</similasyon>")
                    {
                        //Serverdaki npc nin xml dosyasýndan gelen veri
                        //similasyon nesnesine gönderiliyor
                        similasyonPenceresi->parseXML(npcText.str());
                        similasyonPenceresi->setVisible(true);
                        npcText.str("");
                    }
                }
            }
            else if (being->getJob()>=300 && being->getJob()<=350 )
            {
                circuitWindow->setVisible(true);
                circuitWindow->requestMoveToTop();
                 if (temp=="</circuit>"){
                    circuitWindow->circuitFromXML(npcText.str());
                    npcText.str("");
                 }
            }

            break;

         case SMSG_NPC_CLOSE:
            id = msg.readInt32();
            // If we're talking to that NPC, show the close button
            npcText.str("");
            being = beingManager->findBeing(current_npc);

            if (id == current_npc)
            {
                npcDialog->showCloseButton();
                resetPlayer = true;
            }
            // Otherwise, move on as an empty dialog doesn't help
            else
                closeDialog(id);
            break;

        case SMSG_NPC_NEXT:
            id = msg.readInt32();
            // If we're talking to that NPC, show the next button
            being = beingManager->findBeing(current_npc);
            if (id == current_npc)
            {
                npcDialog->showNextButton();
                resetPlayer = true;
            }
            // Otherwise, move on as an empty dialog doesn't help
            else
                nextDialog(id);
            break;

        case SMSG_NPC_INT_INPUT:
            // Request for an integer
            current_npc = msg.readInt32();
            npcDialog->setNpc(current_npc);
            npcDialog->integerRequest(0);
            npcDialog->setVisible(true);
            resetPlayer = true;
            break;

        case SMSG_NPC_STR_INPUT:
            // Request for a string
            current_npc = msg.readInt32();
            npcDialog->setNpc(current_npc);
            npcDialog->textRequest("");
            npcDialog->setVisible(true);
            resetPlayer = true;
            break;
    }

    if (resetPlayer && player_node->mAction != Being::SIT)
        player_node->setAction(Being::STAND);
}

void NpcHandler::talk(int npcId)
{
    MessageOut outMsg(CMSG_NPC_TALK);
    outMsg.writeInt32(npcId);
    outMsg.writeInt8(0); // Unused
}

void NpcHandler::nextDialog(int npcId)
{
    MessageOut outMsg(CMSG_NPC_NEXT_REQUEST);
    outMsg.writeInt32(npcId);
}

void NpcHandler::closeDialog(int npcId)
{
    MessageOut outMsg(CMSG_NPC_CLOSE);
    outMsg.writeInt32(npcId);
    npcDialog->setText("");
    npcDialog->setVisible(false);
}

void NpcHandler::listInput(int npcId, int value)
{
    MessageOut outMsg(CMSG_NPC_LIST_CHOICE);
    outMsg.writeInt32(npcId);
    outMsg.writeInt8(value);
}

void NpcHandler::integerInput(int npcId, int value)
{
    MessageOut outMsg(CMSG_NPC_INT_RESPONSE);
    outMsg.writeInt32(npcId);
    outMsg.writeInt32(value);
}

void NpcHandler::stringInput(int npcId, const std::string &value)
{
    MessageOut outMsg(CMSG_NPC_STR_RESPONSE);
    outMsg.writeInt16(value.length() + 9);
    outMsg.writeInt32(npcId);
    outMsg.writeString(value, value.length());
    outMsg.writeInt8(0); // Prevent problems with string reading
}

void NpcHandler::sendLetter(int npcId, const std::string &recipient,
                            const std::string &text)
{
    // TODO
}

void NpcHandler::startShopping(int beingId)
{
    // TODO
}

void NpcHandler::buy(int beingId)
{
    MessageOut outMsg(CMSG_NPC_BUY_SELL_REQUEST);
    outMsg.writeInt32(beingId);
    outMsg.writeInt8(0); // Buy
}

void NpcHandler::sell(int beingId)
{
    MessageOut outMsg(CMSG_NPC_BUY_SELL_REQUEST);
    outMsg.writeInt32(beingId);
    outMsg.writeInt8(1); // Sell
}

void NpcHandler::buyItem(int beingId, int itemId, int amount)
{
    MessageOut outMsg(CMSG_NPC_BUY_REQUEST);
    outMsg.writeInt16(8); // One item (length of packet)
    outMsg.writeInt16(amount);
    outMsg.writeInt16(itemId);
}

void NpcHandler::sellItem(int beingId, int itemId, int amount)
{
    MessageOut outMsg(CMSG_NPC_SELL_REQUEST);
    outMsg.writeInt16(8); // One item (length of packet)
    outMsg.writeInt16(itemId + INVENTORY_OFFSET);
    outMsg.writeInt16(amount);
}

void NpcHandler::endShopping(int beingId)
{
    // TODO
}

} // namespace EAthena
