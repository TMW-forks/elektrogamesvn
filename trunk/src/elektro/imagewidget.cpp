#include "imagewidget.h"
#include "../resources/resourcemanager.h"
#include "../resources/image.h"
#include "../graphics.h"

ImageWidget::ImageWidget(std::string imagename, bool sets)
{
    setFrameSize(0);
    setTitleBarHeight(0);
    mSet = sets;
    if (imagename != "")
    {
        // Load the skin
        ResourceManager *resman = ResourceManager::getInstance();

        if(mSet)
        {
            Image *temp = resman->getImage("graphics/"+imagename);
            temp->decRef();
            mImageSet = resman->getImageSet("graphics/"+imagename,temp->getWidth()/4,temp->getHeight());
            setSize(temp->getWidth()/4,temp->getHeight());
            setSize(temp->getWidth()/4,temp->getHeight());
        }
        else
        {
            Image *temp = resman->getImage("graphics/"+imagename);
            temp->decRef();
            mImageSet = resman->getImageSet("graphics/"+imagename,temp->getWidth(),temp->getHeight());
            setSize(temp->getWidth(),temp->getHeight());
            setSize(temp->getWidth(),temp->getHeight());
        }

    }
}

ImageWidget::~ImageWidget()
{
    mImageSet->decRef();
}

void
ImageWidget::draw(gcn::Graphics* graphics)
{
    mMode = (mMode<0 || mMode >4? 0 : mMode);
    Graphics *g = static_cast<Graphics*>(graphics);
    if (mImageSet != NULL)
    {
        if(!mImageSet->get(mMode))
        {
            graphics->setColor(gcn::Color(0xff0000));
            g->drawRectangle(gcn::Rectangle(0,0,getWidth(),getHeight()));
        }
        else
            g->drawImage(mImageSet->get(mMode),0,0);
    }
}
