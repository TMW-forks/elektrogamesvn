#ifndef YUZGRAM_H
#define YUZGRAM_H

#include "kutle.h"
#include <guichan/actionlistener.hpp>
#include "gui/widgets/window.h"
#include <guichan/graphics.hpp>

class YuzGram : public Kutle
{
    public:
        YuzGram(gcn::ActionListener *listener);
        virtual ~YuzGram();
        void draw(gcn::Graphics *graphics);
    protected:
    private:
};

#endif // YUZGRAM_H
