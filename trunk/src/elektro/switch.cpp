#include "switch.h"

Switch::Switch(gcn::ActionListener *listener, Node *n1, Node *n2):
    Component(listener, n1, n2)
{
      setType(SWITCH);
}

Switch::~Switch()
{
    //dtor
}

void Switch::draw(gcn::Graphics *graphics)
{
    std::string ss="graphics/elektrik/item-anahtar";

    if (getStatus()==ACTIVE) ss += "-b";
    if (getStatus()==BURNED) ss += "-c";

    ss += ".png";
    Graphics *g = static_cast<Graphics*>(graphics);
    ResourceManager *resman = ResourceManager::getInstance();
    ImageSet *res = resman->getImageSet(ss,32,32);
    g->drawImage(res->get(mAngel),4,4);
    Component::draw(graphics);
}
