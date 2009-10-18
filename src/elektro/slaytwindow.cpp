#include "slaytwindow.h"

#include "log.h"
#include "net/ea/npchandler.h"
#include "../npc.h"
extern int current_npc;

SlaytWindow::SlaytWindow()
{
    setWindowName("Slayt Penceresi");
    logger->log("Slayt Window Açılır");

    setMinWidth(300);
    setMinHeight(400);
    setResizable(true);
    setVisible(false);
    setSize(520, 450);
    setPosition(100,100);
    padX = 0;
    padY = 0;

    slideListModel = new SlideListModel;
    slideListModel->ekle("eSata");

    mDropDown = new DropDown(slideListModel);
    mDropDown->setVisible(false);

    mStart = new Button("Başla", "Slide_Start",this);
    mCancel = new Button ("Daha Sonra", "Slide_Cancel",this);
    mStart->setVisible(false);
    mCancel->setVisible(false);

    startCancelPosition();

    mNext = new Button ("İleri","Slide_Next",this);
    mPrev = new Button ("Prev","Slide_Prev",this);
    mNext->setVisible(false);
    mPrev->setVisible(false);

    nextPrevPosition();

    add(mDropDown);
    add(mStart);
    add(mCancel);
    add(mNext);
    add(mPrev);
}

SlaytWindow::~SlaytWindow()
{
    delete mDropDown;
    delete mStart;
    delete mCancel;
    delete mNext;
    delete mPrev;
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
    mPrev->setX(getWidth()-mNext->getWidth()-mPrev->getWidth()-mDropDown->getWidth()-15);
    mDropDown->setX(mPrev->getX()+mPrev->getWidth()+5);
    mNext->setX(mDropDown->getX()+mDropDown->getWidth()+5);
    mPrev->setY(0);
    mNext->setY(0);
    mDropDown->setY(0);
}

void
SlaytWindow::action(const gcn::ActionEvent &event)
{
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
        if (mCurrentSlide<mTotalSlides);
           Net::getNpcHandler()->listInput(current_npc, mCurrentSlide+2+1);
    }
    else if (event.getId() == "Slide_Prev")
    {
        if (mCurrentSlide>1)
           Net::getNpcHandler()->listInput(current_npc, mCurrentSlide+2-1);
    }
}

void
SlaytWindow::draw(gcn::Graphics *graphics)
{
    Window::draw(graphics);
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
            mNext->setVisible(true);
            mPrev->setVisible(true);
            mDropDown->setVisible(true);
            for(TmiLabel it =mvLabel.begin(); it<mvLabel.end(); it++)
                (*it)->setVisible(true);
            break;
    }
}
void
SlaytWindow::clearOldSlide()
{
    TmiLabel miLabel;
    for(miLabel = mvLabel.begin();
        miLabel != mvLabel.end();
        ++miLabel)
    {
        delete(*miLabel); //eski buttonları sil
    }
    mvLabel.clear();
}

void
SlaytWindow::parseXML(std::string mDoc)
{
//    /******/
//mDoc ="<?xml version=\"1.0\" encoding=\"utf-8\" ?> \
//        <presentation> \
//        <presentationpro x =\"50\" y=\" 50\" w=\"400\" h=\"300\" totalslides=\"5\"> \
//        </presentation>";
//
//
//    /*******/
logger->log("Slayt parse işlemi başladı");
    ResourceManager *resman = ResourceManager::getInstance();
    logger->log(mDoc.c_str());
    mxmlDoc = xmlParseMemory(mDoc.c_str(),mDoc.size());
    if (!mxmlDoc)
    {
        logger->error("slayt.cpp: Error while parsing slayt xml (from npc.xml)!"+mDoc);
    }

    xmlNodePtr rootNode = xmlDocGetRootElement(mxmlDoc);
    if (!rootNode || !xmlStrEqual(rootNode->name, BAD_CAST "presentation"))
    {
        logger->error("slayt.cpp: from npc xml is not a valid database file!"+mDoc);
    }

    for_each_xml_child_node(node, rootNode)
    {
        // Testten önce gösterilen mesaj
        if (xmlStrEqual(node->name, BAD_CAST "presentationpro"))
        {
            slideState = SLIDE_MESSAGE_STATE;
            int x = XML::getProperty(node, "x", 50);
            int y = XML::getProperty(node, "y", 50);
            int w = XML::getProperty(node, "w", 100);
            int h = XML::getProperty(node, "h", 400);
            setPosition (x,y);
            setSize(w,h);
            mTotalSlides = XML::getProperty(node, "totalslides", 0);
        }
        else if (xmlStrEqual(node->name, BAD_CAST "slide"))
        {
            slideState = SLIDE_NEW_STATE;
            clearOldSlide();
            mCurrentSlide = XML::getProperty(node, "slideid", 1);
            for_each_xml_child_node(subnode, node)
            {
                if (xmlStrEqual(subnode->name, BAD_CAST "label"))
                {
                    gcn::Label *templabel=new gcn::Label("");
                    templabel->setCaption(XML::getProperty(subnode, "text", "label"));
                    templabel->setX(XML::getProperty(subnode, "x", 0)+padX);
                    templabel->setY(XML::getProperty(subnode, "y", 0)+padY);
                    int w=XML::getProperty(subnode, "width", 0);
                    int h=XML::getProperty(subnode, "height", 0);
                    if (w==0||h==0)
                        templabel->adjustSize();
                      else
                        templabel->setSize(w,h);

                    int r = XML::getProperty(subnode, "fcolorr", 0);
                    int g = XML::getProperty(subnode, "fcolorg", 0);
                    int b = XML::getProperty(subnode, "fcolorb", 0);
                    if (r!=0 && g!=0 && b!=0) templabel->setForegroundColor(gcn::Color(r,g,b));

                    r = XML::getProperty(subnode, "bcolorr", 0);
                    g = XML::getProperty(subnode, "bcolorg", 0);
                    b = XML::getProperty(subnode, "bcolorb", 0);
                    if (r!=0 && g!=0 && b!=0) templabel->setBackgroundColor(gcn::Color(r,g,b));

                    std::string font = XML::getProperty(subnode, "font", "boldFont");
                    if (font=="boldFont") templabel->setFont(boldFont);
                     else if (font=="bas_1") templabel->setFont(font_bas_1);
                     else if (font=="bas_2") templabel->setFont(font_bas_2);
                     else if (font=="bas_3") templabel->setFont(font_bas_3);
                     else if (font=="bas_4") templabel->setFont(font_bas_4);
                     else if (font=="bas_5") templabel->setFont(font_bas_5);
                     else if (font=="bas_b_1") templabel->setFont(font_bas_b_1);
                     else if (font=="bas_b_2") templabel->setFont(font_bas_b_2);
                     else if (font=="el_1") templabel->setFont(font_el_1);
                     else if (font=="el_2") templabel->setFont(font_el_2);
                     else if (font=="el_3") templabel->setFont(font_el_3);
                     else if (font=="el_b_1") templabel->setFont(font_el_b_1);
                     else if (font=="el_b_2") templabel->setFont(font_el_b_2);
                     else if (font=="txt_1") templabel->setFont(font_txt_1);
                     else if (font=="txt_2") templabel->setFont(font_txt_2);
                     else if (font=="txt_3") templabel->setFont(font_txt_3);
                     else if (font=="txt_4") templabel->setFont(font_txt_4);
                     else if (font=="txt_5") templabel->setFont(font_txt_5);
                     else if (font=="txt_6") templabel->setFont(font_txt_6);
                     else if (font=="txt_b_1") templabel->setFont(font_txt_b_1);
                     else if (font=="txt_b_2") templabel->setFont(font_txt_b_2);
                     else if (font=="txt_b_3") templabel->setFont(font_txt_b_3);
                     else if (font=="txt_cal") templabel->setFont(font_calibri);
                     else if (font=="txt_cal_i") templabel->setFont(font_i_calibri);
                     else if (font=="txt_cal_b") templabel->setFont(font_b_calibri);
                     else if (font=="txt_cal_bi") templabel->setFont(font_b_i_calibri);
                     else templabel->setFont(boldFont);

                    templabel->setFrameSize(XML::getProperty(subnode, "bordersize", 0));
                    templabel->setVisible(false);
                    templabel->adjustSize();
                    add(templabel);
//                    gcn::Label *t;
//                    t =templabel;
                    mvLabel.push_back(templabel);
                }

            }
        }


     }
     slideStateControl();
}
