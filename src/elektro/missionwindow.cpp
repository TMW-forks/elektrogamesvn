#include "missionwindow.h"

#include <SDL_mouse.h>

#include <guichan/widgets/label.hpp>

#include "gui/widgets/button.h"
#include "gui/gui.h"
#include "gui/viewport.h"
#include "elektro/bitbutton.h"

#include "../game.h"
//#include "../engine.h"
//#include "../particle.h"
//#include "../map.h"

#include "utils/stringutils.h"
#include "../resources/resourcemanager.h"
#include "../graphics.h"
#include "log.h"

MissionWindow::MissionWindow():
    Window("Mission")
{
    setResizable(false);
    setDefaultSize(1010-400, 754-300, 400, 300);
    loadWindowState();
    setResizable(true);
    ResourceManager *resman = ResourceManager::getInstance();
    sayfaImg = resman->getImage("graphics/images/help1.png");
    sayfa=1;

    mBackgroundPattern = resman->getImage("graphics/elektrik/backgroundpattern.png");
    setVisible(true);
    addMainMission("Eğitim Odası");
    addMainMission("Devre Tamamlama");
    addMainMission("Akım bulma");
    addMainMission("Gerilim bulma");
    putMissionButtons();

    TextBox *tl = new TextBox();
    tl->setText("Test npcleri eğitimi");
    tl->setWidth(100);
    tl->setOpaque(true);
    tl->setBackgroundColor(gcn::Color(156,184,184));
    tl->setVisible(false);
    add(tl);

    TextBox *tl1 = new TextBox();
    tl1->setText("Robot dönüştürme eğitimi noexp");
    tl1->setWidth(100);
    tl1->setOpaque(true);
    tl1->setBackgroundColor(gcn::Color(156,184,184));
    tl1->setVisible(false);
    add(tl1);

    TextBox *tl2 = new TextBox();
    tl2->setText("Kablo bağlama");
    tl2->setWidth(100);
    tl2->setOpaque(true);
    tl2->setBackgroundColor(gcn::Color(156,184,184));
    tl2->setVisible(false);
    add(tl2);

    TextBox *tl3 = new TextBox();
    tl3->setWidth(100);
    tl3->setText("Item yerleştirme");
    tl3->setOpaque(true);
    tl3->setBackgroundColor(gcn::Color(156,184,184));
    tl3->setVisible(false);
    add(tl3);

    BrowserBox *tt = new BrowserBox();
    tt->addRow("soranCan'a git");
    tt->addRow("onu yap ");
    tt->addRow("bunu yap");
    tt->addRow("falan filan");
    tt->setDimension(gcn::Rectangle(120,150,100,5));
    tt->setVisible(false);
    add(tt);

    SmSubMission *ts = new SmSubMission;
    ts->oneTarget =tl;
    ts->oneExplain = tt;
    addSubMission("Eğitim Odası", ts);

    SmSubMission *ts1 = new SmSubMission;
    ts1->oneTarget =tl1;
    ts1->oneExplain = NULL;
    addSubMission("Eğitim Odası", ts1);

    SmSubMission *ts2 = new SmSubMission;
    ts2->oneTarget =tl2;
    ts2->oneExplain = tt;
    addSubMission("Eğitim Odası", ts2);

    SmSubMission *tss = new SmSubMission;
    tss->oneTarget =tl3;
    tss->oneExplain = tt;
    addSubMission("Devre Tamamlama", tss);

hideSubMissions();
//viewOneMission("Eğitim Odası");
    putSubMission();
}

void
MissionWindow::logic()
{

}

void
MissionWindow::draw(gcn::Graphics* graphics)
{
    Window::draw(graphics);
    Graphics *g = static_cast<Graphics*>(graphics);
    g->drawRescaledImage(mBackgroundPattern,
                     0,0,120,10,
                     mBackgroundPattern->getWidth(),mBackgroundPattern->getHeight(),
                     getWidth()-130,getHeight()-25,false);
    drawChildren(graphics);
}
void
MissionWindow::action(const gcn::ActionEvent &event)
{
    //ResourceManager *resman = ResourceManager::getInstance();
logger->log("action");
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
    logger->log("addSubMission");
    TSubMissions *temp;
    SmMainMission *mn = mMainMission[mainname];
    mn->subMissions.push_back(one);
}

void
MissionWindow::addMainMission(std::string one)
{
    logger->log("addMainMission");
    Button *tmp = new Button(one,one,this);
    add(tmp);
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
//    if(subtemp.size() == 0) return;
    for(mSubMissionsIter = subtemp.begin(); mSubMissionsIter != subtemp.end(); mSubMissionsIter++)
    {
        if((*mSubMissionsIter)->oneTarget)
            (*mSubMissionsIter)->oneTarget->setVisible(true);
//
//        if((*mSubMissionsIter)->oneExplain)
//            (*mSubMissionsIter)->oneExplain->setVisible(true);

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
                (*mSubMissionsIter)->oneTarget->setPosition(125,y);
                (*mSubMissionsIter)->oneTarget->setWidth(150);
            y += dy;
        }
    }
}
void
MissionWindow::hideSubMissions()
{
    logger->log(__FUNCTION__);

    for(mMainMissionIter = mMainMission.begin(); mMainMissionIter != mMainMission.end(); mMainMissionIter++)
    {
        SmMainMission *temp;
        temp = mMainMissionIter->second;
        if (temp ==NULL) continue;
        TSubMissions subtemp= temp->subMissions;
        for(mSubMissionsIter = subtemp.begin(); mSubMissionsIter < subtemp.end(); mSubMissionsIter++)
        {
            if((*mSubMissionsIter)->oneTarget)
                (*mSubMissionsIter)->oneTarget->setVisible(false);

            if((*mSubMissionsIter)->oneExplain)
                (*mSubMissionsIter)->oneExplain->setVisible(false);

            (*mSubMissionsIter)->oneVisible = false;
        }
    }
}

