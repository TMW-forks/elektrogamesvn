#include "yirmigram.h"

YirmiGram::YirmiGram(gcn::ActionListener *listener):Kutle(listener)
{
    setAgirlik(20);
}

YirmiGram::~YirmiGram()
{
    //dtor
}

void YirmiGram::draw(gcn::Graphics *graphics)
{
    std::string path="graphics/elektrik/yirmi_gram.png";

    Graphics *g = static_cast<Graphics*>(graphics);
    ResourceManager *resman = ResourceManager::getInstance();
    ImageSet *res = resman->getImageSet(path,32,32);
    g->drawImage(res->get(getResimIndex()),4,4);
    Kutle::draw(graphics);
}
