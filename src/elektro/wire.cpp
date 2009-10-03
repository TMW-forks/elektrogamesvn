#include "wire.h"

#include "utils/stringutils.h"

Wire::Wire(gcn::ActionListener *listener):
    Component(listener)
{
    setType(WIRE);
//    mRotate->setVisible(false);
//    mClose->setVisible(false);
    node2->setVisible(false);
    node1->setScroll(true);
    node1->setInterval(0);
    setW(node1->getWidth());
    setH(node1->getHeight());
}

Wire::~Wire()
{
    //dtor
}

void Wire::draw(gcn::Graphics *graphics)
{
    nodesCalc();
    Component::draw(graphics);
}

void Wire::nodesCalc()
{
    node1->setY(0);
    node1->setX(0);

}
