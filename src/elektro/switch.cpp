#include "switch.h"
#include "circuitwindow.h"

#include "circuitwindow.h"
#include "game.h"

extern CircuitWindow *circuitWindow;
extern volatile int tick_time;
static int first_tick = 0;

Switch::Switch(gcn::ActionListener *listener, Node *n1, Node *n2):
    Component(listener, n1, n2)
{
      setType(SWITCH);
}

Switch::~Switch()
{
    //dtor
}

void
Switch::mousePressed(gcn::MouseEvent &event)
{
    bool dbl_click = false;

    if (first_tick ==0)
    {
        first_tick = tick_time;
        dbl_click = false;
    }else
    {
        int delta = abs(tick_time - first_tick);
        delta>50 ? dbl_click = false: dbl_click = true;
        circuitWindow->mHint->setCaption(toString(delta)+"-"+toString(event.getClickCount()));
        first_tick = 0;
    }

    if (getStatus() !=BURNED && dbl_click)
    {
        setStatus(getStatus() == ACTIVE ? PASIVE : ACTIVE);
        dbl_click = false;
        circuitWindow->setRefresh(true);
    }
    Component::mousePressed(event);
}


void
Switch::draw(gcn::Graphics *graphics)
{
    std::string ss="graphics/elektrik/item-anahtar";

    if (getStatus()==ACTIVE) ss += "-b";
    if (getStatus()==BURNED) ss += "-c";

    ss += ".png";
    Graphics *g = static_cast<Graphics*>(graphics);
    ResourceManager *resman = ResourceManager::getInstance();

    ImageSet *res = circuitWindow->mComponentImageSet[ss];
    g->drawImage(res->get(mAngel),4,4);

    Component::draw(graphics);
}
