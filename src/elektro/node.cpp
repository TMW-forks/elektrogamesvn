#include "node.h"

#include <sstream>
#include "../resources/image.h"
#include "../resources/resourcemanager.h"
#include "../resources/imageset.h"
#include "../game.h"
#include "elektro/circuitwindow.h"
#include "gui/chat.h"
#include "../log.h"
#include "utils/stringutils.h"

extern CircuitWindow *circuitWindow;

Node::Node(const std::string& caption,const std::string& hint,
            const std::string &actionEventId,
            gcn::ActionListener *listener):
    BitButton(caption, hint, actionEventId,listener)
{
    setRightClick(false);
    setCreator(false);
    setDead(false);
    setMoving (false);
    setOwner(NULL);
    setFrameSize(0);
    setCurrent(0);
}

Node::~Node()
{

}

void Node::draw(gcn::Graphics *graphics)
{
    BitButton::draw(graphics);
}

void Node::mousePressed(gcn::MouseEvent& mouseEvent)
{
    Uint8* keys;
    keys = SDL_GetKeyState(NULL);
    xx = mouseEvent.getX();
    yy = mouseEvent.getY();
    if (mouseEvent.getButton() == gcn::MouseEvent::RIGHT)
    {
        mRightClick = true;
    }
    else if (mSelectable && mouseEvent.getButton() == gcn::MouseEvent::LEFT)
    {
        mMoving = true;
    } else
    BitButton::mousePressed(mouseEvent);
}

void Node::mouseDragged(gcn::MouseEvent& mouseEvent)
{
    Uint8* keys;
    keys = SDL_GetKeyState(NULL);
    if (mMoving)
    {
        if(mToLink && !mCreator && (keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT]))
        {
            const std::string &actionEventId="node_shift";
            mCreator = true;
            setActionEventId(actionEventId);
            distributeActionEvent();
        }
        else if(mMovable && mSelectable)
        {
            requestMoveToTop();
            setX(getX()+mouseEvent.getX()-xx);
            setY(getY()+mouseEvent.getY()-yy);
            circuitWindow->collisionCheck = true;
        if (getX()<120) setX(120);
        if (getY()<10) setY(10);
        if (getX()>(circuitWindow->getWidth()-getWidth()-15)) setX(circuitWindow->getWidth()-15-getWidth());
        if (getY()>(circuitWindow->getHeight()-getHeight()-25)) setY(circuitWindow->getHeight()-getHeight()-25);
        circuitWindow->mWireRefresh = true;

        }
    }
}

void Node::mouseReleased(gcn::MouseEvent& mouseEvent)
{
    mMoving = false;
    mCreator = false;
    if (circuitWindow->nodeCollision)
        circuitWindow->bindingNodes();
    circuitWindow->collisionCheck = false;
    circuitWindow->nodeCollision = false;

    if (mRightClick && mDeletable && mFree)
    {
        mRightClick =false;
        mDead = true;
        const std::string &actionEventId="node_close";
        setActionEventId(actionEventId);
        distributeActionEvent();
    }
    BitButton::mouseReleased(mouseEvent);
}
void
Node::mouseMoved(gcn::MouseEvent &event)
{
    //
}

void Node::mouseEntered(gcn::MouseEvent &mouseEvent)
{    Uint8* keys;
    keys = SDL_GetKeyState(NULL);
    //devre penceresinde shiftle yeni node oluþmasýný engelle
    circuitWindow->setNodeCreate(false);
    circuitWindow->mHint->setCaption("node:"+toString(mCurrent));
    circuitWindow->mHint->adjustSize();
    if (mFromLink && (keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT]))
    {
        gui->setCursorType(5);
    }
    else if (mMovable)
        gui->setCursorType(6);

    circuitWindow->toolErase = mDeletable;
    circuitWindow->toolFromRight = mFromLink;
    circuitWindow->toolToRight = mToLink;
    circuitWindow->toolMove = mMovable;
    circuitWindow->toolFromLeft= false;
    circuitWindow->toolToLeft = false;
    circuitWindow->toolRotate = false;
    circuitWindow->toolSelect = false;
    circuitWindow->toolCaption->setCaption("Baglanti :"+toString(getId()));
    circuitWindow->toolCaption->adjustSize();
    circuitWindow->toolCaption->setX(55-circuitWindow->toolCaption->getWidth()/2);
}

void Node::mouseExited(gcn::MouseEvent &mouseEvent)
{
    gui->setCursorType(0);
    circuitWindow->setNodeCreate(true);  //artýk node oluþturabilir.
}

void
Node::esitle(Node* atanan)
{
    this->mId = atanan->mId;
    this->setX(atanan->getX());
    this->setY(atanan->getY());
    this->mSelectable =  atanan->mSelectable;
    this->mFree = atanan->mFree;
    this->mDead = atanan->mDead;
    this->mToLink = atanan->mToLink;
    this->mFromLink = atanan->mFromLink;
    this->mRightClick = atanan->mRightClick;
    this->mOwner = atanan->mOwner;
}
