#include "missionwindow.h"

#include <SDL_mouse.h>

#include <guichan/widgets/label.hpp>

#include "gui/widgets/button.h"
#include "gui/gui.h"
#include "gui/viewport.h"
#include "elektro/bitbutton.h"
#include "elektro/imagewidget.h"

#include "../game.h"

#include "utils/stringutils.h"
#include "../resources/resourcemanager.h"
#include "../graphics.h"
#include "log.h"
#include "net/ea/npchandler.h"

#include "../npc.h"

#include "utils/xml.h"
#include <libxml/xmlwriter.h>

MissionWindow::MissionWindow():
    Window("Mission")
{
    setResizable(false);
    setDefaultSize(1010-400, 754-300, 400, 300);
    loadWindowState();
    setResizable(true);

    mContainerSub = new Container();
    mContainerSub->setOpaque(false);

    mContainerMain = new Container();
    mContainerMain->setOpaque(false);

    mContainerExp = new Container();
    mContainerExp->setOpaque(false);

    mScrollSub = new ScrollArea(mContainerSub);
    mScrollSub->setDimension(gcn::Rectangle(130,50,getWidth()-140,getHeight()-180));
    mScrollSub->setOpaque(false);
    mContainerSub->addActionListener(this);
    add(mScrollSub);

    mScrollMain = new ScrollArea(mContainerMain);
    mScrollMain->setDimension(gcn::Rectangle(0,10,135,getHeight()-50));
    mScrollMain->setOpaque(false);
    mContainerMain->addActionListener(this);
    add(mScrollMain);

    mScrollExp = new ScrollArea(mContainerExp);
    mScrollExp->setDimension(gcn::Rectangle(130,215,getWidth()-140,getHeight()-250));
    mScrollExp->setOpaque(false);
    mScrollExp->setScrollPolicy(gcn::ScrollArea::SHOW_AUTO,gcn::ScrollArea::SHOW_AUTO);
    mContainerExp->addActionListener(this);
    add(mScrollExp);

    mContainerSub->setDimension(mScrollSub->getDimension());
    mContainerMain->setDimension(mScrollMain->getDimension());
    mContainerExp->setDimension(mScrollExp->getDimension());

    ResourceManager *resman = ResourceManager::getInstance();
    mBackgroundPattern = resman->getImage("graphics/elektrik/gorev_background.png");

    Button *btnClose = new Button("Kapat","close",this);
    btnClose->setPosition(getWidth()-btnClose->getWidth()-5,0);
    add(btnClose);
    susle = new ProgressBar(0,getWidth()-90, 40, gcn::Color(171, 0, 34));
    susle->setPosition(40,100);
    susle->setVisible(false);
    susle->setSmoothColorChange(true);
    susleprogress = false;
//    hideSubMissions();
    setVisible(false);
}

void
MissionWindow::logic()
{
    if (susleprogress)
    {
        susle->setProgress(susle->getProgress()+ 0.9);
        susle->setColor(gcn::Color(0, 171, 34));
    }
    if (susle->getProgress() == 1)
    {
        susle->setVisible(false);
        susleprogress = false;
        susle->setProgress(0.0f);
    }
    Window::logic();
}

void
MissionWindow::draw(gcn::Graphics* graphics)
{
    Window::draw(graphics);
    Graphics *g = static_cast<Graphics*>(graphics);
    g->drawImage(mBackgroundPattern,0,0);
    drawChildren(graphics);
}

void
MissionWindow::action(const gcn::ActionEvent &event)
{
    //ResourceManager *resman = ResourceManager::getInstance();
logger->log("action :%s",event.getId().c_str());
    if (event.getId() == "close")
    {
        setVisible(false);
        susle->setVisible(false);
    }
    for(TMainMissionsIter mit = mMainMission.begin(); mit != mMainMission.end(); ++mit)
    {
        SmMainMission *temp;
        temp = mit->second;
        if (mit->first == event.getId())
        {
            hideSubMissions();
            if (temp->subMissions.size() != 0)
                viewOneMission(event.getId());
                mScrollSub->setVerticalScrollAmount(0);
        }
    }
}

void
MissionWindow::clearMissions()
{
    for(mMainMissionIter = mMainMission.begin(); mMainMissionIter != mMainMission.end(); mMainMissionIter++)
    {
        SmMainMission *temp;
        temp = mMainMissionIter->second;
        TSubMissions subtemp= temp->subMissions;

        for(mSubMissionsIter = subtemp.begin(); mSubMissionsIter != subtemp.end(); mSubMissionsIter++)
        {
            if ((*mSubMissionsIter)->oneTarget != NULL)
                delete (*mSubMissionsIter)->oneTarget;
            if ((*mSubMissionsIter)->oneImage != NULL)
                delete (*mSubMissionsIter)->oneImage;
            if ((*mSubMissionsIter)->oneExplain != NULL)
                delete (*mSubMissionsIter)->oneExplain;
        }
        subtemp.clear();
    }
    mMainMission.clear();
}

void
MissionWindow::addSubMission(std::string mainname, SmSubMission *one)
{
    logger->log("addSubMission: %s", mainname.c_str());
    TSubMissions *temp;
    SmMainMission *mn = mMainMission[mainname];
    mn->subMissions.push_back(one);
}

void
MissionWindow::addMainMission(std::string one)
{
    logger->log("addMainMission : %s",one.c_str());
    Button *tmp = new Button(one,one,this);
    mContainerMain->add(tmp);
    SmMainMission *smtmp = new SmMainMission;
    smtmp->mainButton = tmp;
    mMainMission[one] = smtmp;
}

void
MissionWindow::viewOneMission(std::string one)
{
    logger->log("viewOneMission");
    SmMainMission *temp;
    temp = mMainMission[one];
    TSubMissions subtemp= temp->subMissions;
    if(subtemp.size() == 0) return;
    for(mSubMissionsIter = subtemp.begin(); mSubMissionsIter != subtemp.end(); mSubMissionsIter++)
    {
        if((*mSubMissionsIter)->oneTarget)
            (*mSubMissionsIter)->oneTarget->setVisible(true);

        if((*mSubMissionsIter)->oneExplain)
            (*mSubMissionsIter)->oneExplain->setVisible(false);

        if((*mSubMissionsIter)->oneImage)
            (*mSubMissionsIter)->oneImage->setVisible(true);

        if((*mSubMissionsIter)->oneVisible)
            (*mSubMissionsIter)->oneVisible = true;
    }
}

void
MissionWindow::putMissionButtons()
{
logger->log(__FUNCTION__);
    int x = 5;
    int y = 10;
    int dy =27;
    for(mMainMissionIter = mMainMission.begin(); mMainMissionIter != mMainMission.end(); mMainMissionIter++)
    {
        SmMainMission *temp;
        temp = mMainMissionIter->second;
        Button *subtemp = temp->mainButton;
        subtemp->setPosition(x,y);
        subtemp->setWidth(110);
        y += dy;
    }
}

void
MissionWindow::drawTree()
{

}

void
MissionWindow::putSubMission()
{
    logger->log(__FUNCTION__);

    for(mMainMissionIter = mMainMission.begin(); mMainMissionIter != mMainMission.end(); mMainMissionIter++)
    {
        int x  = 5;
        int y  = 10;
        int dy = 25;

        SmMainMission *temp;
        temp = mMainMissionIter->second;
        TSubMissions subtemp= temp->subMissions;

        for(mSubMissionsIter = subtemp.begin(); mSubMissionsIter != subtemp.end(); mSubMissionsIter++)
        {
            if ((*mSubMissionsIter)->oneTarget != NULL)
            {
                (*mSubMissionsIter)->oneTarget->setPosition(x,y);
                (*mSubMissionsIter)->oneTarget->setWidth(170);
            }
            if ((*mSubMissionsIter)->oneImage != NULL)
            {
                (*mSubMissionsIter)->oneImage->setX(x + 180);
                (*mSubMissionsIter)->oneImage->setY(y);
            }
            y += dy;
        }
    }
}
void
MissionWindow::hideSubMissions()
{
    logger->log(__FUNCTION__);
    TMainMissionsIter mit;
    for(mit = mMainMission.begin(); mit != mMainMission.end(); mit++)
    {
        SmMainMission *temp;
        temp = mit->second;
        if (temp ==NULL) continue;
        TSubMissions subtemp= temp->subMissions;
        TSubMissionsIter zit;
        for(zit = subtemp.begin(); zit < subtemp.end(); zit++)
        {
            if((*zit)->oneTarget)
                (*zit)->oneTarget->setVisible(false);

            if((*zit)->oneExplain)
                (*zit)->oneExplain->setVisible(false);

            if((*zit)->oneImage)
                (*zit)->oneImage->setVisible(false);

            (*zit)->oneVisible = false;
        }
    }
}

void
MissionWindow::parse(std::string mDoc)
{
    susle->setProgress(0.0f);
    susleprogress = true;
    susle->setVisible(true);
    add(susle);
    hideSubMissions();
    xmlDocPtr mxmlDoc;
    mxmlDoc=xmlParseMemory(mDoc.c_str(),mDoc.size());
    if (!mxmlDoc)
    {
        localChatTab->chatLog("Bu üstad'ın morali bozuk :(", BY_SERVER);
        localChatTab->chatLog("Bu durumu bir yöneticiye haber versen çok iyi olur.", BY_SERVER);
        return;
    }
    xmlNodePtr rootNode = xmlDocGetRootElement(mxmlDoc);
    if (!rootNode || !xmlStrEqual(rootNode->name, BAD_CAST "mission"))
    {
        localChatTab->chatLog("Bu üstad'ın morali bozuk :(", BY_SERVER);
        localChatTab->chatLog("Bu durumu bir yöneticiye haber versen çok iyi olur.", BY_SERVER);
        return;
    }

    for_each_xml_child_node(node, rootNode)
    {
//         Ana görev
        if (xmlStrEqual(node->name, BAD_CAST "mainmission"))
        {
            std::string mainMisName = XML::getProperty(node, "name", "Ana Görev");
            addMainMission(mainMisName);
            for_each_xml_child_node(subnode, node)
            {
                susle->setProgress(susle->getProgress()*1.1f);

                if (xmlStrEqual(subnode->name, BAD_CAST "submission"))
                {
                    SmSubMission *tempSub = new SmSubMission;
                    tempSub->mainName = mainMisName;
                    tempSub->oneVisible = false;
                    TextBox *tempText = new TextBox();
                    tempText->setText(XML::getProperty(subnode, "label", "Alt görev."));
                    tempText->setWidth(100);
                    tempText->setOpaque(true);
                    tempText->setBackgroundColor(gcn::Color(156,184,184));
                    tempText->setVisible(false);
                    tempText->setActionEventId(mainMisName+tempText->getText());
                    tempText->addActionListener(this);
                    tempText->addMouseListener(this);
                    tempText->setFrameSize(1);
                    tempText->setId(tempText->getText());
                    mContainerSub->add(tempText);
                    tempSub->oneTarget = tempText;
                    std::string imagestr = XML::getProperty(subnode, "image", "elektrik/dugme.png");
                    ImageWidget *tempImage;
                    if (imagestr != "")
                    {
                        tempImage = new ImageWidget(imagestr);
                        tempImage->setVisible(false);
                        tempImage->setMode(XML::getProperty(subnode, "status", 0));
                    }
                    else
                        tempImage = NULL;

                    mContainerSub->add(tempImage);
                    tempSub->oneImage = tempImage;
                    tempSub->oneStatus = XML::getProperty(subnode, "status", 1)-1;
                    tempSub->oneExplain = NULL;
                    for_each_xml_child_node(textnode, subnode)
                    {
                        if (xmlStrEqual(textnode->name, BAD_CAST "text"))
                        {
                            BrowserBox *tt = new BrowserBox();
                            tt->setVisible(false);
                            tt->setOpaque(false);
                            tt->addMouseListener(this);
                            mContainerExp->add(tt);
                            tempSub->oneExplain = tt;
                            tt->setDimension(gcn::Rectangle(0,0,mContainerExp->getWidth(),mContainerExp->getHeight()));

                            for_each_xml_child_node(rownode, textnode)
                            {
                                 if (xmlStrEqual(rownode->name, BAD_CAST "addrow"))
                                 {
                                     tt->addRow(XML::getProperty(rownode, "text", ""));
                                 }
                             }
                        }
                    }
                    addSubMission(mainMisName, tempSub);
                }
            }
        }
    }
    putMissionButtons();
    putSubMission();
    hideSubMissions();
    current_npc = 0;
    NPC::isTalking = false;
}

void
MissionWindow::mousePressed(gcn::MouseEvent &event)
{
    Widget *w= event.getSource();
    const std::string ClickedId = event.getSource()->getId();
    for(TMainMissionsIter mit = mMainMission.begin(); mit != mMainMission.end(); ++mit)
    {
        SmMainMission *temp;
        temp = mit->second;
        TSubMissionsIter tik;
        for(tik = temp->subMissions.begin(); tik != temp->subMissions.end(); tik++ )
        {
            logger->log("%s == %s",(*tik)->oneTarget->getId().c_str() , ClickedId.c_str());
            if ((*tik)->oneTarget->isVisible() &&
                   (*tik)->oneTarget->getId() == ClickedId)
            {
                if((*tik)->oneExplain != NULL)
                {
                    (*tik)->oneExplain->setVisible(true);
                    mContainerExp->setHeight((*tik)->oneExplain->getHeight());
                    mScrollExp->setVerticalScrollAmount(0);
                }
            }
            else if(ClickedId != "")
            {
                if((*tik)->oneExplain != NULL)
                    (*tik)->oneExplain->setVisible(false);
            }
        }
    }
    Window::mousePressed(event);
}
