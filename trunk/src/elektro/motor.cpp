#include "motor.h"
#include "circuitwindow.h"
#include "game.h"
#include <sstream>
#include "log.h"

extern CircuitWindow *circuitWindow;

Motor::Motor(gcn::ActionListener *listener, Node *n1, Node *n2):
    Component(listener, n1, n2)
{
    setType(MOTOR);
    setYariCap(21);
    picture = 1;
}

Motor::~Motor()
{
    //dtor
}

void Motor::draw(gcn::Graphics *graphics)
{
    std::stringstream ss;
    ss <<"graphics/elektrik/item-motor-";

    ss << picture;

    ss << ".png";
    Graphics *g = static_cast<Graphics*>(graphics);

    ImageSet *res = circuitWindow->mComponentImageSet[ss.str()];
    g->drawImage(res->get(mAngel),4,4);

    Component::draw(graphics);
}

void Motor::logic()
{
    if (tick_time %4 == 0 && mCurrent != 0) picture++;
    if (picture>3) picture = 1;
}
