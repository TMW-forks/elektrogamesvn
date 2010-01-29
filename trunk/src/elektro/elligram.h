#ifndef ELLIGRAM_H
#define ELLIGRAM_H

#include "kutle.h"
#include <guichan/actionlistener.hpp>
#include "gui/widgets/window.h"
#include <guichan/graphics.hpp>

class ElliGram : public Kutle
{
    public:
        ElliGram(gcn::ActionListener *listener);
        virtual ~ElliGram();
        void draw(gcn::Graphics *graphics);
    protected:
    private:
};

#endif // ELLIGRAM_H
