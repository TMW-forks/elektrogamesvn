#include "otuzbesgram.h"

OtuzbesGram::OtuzbesGram(gcn::ActionListener *listener) : Kutle(listener)
{
     setAgirlik(35);
}

OtuzbesGram::~OtuzbesGram()
{
    //dtor
}

void
OtuzbesGram::draw(gcn::Graphics *graphics)
{
    std::string path="graphics/elektrik/otuzbes_gram.png";

    Graphics *g = static_cast<Graphics*>(graphics);
    ResourceManager *resman = ResourceManager::getInstance();
    ImageSet *res = resman->getImageSet(path,32,32);
    g->drawImage(res->get(getResimIndex()),4,4);
    Kutle::draw(graphics);
}
