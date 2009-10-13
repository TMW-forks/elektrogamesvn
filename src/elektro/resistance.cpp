#include "resistance.h"

#include "utils/stringutils.h"

Resistance::Resistance(gcn::ActionListener *listener, Node *n1, Node *n2):
    Component(listener, n1, n2)
{
    setType(RESISTANCE);
}

Resistance::~Resistance()
{
    //dtor
}

void Resistance::draw(gcn::Graphics *graphics)
{
    std::string ss="graphics/elektrik/item-direnc-"+toString(getValue());
    if (getStatus()==BURNED) ss += "-b";
    ss += ".png";
    Graphics *g = static_cast<Graphics*>(graphics);
    ResourceManager *resman = ResourceManager::getInstance();
    ImageSet *res = resman->getImageSet(ss,32,32);
    g->drawImage(res->get(mAngel),4,4);
    Component::draw(graphics);
}

 void Resistance::nodesCalc()
 {
     Component::nodesCalc();
 }
