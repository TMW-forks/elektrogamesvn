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
    //dtor
}

void Kaldirac::draw(gcn::Graphics *graphics)
{
    std::string ss="graphics/elektrik/res1set.png";

    Graphics *g = static_cast<Graphics*>(graphics);
    ResourceManager *resman = ResourceManager::getInstance();
    ImageSet *res = resman->getImageSet(ss,32,32);
    g->drawImage(res->get(0),4,4);
    drawChildren(graphics);
}

void Kaldirac::action(const gcn::ActionEvent &event)
{

}
