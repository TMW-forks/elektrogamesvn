#ifndef BESGRAM_H
#define BESGRAM_H

#include "kutle.h"
#include <guichan/actionlistener.hpp>
#include "gui/widgets/window.h"
#include <guichan/graphics.hpp>

class BesGram : public Kutle
{
    public:
        BesGram(gcn::ActionListener *listener);
        virtual ~BesGram();
        void draw(gcn::Graphics *graphics);
    protected:
    private:
};

#endif // BESGRAM_H
