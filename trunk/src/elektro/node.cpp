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
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            setX(mouseX-circuitWindow->getX()-getWidth());
            setY(mouseY-circuitWindow->getY()-circuitWindow->getTitleBarHeight()-getHeight());
            circuitWindow->collisionCheck = true;
        if (getX()<40) setX(40);
        if (getY()<50) setY(50);
        if (getX()>(circuitWindow->getWidth()-getWidth()-45)) setX(circuitWindow->getWidth()-45-getWidth());
        if (getY()>(circuitWindow->getHeight()-getHeight()-88)) setY(circuitWindow->getHeight()-getHeight()-88);
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

void Node::mouseEntered(gcn::MouseEvent &mouseEvent)
{    Uint8* keys;
    keys = SDL_GetKeyState(NULL);
    //devre penceresinde shiftle yeni node oluþmasýný engelle
    circuitWindow->setNodeCreate(false);
    if (mFromLink && (keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT]))
    {
        gui->setCursorType(3);
    }
    else if (mMovable)
        gui->setCursorType(1);

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
