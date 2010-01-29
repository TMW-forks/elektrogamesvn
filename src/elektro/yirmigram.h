#ifndef YIRMIGRAM_H
#define YIRMIGRAM_H

#include "kutle.h"
#include <guichan/actionlistener.hpp>
#include "gui/widgets/window.h"
#include <guichan/graphics.hpp>


class YirmiGram : public Kutle
{
    public:
        YirmiGram(gcn::ActionListener *listener);
        virtual ~YirmiGram();
        void draw(gcn::Graphics *graphics);
    protected:
    private:
};

#endif // YIRMIGRAM_H
