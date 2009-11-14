#include "component.h"

#include <SDL_types.h>
#include <guichan/sdl/sdlinput.hpp>

#include "../resources/image.h"
#include "../resources/resourcemanager.h"
#include "../resources/imageset.h"
#include "../graphics.h"
#include "../log.h"
#include "../game.h"
#include "circuitwindow.h"
#include "utils/stringutils.h"
#include <cmath>

extern CircuitWindow *circuitWindow;

Component::Component(gcn::ActionListener *listener, Node *n1, Node *n2):
    gcn::Window("compo"),
    mListener(listener)

{
    mAngel=0;
    mSelected =false;
    mInterval =10;
    mDead = false;
    mDeletable =true;
    setFrameSize(0);
    setPadding(0);
    setTitleBarHeight(0);
    setMovable(true);
    setVisible(true);
    setYon(0);
    setCurrent(0);

    ResourceManager *resman = ResourceManager::getInstance();
    ImageSet *mHalkaImages;
    mHalkaImages = resman->getImageSet("graphics/gui/target-cursor-blue-s.png", 44, 35);
    Animation *mHalkaAnime = new Animation();
    for (unsigned int i = 0; i < mHalkaImages->size(); ++i)
         mHalkaAnime->addFrame(mHalkaImages->get(i), 75, 0, 0);
    mSelectedAnime = new SimpleAnimation(mHalkaAnime);

    setWidth(40);
    setHeight(40);

    node1 = n1;
    node2 = n2;
    n1->setOwner(this);
    n2->setOwner(this);

    if (mListener)
    {
        addActionListener(mListener);
    }
}

Component::~Component()
{
    delete mSelectedAnime;
}

void Component::setX (int x)
{
    gcn::Window::setX(x);
    mX = x;
            //node için açý kontrolü yapýlacak (180 derce yapýlýrsa olabilir)

}

void Component::setY (int y)
{
    gcn::Window::setY(y);
    mY = y;
}

void Component::setW (int w)
{
    mW = w;
    gcn::Window::setWidth(w);
    nodesCalc();
}

void Component::setH (int h)
{
    mH = h;
    gcn::Window::setHeight(h);
    nodesCalc();
}

int Component::getW()
{
    return mW;
}

int Component::getH ()
{
    return mH;
}

void Component::setBounce(int x, int y, int w, int h)
{
    setW (w);
    setH (h);
    setX (x);
    setY (y);
}

void Component::setSelectable (bool s)
{
    mSelectable = s;
}

bool Component::getSelectable ()
{
    return mSelectable;
}

void Component::setSelected (bool s)
{
    if (mSelectable)
        mSelected = s;
}

bool Component::getSelected ()
{
    return mSelected;
}

void Component::setType (Type t)
{
    mType = t;
}

Type Component::getType ()
{
    return mType;
}

void Component::setStatus (Status s)
{
    mStatus = s;
}


void Component::setStatus (int s)
{
    switch(s){
        case 0: mStatus= PASIVE;break;
        case 1: mStatus=ACTIVE;break;
        case 2: mStatus=BURNED;break;
        case 3: mStatus=PLUS;break;
        case 4: mStatus=PLUS2;break;
    }
}

Status Component::getStatus ()
{
    return mStatus;
}

void Component::draw(gcn::Graphics *graphics)
{
    graphics->setColor(gcn::Color(0xff0000));
    graphics->drawRectangle(gcn::Rectangle(0,0,getWidth(),getHeight()));
    Graphics *g = static_cast<Graphics*>(graphics);
    ResourceManager *resman = ResourceManager::getInstance();
    if (getSelected())
    {
        mSelectedAnime->update(10);
        Image* targetCursor=mSelectedAnime->getCurrentImage();
        g->drawImage(targetCursor, -2, 2);
    }
    drawChildren(graphics);
}

void Component::mousePressed(gcn::MouseEvent &event)
{

    if (getSelectable())
    {
        Uint8* keys;
        keys = SDL_GetKeyState(NULL);
        requestMoveToTop();
        node1->requestMoveToTop();
        node2->requestMoveToTop();
        xx = event.getX();
        yy = event.getY();
        nodesCalc();

        if (keys[SDLK_LCTRL] || keys[SDLK_RCTRL])
        {
            setSelected(true);
        }
        else
        {
            if (!getSelected())
            {
                circuitWindow->clearSelections();
                setSelected(true);
                mNewSelected=true;
            }
        }
        if (isMovable() && !((keys[SDLK_LALT] && keys[SDLK_RALT])))
        {
            mDragOffsetX = event.getX();
            mDragOffsetY = event.getY();
            mIsMoving = true;
        }
        if (isMovable()&&(keys[SDLK_LALT] || keys[SDLK_RALT]))
        {
            mFirstX=event.getX();
            mFirstY=event.getY();
        }
    }
   nodesCalc();
}

void Component::mouseReleased(gcn::MouseEvent &event)
{
   Uint8* keys;
   keys = SDL_GetKeyState(NULL);
   if (event.getButton() == gcn::MouseEvent::RIGHT && mDeletable)
   {
        const std::string &actionEventId="com_close";
        setActionEventId(actionEventId);
        distributeActionEvent();
   }
   else if (!keys[SDLK_LCTRL] && !keys[SDLK_RCTRL])
   {
        if (getSelected()&&mNewSelected==false)
        {
            circuitWindow->clearSelections();
            mNewSelected = false;
        }
        else
        {
            circuitWindow->clearSelections();
            setSelected(true);
        }
   }
   nodesCalc();
}

void Component::mouseDragged(gcn::MouseEvent &event)
{
//    circuitWindow->moveComponents(mDragOffsetX,mDragOffsetY);
        Uint8* keys;
        keys = SDL_GetKeyState(NULL);
        if (isMovable()) mNewSelected=true;

        if (isMovable() && (keys[SDLK_LALT] || keys[SDLK_RALT]))
        {
            if (event.getY()-mFirstY>mInterval)
            {
                const std::string &actionEventId="com_rotate_y-";
                mFirstY = event.getY();
                setActionEventId(actionEventId);
                distributeActionEvent();
            }
            if (event.getY()-mFirstY<-mInterval)
            {
                const std::string &actionEventId="com_rotate_y+";
                mFirstY = event.getY();
                setActionEventId(actionEventId);
                distributeActionEvent();
            }
        }
        else
        {
//        circuitWindow->mHint->setCaption(toString(event.getX())+" - "+toString(event.getX()));
//        circuitWindow->mHint->adjustSize();
            requestMoveToTop();
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            setX(getX()+event.getX()-xx);
            setY(getY()+event.getY()-yy);
            logger->log("mouse x: %d     mouse y : %d",event.getX(),event.getY() );
        if (getX()<20) setX(20);
        if (getY()<50) setY(50);
        if (getX()>(circuitWindow->getWidth()-getWidth()-15)) setX(circuitWindow->getWidth()-15-getWidth());
        if (getY()>(circuitWindow->getHeight()-getHeight()-60)) setY(circuitWindow->getHeight()-getHeight()-60);
        }
        nodesCalc();
}

void Component::setActionListener (gcn::ActionListener *listener)
{
    mListener = listener;
}

void Component::setAngel(int a)
{
    mAngel = a;
    nodesCalc();
}

int Component::getAngel()
{
    return mAngel;
}

void Component::setDeletable(bool s)
{
    mDeletable = s;
}

bool Component::getDeletable()
{
    return mDeletable;
}

void Component::nodesCalc()
{
    int xx,yy;
    xx = cos(mAngel*15*(22/7.0)/180)*22;
    yy = sin(mAngel*15*(22/7.0)/180)*22;

    node1->setX(20-xx+getX()-4);
    node1->setY(20-yy+getY()-4);
    node2->setX(20+xx+getX()-4);
    node2->setY(20+yy+getY()-4);
}

int Component::sgn (long a)
{
  if (a > 0) return +1;
  else if (a < 0) return -1;
  else return 0;
}

void Component::drawCircle(gcn::Graphics *g, int xc, int yc, int r)
{
    int x = 0;
int y = r;
int p = 3 - 2 * r;
   while (x <= y)
   {
      g->drawPoint(xc + x, yc + y);
      g->drawPoint(xc - x, yc + y);
      g->drawPoint(xc + x, yc - y);
      g->drawPoint(xc - x, yc - y);
      g->drawPoint(xc + y, yc + x);
      g->drawPoint(xc - y, yc + x);
      g->drawPoint(xc + y, yc - x);
      g->drawPoint(xc - y, yc - x);
      if (p < 0)
         p += 4 * x++ + 6;
      else
         p += 4 * (x++ - y--) + 10;
   }
}

void Component::drawLine(gcn::Graphics *g,int a,int b,int c,int d, int t)
{
  long u,s,v,d1x,d1y,d2x,d2y,m,n;
  int  i;
  u   = c-a;
  v   = d-b;
  d1x = sgn(u);
  d1y = sgn(v);
  d2x = sgn(u);
  d2y = 0;
  m   = abs(u);
  n   = abs(v);
  if (m<=n) {
    d2x = 0;
    d2y = sgn(v);
    m   = abs(v);
    n   = abs(u);
  }
  s = (int)(m / 2);
  for (i=0;i<round(m);i++) {
//    g->drawPoint(a,b);
    drawCircle(g,a,b,t);
    s += n;
    if (s >= m) {
      s -= m;
      a += d1x;
      b += d1y;
    }
    else {
      a += d2x;
      b += d2y;
    }
  }
}

void Component::setValue(float v)
{
    mValue = v;
}

float Component::getValue()
{
    return mValue;
}

void Component::setId(int id)
{
    mId=id;
}

int Component::getId()
{
    return mId;
}

void Component::setItemId(int id)
{
    mItemId=id;
}

int Component::getItemId()
{
    return mItemId;
}

void
Component::mouseMoved(gcn::MouseEvent &event)
{
    //
}
void Component::mouseEntered(gcn::MouseEvent &event)
{
    std::string typeName[]= {
     "Bilinmiyor",
     "Tel",
     "Direnc",
     "Pil",
     "Diyot",
     "Lamba",
     "Dugme"};

    Uint8* keys;
    keys = SDL_GetKeyState(NULL);
    if (isMovable() && ((keys[SDLK_LALT] || keys[SDLK_RALT]))) gui->setCursorType(5);
    else if (isMovable())  gui->setCursorType(6);
    circuitWindow->toolErase = mDeletable;
    circuitWindow->toolFromLeft = node1->getFromLink();
    circuitWindow->toolFromRight = node2->getFromLink();
    circuitWindow->toolToRight = node1->getToLink();
    circuitWindow->toolToLeft = node2->getToLink();
    circuitWindow->toolMove = mMovable;
    circuitWindow->toolSelect = mSelectable;
    circuitWindow->toolRotate = mMovable;
    circuitWindow->toolCaption->setCaption(typeName[mType]+" :"+toString(getId()));
    circuitWindow->toolCaption->adjustSize();
    circuitWindow->toolValue->setCaption(toString(mValue));
    circuitWindow->toolValue->adjustSize();
    circuitWindow->mHint->setCaption("comp:"+toString(mCurrent));
    circuitWindow->mHint->adjustSize();
}

void Component::mouseExited(gcn::MouseEvent &event)
{
     gui->setCursorType(0);
}

 void Component::action(const gcn::ActionEvent &event)
 {
    logger->log(" ++++++++++++++++ ACT ++++++++++++++++++++");
 }
