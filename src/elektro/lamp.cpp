#include "lamp.h"

#include "utils/stringutils.h"

Lamp::Lamp(gcn::ActionListener *listener, Node *n1, Node *n2):
    Component(listener, n1 , n2)
{
    setType(LAMP);
}

Lamp::~Lamp()
{
    //dtor
}

void Lamp::draw(gcn::Graphics *graphics)
{
    std::string ss="graphics/elektrik/item-ampul-";
    ss += toString(getValue());
    if (getStatus()==ACTIVE) ss += "c";
    if (getStatus()==PLUS) ss += "d";
    if (getStatus()==PLUS2) ss += "e";
    if (getStatus()==BURNED) ss += "f";

    ss += ".png";
    Graphics *g = static_cast<Graphics*>(graphics);
    ResourceManager *resman = ResourceManager::getInstance();
    ImageSet *res = resman->getImageSet(ss,32,32);
    g->drawImage(res->get(mAngel),4,4);
    Component::draw(graphics);
}
