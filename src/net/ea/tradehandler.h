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

#ifndef NET_EA_TRADEHANDLER_H
#define NET_EA_TRADEHANDLER_H

#include "net/messagehandler.h"
#include "net/net.h"
#include "net/tradehandler.h"

namespace EAthena {

class TradeHandler : public MessageHandler, public Net::TradeHandler
{
    public:
        TradeHandler();

        void handleMessage(MessageIn &msg);

        void request(Being *being);

        void respond(bool accept);

        void addItem(Item *item, int amount);

        void removeItem(int slotNum, int amount);

        void setMoney(int amount);

        void confirm();

        void finish();

        void cancel();
};

} // namespace EAthena

#endif // NET_EA_TRADEHANDLER_H
