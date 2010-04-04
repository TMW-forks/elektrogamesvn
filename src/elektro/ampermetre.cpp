#include "motor.h"
#include "circuitwindow.h"
#include "game.h"
#include <sstream>
#include "log.h"
#include "gui/gui.h"

extern CircuitWindow *circuitWindow;

Ampermetre::Ampermetre(gcn::ActionListener *listener, Node *n1, Node *n2):
    Component(listener, n1, n2)
{
    setType(AMPERMETRE);
    setYariCap(20);
    mLabel = new gcn::Label("aa0.0");
    mLabel->setPosition(7,10);
    mLabel->setForegroundColor(gcn::Color(00,90,220));
    mLabel->setFont(font_txt_1_12);
    mLabel->adjustSize();
    add(mLabel);
}

Ampermetre::~Ampermetre()
{
    delete mLabel;
}

void Ampermetre::draw(gcn::Graphics *graphics)
{
    std::stringstream ss;
    ss <<"graphics/elektrik/item-ampermetre.png";
    Graphics *g = static_cast<Graphics*>(graphics);

    ImageSet *res = circuitWindow->mComponentImageSet[ss.str()];
    g->drawImage(res->get(0),4,4);

    Component::draw(graphics);
}

void Ampermetre::setCurrent(float s)
{
    std::stringstream zz;
    zz << Round(s,2);
    mLabel->setCaption("x");
    mLabel->adjustSize();
    setCurrent(s);
}

