#include "beskilogram.h"
#include "log.h"
BesKiloGram::BesKiloGram(gcn::ActionListener *listener): Kutle(listener)
{

}

BesKiloGram::~BesKiloGram()
{
    //dtor
}

void BesKiloGram::draw(gcn::Graphics *graphics)
{
    std::string ss="graphics/elektrik/item-ampul-0";

    ss += "-b.png";
    Graphics *g = static_cast<Graphics*>(graphics);
    ResourceManager *resman = ResourceManager::getInstance();
    ImageSet *res = resman->getImageSet(ss,32,32);
    g->drawImage(res->get(0),4,4);
    Kutle::draw(graphics);
    logger->log("drawýn içi");
}
