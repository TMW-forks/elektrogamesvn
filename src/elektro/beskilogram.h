#ifndef BESKILOGRAM_H
#define BESKILOGRAM_H

#include "kutle.h"
#include <guichan/actionlistener.hpp>
#include "gui/widgets/window.h"
#include <guichan/graphics.hpp>

class BesKiloGram : public Kutle
{
    public:
        BesKiloGram(gcn::ActionListener *listener);
        virtual ~BesKiloGram();
        void draw(gcn::Graphics *graphics);
    protected:
    private:
};

#endif // BESKILOGRAM_H
