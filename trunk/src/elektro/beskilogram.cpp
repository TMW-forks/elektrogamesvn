#include "beskilogram.h"
#include "log.h"
#include "utils/gettext.h"
BesKiloGram::BesKiloGram(gcn::ActionListener *listener):
    Kutle(listener)
{
    setAgirlik(5);
}

BesKiloGram::~BesKiloGram()
{
    //dtor
}

void BesKiloGram::draw(gcn::Graphics *graphics)
{
    std::string ss="graphics/elektrik/item-ampul-0.png";

    Graphics *g = static_cast<Graphics*>(graphics);
    ResourceManager *resman = ResourceManager::getInstance();
    ImageSet *res = resman->getImageSet(ss,32,32);
    g->drawImage(res->get(0),4,4);
    Kutle::draw(graphics);
}
