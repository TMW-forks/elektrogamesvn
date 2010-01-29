#include "yuzgram.h"

YuzGram::YuzGram(gcn::ActionListener *listener) : Kutle(listener)
{
    setAgirlik(100);
}

YuzGram::~YuzGram()
{
    //dtor
}

void
YuzGram::draw(gcn::Graphics *graphics)
{
    std::string path="graphics/elektrik/yuz_gram.png";

    Graphics *g = static_cast<Graphics*>(graphics);
    ResourceManager *resman = ResourceManager::getInstance();
    ImageSet *res = resman->getImageSet(path,32,32);
    g->drawImage(res->get(getResimIndex()),4,4);
    Kutle::draw(graphics);
}
