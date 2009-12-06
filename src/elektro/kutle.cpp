#include "kutle.h"
#include "similasyonpenceresi.h"

#include <SDL_types.h>
#include <guichan/sdl/sdlinput.hpp>

#include "../graphics.h"
#include "../log.h"
#include "../game.h"

#include <utility> //make_pair() için

extern SimilasyonPenceresi *similasyonPenceresi;

Kutle::Kutle(gcn::ActionListener *listener) :
    mListener(listener),
    Window(_("Kütle"))
{
    mAgirlik = 0;
    if (mListener)
    {
        addActionListener(mListener);
    }
    setFrameSize(0);
}

Kutle::~Kutle()
{
    //dtor
}

void Kutle::action(const gcn::ActionEvent &event)
{

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
        setX(getX()+event.getX()-tempX);
        setY(getY()+event.getY()-tempY);

        if (getX()<20)
            setX(20);
        if (getY()<125)
            setY(125);
        if (getX()>(similasyonPenceresi->getWidth()-getWidth()-15))
            setX(similasyonPenceresi->getWidth()-15-getWidth());
        if (getY()>(similasyonPenceresi->getHeight()-getHeight()-95))
            setY(similasyonPenceresi->getHeight()-getHeight()-95);

        //Nesnenin sürükleme anında istediğim alana yapışması için.
        //1.Kefe
        if (getX()>290 && getX()<320 && getY()>265)
        {
            setX(305);
            setY(280);
        }
        else if (getX()>320 && getX()<365 && getY()>265)
        {
            setX(345);
            setY(280);
        }
        else if (getX()>365 && getX()<400 && getY()>265)
        {
            setX(385);
            setY(280);
        }
//        else
//        {
//            if (similasyonPenceresi->idKefeIt != similasyonPenceresi->idKefe.begin())
//            {
//                logger->log("azaldı");
//                similasyonPenceresi->idKefeIt--;
//                similasyonPenceresi->idKefe.erase(similasyonPenceresi->idKefeIt);
//            }
//        }
    }
}

void
Kutle::mouseReleased(gcn::MouseEvent &event)
{
    setSelected(false);

    if (getX()==305 && getY() == 280)
    {
        logger->log("arttı");
        similasyonPenceresi->idKefe[getID()] = similasyonPenceresi->kefe1;
    }
    else if (getX()==345 && getY() == 280)
    {
        logger->log("arttı");
        similasyonPenceresi->idKefe[getID()] = similasyonPenceresi->kefe2;
    }
    else if (getX()==385 && getY() == 280)
    {
        logger->log("arttı");
        similasyonPenceresi->idKefe[getID()] = similasyonPenceresi->kefe3;
    }
    else
    {
        //similasyonPenceresi->idKefe.insert(std::make_pair(getID(),0));
        similasyonPenceresi->idKefe[getID()] = 0;
    }
}

void
Kutle::setID(int id)
{
    mID = id;
}

int
Kutle::getID()
{
    return mID;
}

void
Kutle::setSelected (bool durum)
{
    mSelected = durum;
}

bool
Kutle::getSelected ()
{
    return mSelected;
}

int
Kutle::getAgirlik()
{
    return mAgirlik;
}

void
Kutle::setAgirlik(int agirlik)
{
    mAgirlik = agirlik;
}

