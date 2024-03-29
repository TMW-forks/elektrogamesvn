#include "kutle.h"
#include "similasyonpenceresi.h"

#include <SDL_types.h>
#include <guichan/sdl/sdlinput.hpp>

#include "../graphics.h"
#include "../log.h"
#include "../game.h"

#include <utility> //make_pair() için
#include <cmath>

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
    resimIndex=3;
    setHareket(false);
    mAlfa = 1;
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
            setY(284);
        }
        //2.Kefe
        else if (getX()>320 && getX()<365 && getY()>265)
        {
            setX(345);
            setY(284);
        }
        //3.Kefe
        else if (getX()>365 && getX()<400 && getY()>265)
        {
            setX(385);
            setY(284);
        }
    }
}

void
Kutle::mouseReleased(gcn::MouseEvent &event)
{
    std::vector<int> agirlikKefe;
    setSelected(false);

    if (getX()==305 && getY() == 284)
    {
        agirlikKefe.push_back(similasyonPenceresi->kefe1);
        agirlikKefe.push_back(getAgirlik());
        similasyonPenceresi->idKefe[getID()]= agirlikKefe;
        setHareket(true);
        mR = 32;
        mX = getX()-30;
        mY = 225+92-32;
    }
    else if (getX()==345 && getY() == 284)
    {
        agirlikKefe.push_back(similasyonPenceresi->kefe2);
        agirlikKefe.push_back(getAgirlik());
        similasyonPenceresi->idKefe[getID()]= agirlikKefe;
        setHareket(true);
        mR = 79;
        mX = getX()-78;
        mY = 225+92-40;
    }
    else if (getX()==385 && getY() == 284)
    {
        agirlikKefe.push_back(similasyonPenceresi->kefe3);
        agirlikKefe.push_back(getAgirlik());
        similasyonPenceresi->idKefe[getID()]= agirlikKefe;
        setHareket(true);
        mR = 119;
        mX = getX()-64-getWidth();
        mY = 225+92-(getHeight());
    }
    else
    {
        agirlikKefe.push_back(0);
        agirlikKefe.push_back(getAgirlik());
        similasyonPenceresi->idKefe[getID()]= agirlikKefe;
        setHareket(false);
    }

}

void
Kutle::hesaplaY2()
{
    int y=0;
    y = getY() + sin(-15*(22/7.0)/180)*(getX()-282);
//    y = getY() + tan(-15*(22/7.0)/180)*(getX()-300);
    setY(y);
}

void
Kutle::hesaplaY()
{
    int y=0;
    int x=0;

    mAlfa = (resimIndex - 2)*6;
    y = mY + sin(mAlfa*(22/7.0)/180)*mR;
    mAlfa = (resimIndex - 2)*4;
    x = mX + cos(mAlfa*(22/7.0)/180)*mR;
//    y = getY() + sin(15*(22/7.0)/180)*(getX()-352);
//    y = getY() + tan(15*(22/7.0)/180)*(getX()-355);
//    y = getY() + sin(15*(22/7.0)/180)*34;
    setY(y);
    setX(x);
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
Kutle::setHareket (bool durum)
{
    mHareket = durum;
}

bool
Kutle::getHareket ()
{
    return mHareket;
}

void
Kutle::setSelected(bool durum)
{
    mSelected = durum;
}

bool
Kutle::getSelected()
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

void
Kutle::setResimIndex(int index)
{
    resimIndex = index;
}

int
Kutle::getResimIndex()
{
    return resimIndex;
}
