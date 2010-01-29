#include "besgram.h"

BesGram::BesGram(gcn::ActionListener *listener) : Kutle(listener)
{
     setAgirlik(5);
}

BesGram::~BesGram()
{
    //dto
}

void
BesGram::draw(gcn::Graphics *graphics)
{
    std::string path="graphics/elektrik/bes_gram.png";

    Graphics *g = static_cast<Graphics*>(graphics);
    ResourceManager *resman = ResourceManager::getInstance();
    ImageSet *res = resman->getImageSet(path,32,32);
    g->drawImage(res->get(getResimIndex()),4,4);
    Kutle::draw(graphics);
}
