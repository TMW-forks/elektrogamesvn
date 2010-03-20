#include "battery.h"
#include "circuitwindow.h"
#include "game.h"
#include <sstream>

extern CircuitWindow *circuitWindow;

Battery::Battery(gcn::ActionListener *listener, Node *n1, Node *n2):
    Component(listener, n1, n2)
{
    setType(BATTERY);
    setYariCap(19);
}

Battery::~Battery()
{
    //dtor
}

void Battery::draw(gcn::Graphics *graphics)
{
    std::stringstream ss;
    ss <<"graphics/elektrik/item-pil";

    if (getStatus()!=BURNED) ss << "-" << getValue() << "v";

    if (getStatus()==BURNED) ss << "-b";

    ss << ".png";
    Graphics *g = static_cast<Graphics*>(graphics);

    ImageSet *res = circuitWindow->mComponentImageSet[ss.str()];
    g->drawImage(res->get(mAngel),4,4);

    Component::draw(graphics);
}
