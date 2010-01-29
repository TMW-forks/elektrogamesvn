#ifndef YIRMIBESGRAM_H
#define YIRMIBESGRAM_H

#include "kutle.h"
#include <guichan/actionlistener.hpp>
#include "gui/widgets/window.h"
#include <guichan/graphics.hpp>

class YirmibesGram : public Kutle
{
    public:
        YirmibesGram(gcn::ActionListener *listener);
        virtual ~YirmibesGram();
        void draw(gcn::Graphics *graphics);

    protected:
    private:
};

#endif // YIRMIBESGRAM_H
