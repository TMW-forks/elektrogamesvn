#include "beskilogram.h"
#include "kaldirac.h"
#include "log.h"
#include "utils/gettext.h"

BesKiloGram::BesKiloGram(gcn::ActionListener *listener):Kutle(listener)
{
    setAgirlik(5);
}

BesKiloGram::~BesKiloGram()
{
    //dtor
}

void BesKiloGram::draw(gcn::Graphics *graphics)
{
    std::string path="graphics/sprites/elektroadd/bes_gram.png";

    Graphics *g = static_cast<Graphics*>(graphics);
    ResourceManager *resman = ResourceManager::getInstance();
    ImageSet *res = resman->getImageSet(path,32,32);
    g->drawImage(res->get(getResimIndex()),4,4);
    Kutle::draw(graphics);
}

void
BesKiloGram::logic()
{

}


