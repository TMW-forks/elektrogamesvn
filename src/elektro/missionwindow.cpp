#include "missionwindow.h"

#include <SDL_mouse.h>

#include <guichan/widgets/label.hpp>

#include "gui/widgets/button.h"
#include "gui/gui.h"
#include "gui/viewport.h"
#include "elektro/bitbutton.h"
#include "elektro/imagewidget.h"

#include "../game.h"
//#include "../engine.h"
//#include "../particle.h"
//#include "../map.h"

#include "utils/stringutils.h"
#include "../resources/resourcemanager.h"
#include "../graphics.h"
#include "log.h"


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
    mContainerSub->setDimension(gcn::Rectangle(0,0,250,300));
    mContainerSub->setOpaque(false);

    mContainerMain = new Container();
    mContainerMain->setDimension(gcn::Rectangle(0,0,145,380));
    mContainerMain->setOpaque(false);

    mContainerExp = new Container();
    mContainerExp->setDimension(gcn::Rectangle(0,0,145,100));
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
    mScrollExp->setDimension(gcn::Rectangle(130,210,getWidth()-140,getHeight()-250));
    mScrollExp->setOpaque(false);
    mContainerExp->addActionListener(this);
    add(mScrollExp);

    ResourceManager *resman = ResourceManager::getInstance();
    mBackgroundPattern = resman->getImage("graphics/elektrik/gorev_background.png");

//    hideSubMissions();
    setVisible(false);
}

void
MissionWindow::logic()
{
Window::logic();
}

void
MissionWindow::draw(gcn::Graphics* graphics)
{
    Window::draw(graphics);
    Graphics *g = static_cast<Graphics*>(graphics);
//    g->drawRescaledImage(mBackgroundPattern,
//                     0,0,120,10,
//                     mBackgroundPattern->getWidth(),mBackgroundPattern->getHeight(),
//                     getWidth()-130,getHeight()-30,false);
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
//    for(mSubMissionsIter = mSubMissions.begin(); mSubMissionsIter != mSubMissions.end(); mSubMissionsIter++)
//    {
//        delete (*mSubMissionsIter)->oneTarget;
//        delete (*mSubMissionsIter)->oneExplain;
//        ResourceManager *resman = ResourceManager::getInstance();
//        (*mSubMissionsIter)->oneImage->decRef();
//    }
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
            (*mSubMissionsIter)->oneExplain->setVisible(true);

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
        int dy = 20;

        SmMainMission *temp;
        temp = mMainMissionIter->second;
        TSubMissions subtemp= temp->subMissions;

        for(mSubMissionsIter = subtemp.begin(); mSubMissionsIter != subtemp.end(); mSubMissionsIter++)
        {
                (*mSubMissionsIter)->oneTarget->setPosition(x,y);
                (*mSubMissionsIter)->oneTarget->setWidth(150);
                (*mSubMissionsIter)->oneTarget->showPart(gcn::Rectangle(0,0,100,5));
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

            (*zit)->oneVisible = false;
        }
    }
}

void
MissionWindow::parse(std::string mDoc)
{
    ResourceManager *resman = ResourceManager::getInstance();
    logger->log(mDoc.c_str());

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
                if (xmlStrEqual(subnode->name, BAD_CAST "submission"))
                {
                    SmSubMission *tempSub = new SmSubMission;
                    tempSub->mainName = mainMisName;
                    TextBox *tempText = new TextBox();
                    tempText->setText(XML::getProperty(subnode, "label", "Alt görev."));
                    tempText->setWidth(100);
                    tempText->setOpaque(true);
                    tempText->setBackgroundColor(gcn::Color(156,184,184));
                    tempText->setVisible(false);
                    tempText->setActionEventId(mainMisName+tempText->getText());
                    tempText->addActionListener(this);
//                    tempText->addMouseListener(this);
//                    tempText->setFocusable(false);
                    mContainerSub->add(tempText);
                    tempSub->oneTarget = tempText;
                    tempSub->oneImage = XML::getProperty(subnode, "image", "elektrik/dugme.png");
                    tempSub->oneStatus = XML::getProperty(subnode, "status", 1)-1;
                    tempSub->oneExplain = NULL;
                    if (xmlStrEqual(subnode->name, BAD_CAST "text"))
                    {
                        BrowserBox *tt = new BrowserBox();
                        for_each_xml_child_node(supnode, subnode)
                        {
                             tt->addRow(XML::getProperty(supnode, "addrow", ""));
                             tt->setVisible(false);
                             mContainerExp->add(tt);
                         }
                         tempSub->oneExplain = tt;
                    }
                    addSubMission(mainMisName, tempSub);
                }
            }
        }
    }
    putMissionButtons();
    putSubMission();
}

void
MissionWindow::mousePressed(gcn::MouseEvent &event)
{
    logger->log("pressssssssss");
        for(TMainMissionsIter mit = mMainMission.begin(); mit != mMainMission.end(); ++mit)
        {
            SmMainMission *temp;
            temp = mit->second;
            TSubMissionsIter tik;
            gcn::Rectangle a;
            a.x += mScrollExp->getX();
            a.y += mScrollExp->getY();

            for(tik = temp->subMissions.begin(); tik != temp->subMissions.end(); tik++ )
            {
                a = (*tik)->oneTarget->getDimension();
                logger->log("XX : %s - %d %d %d %d +++ %d-%d",(*tik)->oneTarget->getText().c_str(),
                            a.x,
                            a.y,
                            a.width,
                            a.height,event.getX(), event.getY()
                             );
                if ((*tik)->oneTarget->isVisible() &&
                       a.isPointInRect(event.getX(), event.getY()))
                {
                     logger->log("yessssssssss : %s", (*tik)->oneTarget->getText().c_str());
    //                hideSubMissions();
    //                if (temp->subMissions.size() != 0)
    //                    viewOneMission(event.getId());
    //                    mScrollSub->setVerticalScrollAmount(0);
                }
            }
        }

}

//&&
//                    (*tik)->oneTarget->getDimension().isPointInRect(event.getX(), event.getY()
