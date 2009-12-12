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
    resim = resman->getImage("graphics/sprites/elektroadd/kaldirac.png");

    Graphics *g = static_cast<Graphics*>(graphics);

    g->drawImage(resim,25,25);
//    ImageSet *res = resman->getImageSet(ss,32,32);
//    g->drawImage(res->get(0),4,4);
    drawChildren(graphics);
}

void Kaldirac::action(const gcn::ActionEvent &event)
{

}
