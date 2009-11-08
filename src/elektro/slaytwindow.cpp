#include "slaytwindow.h"

#include "log.h"
#include "net/ea/npchandler.h"
#include "../npc.h"
#include "elektrowidget.h"
extern int current_npc;
extern ElektroWidget *elektroWidget;

SlaytWindow::SlaytWindow():
    Window(_("Slayt"))
{
    logger->log("Slayt Window Açılır");
    setMinWidth(300);
    setMinHeight(400);
    setResizable(true);
    setVisible(false);
    setSize(520, 450);
    setPosition(100,100);
    padX = 0;
    padY = 0;
    setCloseButton(false);
    slideListModel = new SlideListModel;
    slideListModel->ekle("eSata");

    mDropDown = new DropDown(slideListModel);
    mDropDown->setVisible(false);
    mDropDown->setActionEventId("changeSelection");
    mDropDown->addActionListener(this);

    mStart = new Button("Başla", "Slide_Start",this);
    mCancel = new Button ("Daha Sonra", "Slide_Cancel",this);

    mStart->setVisible(false);
    mCancel->setVisible(false);

    startCancelPosition();

    mClose= new Button ("Kapat", "Slide_Close",this);
    mClose->setVisible(false);

    mNext = new Button ("İleri","Slide_Next",this);
    mPrev = new Button ("Geri","Slide_Prev",this);
    mNext->setVisible(false);
    mPrev->setVisible(false);

    nextPrevPosition();

    add(mDropDown);
    add(mStart);
    add(mCancel);
    add(mNext);
    add(mPrev);
    add(mClose);
}

SlaytWindow::~SlaytWindow()
{
    //delete mDropDown;
    delete mStart;
    delete mCancel;
    delete mNext;
    delete mPrev;
    delete mClose;
}

void
SlaytWindow::startCancelPosition()
{
    mStart->setX((getWidth()-mStart->getWidth()-mCancel->getWidth())/2-5);
    mCancel->setX(mStart->getX()+ mStart->getWidth() + 5);
    mStart->setY(getHeight()-100);
    mCancel->setY(getHeight()-100);
}
void
SlaytWindow::nextPrevPosition()
{
    if (mButtonsDefault)
    {
        mPrev->setX(getWidth()-mNext->getWidth()-mPrev->getWidth()-mDropDown->getWidth()-15);
        mDropDown->setX(mPrev->getX()+mPrev->getWidth()+5);
        mNext->setX(mDropDown->getX()+mDropDown->getWidth()+5);
        mPrev->setY(25);
        mNext->setY(25);
        mDropDown->setY(25);
        mClose->setX(mDropDown->getX() + (mDropDown->getWidth() - mClose->getWidth())/2);
        mClose->setY(0);
        mNext->setVisible(true);
        mPrev->setVisible(true);
        mClose->setVisible(true);
        mDropDown->setVisible(true);
    }
    else
    {
        mPrev->setPosition(mPrevX, mPrevY);
        mNext->setPosition(mNextX, mNextY);
        mClose->setPosition(mCloseX, mCloseY);
        mDropDown->setPosition(mDropDownX, mDropDownY);
        mNext->setVisible(mNextVisible);
        mPrev->setVisible(mPrevVisible);
        mClose->setVisible(mCloseVisible);
        mDropDown->setVisible(mDropDownVisible);
    }
}

void
SlaytWindow::action(const gcn::ActionEvent &event)
{
    logger->log("%s",event.getId().c_str());
    if (event.getId() == "Slide_Cancel")
    {
       Net::getNpcHandler()->listInput(current_npc, 1);
       current_npc=0;
       NPC::isTalking = false;
       setVisible(false);
    }
    else if (event.getId() == "Slide_Start")
    {
        Net::getNpcHandler()->listInput(current_npc, 2);
    }
    else if (event.getId() == "Slide_Next")
    {
        if (mCurrentSlide < mTotalSlides)
           Net::getNpcHandler()->listInput(current_npc, mCurrentSlide+2+1);
    }
    else if (event.getId() == "Slide_Prev")
    {
        if (mCurrentSlide>1)
           Net::getNpcHandler()->listInput(current_npc, mCurrentSlide+2-1);
    }
    else if (event.getId() == "changeSelection")
    {
           Net::getNpcHandler()->listInput(current_npc, mDropDown->getSelected()+3);
    }
    else if (event.getId() == "Slide_Close")
    {
           logger->log("Kapan sunu kapan :%d  ",mDropDown->getSelected());
           Net::getNpcHandler()->listInput(current_npc, 0xff);
           current_npc=0;
           NPC::isTalking =false;
           setVisible(false);
           clearOldSlide();
    }
}

void
SlaytWindow::draw(gcn::Graphics *graphics)
{
    Graphics *g = static_cast<Graphics*>(graphics);

    Window::draw(graphics);
    for (miImage = mvImage.begin();
            miImage != mvImage.end();
            ++miImage)
    {
         if (miImage->visible)
            g->drawImage(miImage->img,miImage->x,miImage->y);
    }

     for (miAnim = mvAnim.begin();
           miAnim != mvAnim.end();
           ++miAnim)
     {
        if (miAnim->visible)
        {
            Image *mImage1 = miAnim->anim->getCurrentImage();
            g->drawImage(mImage1,miAnim->x,miAnim->y);
            miAnim->anim->update(miAnim->v);
        }
     }

    drawChildren(graphics);
}

void
SlaytWindow::slideStateControl()
{
    switch(slideState)
    {
        case SLIDE_MESSAGE_STATE:
            startCancelPosition();
            setVisible(true);
            mStart->setVisible(true);
            mCancel->setVisible(true);
            break;
        case SLIDE_NEW_STATE:
            mStart->setVisible(false);
            mCancel->setVisible(false);
            nextPrevPosition();
            for(TmiLabel it =mvLabel.begin(); it<mvLabel.end(); it++)
                (*it)->setVisible(true);
            break;
    }
}
void
SlaytWindow::clearOldSlide()
{
    mNext->setVisible(false);
    mPrev->setVisible(false);
    mDropDown->setVisible(false);
    mClose->setVisible(false);

    TmiLabel miLabel;
    for(miLabel = mvLabel.begin();
        miLabel != mvLabel.end();
        ++miLabel)
    {
        delete(*miLabel); //eski buttonları sil
    }
    mvLabel.clear();

    for (miTextBox = mvTextBox.begin();
            miTextBox != mvTextBox.end();++miTextBox)
    {
          delete(*miTextBox);
    }
    mvTextBox.clear();

    for (miScrollArea = mvScrollArea.begin();
            miScrollArea != mvScrollArea.end();++miScrollArea)
    {
        delete(*miScrollArea);
    }
    mvScrollArea.clear();

    for (miImage = mvImage.begin(); miImage<mvImage.end(); miImage++)
    {
        ResourceManager *resman = ResourceManager::getInstance();
        resman->release((*miImage).img);
    }

    mvImage.clear();

    mvAnim.clear();
}

void
SlaytWindow::parseXML(std::string mDoc)
{
    logger->log("Slayt parse işlemi başladı : \n %s", mDoc.c_str());
    ResourceManager *resman = ResourceManager::getInstance();
    mxmlDoc = xmlParseMemory(mDoc.c_str(),mDoc.size());
    if (!mxmlDoc)
    {
        localChatTab->chatLog("Bu üstad'ın morali bozuk :(", BY_SERVER);
        localChatTab->chatLog("Bu durumu bir yöneticiye haber versen çok iyi olur.", BY_SERVER);
        return;
    }

    xmlNodePtr rootNode = xmlDocGetRootElement(mxmlDoc);
    if (!rootNode || !xmlStrEqual(rootNode->name, BAD_CAST "presentation"))
    {
        localChatTab->chatLog("Bu üstad'ın morali bozuk :(", BY_SERVER);
        localChatTab->chatLog("Bu durumu bir yöneticiye haber versen çok iyi olur.", BY_SERVER);
        return;
    }

    for_each_xml_child_node(node, rootNode)
    {
        // Testten önce gösterilen mesaj/
        if (xmlStrEqual(node->name, BAD_CAST "presentationpro"))
        {
            clearOldSlide();
            slideState = SLIDE_MESSAGE_STATE;
            int x = XML::getProperty(node, "x", 50);
            int y = XML::getProperty(node, "y", 50);
            int w = XML::getProperty(node, "w", 100);
            int h = XML::getProperty(node, "h", 400);
            setPosition (x,y);
            setSize(w,h);
            mTotalSlides = XML::getProperty(node, "totalslides", 0);
            slideListModel->temizle();
            for (int i = 1; i <= mTotalSlides; i++)
            {
                slideListModel->ekle(toString(i));
            }
            mButtonsDefault = true;
         }
         if (xmlStrEqual(node->name, BAD_CAST "slidebutton"))
         {
            mButtonsDefault = false;
            int x = XML::getProperty(node, "x", 0);
            int y = XML::getProperty(node, "y", 0);
            std::string visi = XML::getProperty(node, "visible", "show");
            bool visib;
            visib = (visi=="show" ? true: false);
            std::string obj = XML::getProperty(node, "object", "next");
            logger->log("%s -> %d - %d -%d", obj.c_str(),x, y, visib);
            if(obj=="next")
            {
                mNextX = x;
                mNextY = y;
                mNextVisible = visib;
            }
            else if(obj=="prev")
            {
                mPrevX = x;
                mPrevY = y;
                mPrevVisible = visib;
            }
            else if(obj=="close")
            {
                mCloseX = x;
                mCloseY = y;
                mCloseVisible = visib;
            }
            else if(obj=="combo")
            {
                mDropDownX = x;
                mDropDownY = y;
                mDropDownVisible = visib;
            }
         }
         else if (xmlStrEqual(node->name, BAD_CAST "label"))
         {
            gcn::Label *templabel = elektroWidget->addLabel(node);
            add(templabel);
            mvLabel.push_back(templabel);
         }
         else if (xmlStrEqual(node->name, BAD_CAST "image"))
         {
             SmImage temp = elektroWidget->addImage(node);
             mvImage.push_back(temp);
         }
         else if (xmlStrEqual(node->name, BAD_CAST "simpleanim"))
         {
             SmAnim temp = elektroWidget->addAnim(node);
             mvAnim.push_back(temp);
         }else if (xmlStrEqual(node->name, BAD_CAST "textbox"))
         {
             SmTextBox temp = elektroWidget->addTextBox(this,node);
             add(temp.scrollarea);
             mvScrollArea.push_back(temp.scrollarea);
             mvBrowserBox.push_back(temp.browserbox);
         }

        else if (xmlStrEqual(node->name, BAD_CAST "slide"))
        {
            slideState = SLIDE_NEW_STATE;
            clearOldSlide();
            mCurrentSlide = XML::getProperty(node, "slideid", 1);
            std::string slidename =  XML::getProperty(node, "name", "slayt :"+toString(mCurrentSlide));
            slideListModel->degistir(slidename, mCurrentSlide-1);
            mDropDown->setSelected(mCurrentSlide-1);
        }
     }
     slideStateControl();
}
