#include "ongram.h"

OnGram::OnGram(gcn::ActionListener *listener):Kutle(listener)
{
    setAgirlik(10);
}

OnGram::~OnGram()
{
    //dtor
}

void OnGram::draw(gcn::Graphics *graphics)
{
    std::string path="graphics/elektrik/on_gram.png";

    Graphics *g = static_cast<Graphics*>(graphics);
    ResourceManager *resman = ResourceManager::getInstance();
    ImageSet *res = resman->getImageSet(path,32,32);
    g->drawImage(res->get(getResimIndex()),4,4);
    Kutle::draw(graphics);
}
