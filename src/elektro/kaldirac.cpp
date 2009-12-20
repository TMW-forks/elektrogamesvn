#include "kaldirac.h"

Kaldirac::Kaldirac(gcn::ActionListener *listener) : mListener(listener)
{
    if (mListener)
    {
        addActionListener(mListener);
    }
    setFrameSize(0);
}

Kaldirac::~Kaldirac()
{

}

void Kaldirac::draw(gcn::Graphics *graphics)
{
    //std::string ss="graphics/sprites/elektroadd/kaldirac.png";
    ResourceManager *resman = ResourceManager::getInstance();
    std::string path = "graphics/sprites/elektroadd/kaldirac.png";
    resim = resman->getImage(path);

    Graphics *g = static_cast<Graphics*>(graphics);

    g->drawImage(resim,25,25);
//    ImageSet *res = resman->getImageSet(path,25,25);
//    g->drawImage(res->get(1),4,4);
    drawChildren(graphics);
}

void Kaldirac::action(const gcn::ActionEvent &event)
{

}
