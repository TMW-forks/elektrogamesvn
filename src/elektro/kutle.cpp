#include "kutle.h"

Kutle::Kutle(gcn::ActionListener *listener)
{
    ResourceManager *resman = ResourceManager::getInstance();
    ImageSet *mHalkaImages;
    mHalkaImages = resman->getImageSet("graphics/gui/target-cursor-blue-s.png", 44, 35);
    Animation *mHalkaAnime = new Animation();
    for (unsigned int i = 0; i < mHalkaImages->size(); ++i)
         mHalkaAnime->addFrame(mHalkaImages->get(i), 75, 0, 0);
    mSelectedAnime = new SimpleAnimation(mHalkaAnime);
}

Kutle::~Kutle()
{
    //dtor
}

void Kutle::draw(gcn::Graphics *graphics)
{
    graphics->setColor(gcn::Color(0xff0000));
    //graphics->drawRectangle(gcn::Rectangle(0,0,getWidth(),getHeight()));
    Graphics *g = static_cast<Graphics*>(graphics);
    //ResourceManager *resman = ResourceManager::getInstance();

    if (true) //if (getSelected())
    {
        mSelectedAnime->update(10);
        Image* targetCursor=mSelectedAnime->getCurrentImage();
        g->drawImage(targetCursor, -2, 2);
    }
    drawChildren(graphics);
}

bool Kutle::getSelected ()
{
    return mSelected;
}
void Kutle::mousePressed(gcn::MouseEvent &event)
{
    gcn::Window::mousePressed(event);

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        const int x = event.getX();
        const int y = event.getY();

//     //Handle close button
//        gcn::Rectangle closeButtonRect(getWidth() - 40,getPadding(),50,50);
//
//        if (closeButtonRect.isPointInRect(x, y))
//        {
//            close();
//        }
    }

        // Handle window resizing
        //fareResize = getResizeHandles(event);
}
void Kutle::mouseDragged(gcn::MouseEvent &event)
{

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

void Kutle::setW(int w)
{
    mW=w;
}
void Kutle::setH(int h)
{
    mH= h;
}

void Kutle::setKoordinat(int x,int y)
{
    mX=x;
    mY=y;
}

int Kutle::getW()
{
    return mW;
}
int Kutle::getH()
{
    return mH;
}
void Kutle::getKoordinat(int &x,int &y)
{
    x = getX();
    y = getY();
}
void Kutle::action(const gcn::ActionEvent &event)
 {

 }
