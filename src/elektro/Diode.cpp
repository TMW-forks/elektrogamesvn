#include "diode.h"

#include "../utils/stringutils.h"

Diode::Diode(gcn::ActionListener *listener, Node *n1, Node *n2):
    Component(listener, n1, n2)
{
    setType(DIODE);
}

Diode::~Diode()
{
    //dtor
}

void Diode::draw(gcn::Graphics *graphics)
{
    std::string ss="graphics/elektrik/item-";
    if (getValue()==1)
        ss +="k";
    else  if (getValue()==2)
        ss +="y";
    if (getValue()==3)
        ss +="s";
    else  if (getValue()==4)
        ss +="b";
    ss += "led";

    if (getStatus()==ACTIVE) ss += "-b";
    if (getStatus()==BURNED) ss += "-c";

    ss += ".png";
    Graphics *g = static_cast<Graphics*>(graphics);
    ResourceManager *resman = ResourceManager::getInstance();
    ImageSet *res = resman->getImageSet(ss,32,32);
    g->drawImage(res->get(mAngel),4,4);
    Component::draw(graphics);
}
