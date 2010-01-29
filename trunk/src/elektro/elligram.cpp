#include "elligram.h"

ElliGram::ElliGram(gcn::ActionListener *listener) : Kutle(listener)
{
    setAgirlik(50);
}

ElliGram::~ElliGram()
{
    //dtor
}

void
ElliGram::draw(gcn::Graphics *graphics)
{
    std::string path="graphics/elektrik/elli_gram.png";

    Graphics *g = static_cast<Graphics*>(graphics);
    ResourceManager *resman = ResourceManager::getInstance();
    ImageSet *res = resman->getImageSet(path,32,32);
    g->drawImage(res->get(getResimIndex()),4,4);
    Kutle::draw(graphics);
}
