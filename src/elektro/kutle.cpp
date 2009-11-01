#include "kutle.h"
#include "similasyonpenceresi.h"

#include <SDL_types.h>
#include <guichan/sdl/sdlinput.hpp>

#include "../graphics.h"
#include "../log.h"
#include "../game.h"

extern SimilasyonPenceresi *similasyonPenceresi;

Kutle::Kutle(gcn::ActionListener *listener) :
    mListener(listener),
    Window(_("Kütle"))
{
    if (mListener)
    {
        addActionListener(mListener);
    }
}

Kutle::~Kutle()
{
    //dtor
}

void Kutle::draw(gcn::Graphics *graphics)
{

    drawChildren(graphics);
}

void Kutle::mousePressed(gcn::MouseEvent &event)
{
    gcn::Window::mousePressed(event);

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        Uint8* keys;
        keys = SDL_GetKeyState(NULL);
        tempX = event.getX();
        tempY = event.getY();
        setSelected(true);
    }
}
void Kutle::mouseDragged(gcn::MouseEvent &event)
{
    if (getSelected())
    {
        //requestMoveToTop();
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        setX(mouseX-similasyonPenceresi->getX()-getWidth()/2);
        setY(mouseY-similasyonPenceresi->getY()-getHeight()/2-13);
        if (getX()<20)
            setX(20);
        if (getY()<50)
            setY(50);
        if (getX()>(similasyonPenceresi->getWidth()-getWidth()-15))
            setX(similasyonPenceresi->getWidth()-15-getWidth());
        if (getY()>(similasyonPenceresi->getHeight()-getHeight()-60))
            setY(similasyonPenceresi->getHeight()-getHeight()-60);
    }
}

void Kutle::mouseReleased(gcn::MouseEvent &event)
{

}
void Kutle::mouseEntered(gcn::MouseEvent &event)
{

}
void Kutle::mouseExited(gcn::MouseEvent &event)
{

}
void Kutle::setSelected (bool durum)
{
    mSelected = durum;
}
bool Kutle::getSelected ()
{
    return mSelected;
}
void Kutle::action(const gcn::ActionEvent &event)
{

}
