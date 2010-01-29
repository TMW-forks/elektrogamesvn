#ifndef OTUZBESGRAM_H
#define OTUZBESGRAM_H

#include "kutle.h"
#include <guichan/actionlistener.hpp>
#include "gui/widgets/window.h"
#include <guichan/graphics.hpp>

class OtuzbesGram : public Kutle
{
    public:
        OtuzbesGram(gcn::ActionListener *listener);
        virtual ~OtuzbesGram();
        void draw(gcn::Graphics *graphics);
    protected:
    private:
};

#endif // OTUZBESGRAM_H
