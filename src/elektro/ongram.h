#ifndef ONGRAM_H
#define ONGRAM_H

#include "kutle.h"
#include <guichan/actionlistener.hpp>
#include "gui/widgets/window.h"
#include <guichan/graphics.hpp>

class OnGram : public Kutle
{
    public:
        OnGram(gcn::ActionListener *listener);
        virtual ~OnGram();
        void draw(gcn::Graphics *graphics);
    protected:
    private:
};

#endif // ONGRAM_H
