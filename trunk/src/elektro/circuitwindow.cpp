#include "circuitwindow.h"

#include <SDL_mouse.h>
#include <SDL_draw.h>
#include <guichan/widgets/label.hpp>
#include <math.h>
#include "gui/widgets/button.h"
#include "bitbutton.h"
#include "elektrowidget.h"
#include "gui/gui.h"
#include "gui/viewport.h"

#include "../graphics.h"
#include "../game.h"
#include "../engine.h"
#include "../particle.h"
#include "../map.h"
#include "../log.h"
#include "../item.h"
#include "../gui/chat.h"
#include "resources/itemdb.h"
#include "gui/widgets/chattab.h"

#include "utils/stringutils.h"
#include "../utils/xml.h"
#include <libxml/xmlwriter.h>

#include "net/ea/npchandler.h"

#include "../npc.h"
#include "gsl/gsl_linalg.h"
#include "gsl/gsl_vector.h"
#include "gsl/gsl_math.h"

#include "../localplayer.h"
#include "../sound.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"

#include "net/inventoryhandler.h"
#include "net/net.h"

#include "resources/image.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

extern int current_npc;
extern std::string globalHint;
extern ElektroWidget *elektroWidget;

class Resistance;
class inventoryHandler;

CircuitWindow::CircuitWindow():
    Window("Devre Penceresi"),
    collisionCheck(false),
    nodeCollision(false)
{
    setWindowName("Devreci");
    setMinWidth(300);
    setMinHeight(400);
    setResizable(false);
    setVisible(false);
    setDefaultSize(150, 150, 520, 450);

    ResourceManager *resman = ResourceManager::getInstance();

    nodeConnectImage = resman->getImage("graphics/elektrik/node_connect.png");
    cirToolBar = resman->getImage("graphics/elektrik/cir_toolbar.png");
    cirRotate = resman->getImage("graphics/elektrik/cir_rot.png");
    cirMove = resman->getImage("graphics/elektrik/cir_mov.png");
    cirFromRight = resman->getImage("graphics/elektrik/cir_from_right.png");
    cirFromLeft = resman->getImage("graphics/elektrik/cir_from_left.png");
    cirToRight = resman->getImage("graphics/elektrik/cir_to_right.png");
    cirToLeft = resman->getImage("graphics/elektrik/cir_to_left.png");
    cirErase = resman->getImage("graphics/elektrik/cir_erase.png");
    cirSelect = resman->getImage("graphics/elektrik/cir_sel.png");
    cirRotateG = resman->getImage("graphics/elektrik/cir_rot_g.png");
    cirMoveG = resman->getImage("graphics/elektrik/cir_mov_g.png");
    cirFromRightG = resman->getImage("graphics/elektrik/cir_from_right_g.png");
    cirFromLeftG = resman->getImage("graphics/elektrik/cir_from_left_g.png");
    cirToRightG = resman->getImage("graphics/elektrik/cir_to_right_g.png");
    cirToLeftG = resman->getImage("graphics/elektrik/cir_to_left_g.png");
    cirEraseG = resman->getImage("graphics/elektrik/cir_erase_g.png");
    cirSelectG = resman->getImage("graphics/elektrik/cir_sel_g.png");
    mWireImage = resman->getImage("graphics/elektrik/kablo.png");
    mUnControl = resman->getImage("graphics/elektrik/kontrolsuz.png");
    mBackgroundPattern = resman->getImage("graphics/elektrik/backgroundpattern.png");


    std::vector<std::string> imagesetname;
    std::vector<std::string>::iterator iname;

    imagesetname.push_back("graphics/elektrik/item-ampul-0-b.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-0-c.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-0-d.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-0-e.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-0.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-1.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-10.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-10b.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-10c.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-10d.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-10e.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-10f.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-1b.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-1c.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-1d.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-1e.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-1f.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-2.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-20.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-20b.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-20c.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-20d.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-20e.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-20f.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-2b.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-2c.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-2d.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-2e.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-2f.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-3.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-3b.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-3c.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-3d.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-3e.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-3f.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-5.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-50.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-50b.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-50c.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-50d.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-50e.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-50f.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-5b.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-5c.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-5d.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-5e.png");
    imagesetname.push_back("graphics/elektrik/item-ampul-5f.png");
    imagesetname.push_back("graphics/elektrik/item-anahtar-b.png");
    imagesetname.push_back("graphics/elektrik/item-anahtar-c.png");
    imagesetname.push_back("graphics/elektrik/item-anahtar.png");
    imagesetname.push_back("graphics/elektrik/item-bled-b.png");
    imagesetname.push_back("graphics/elektrik/item-bled-c.png");
    imagesetname.push_back("graphics/elektrik/item-bled.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-1-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-1.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-10-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-10.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-11-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-11.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-12-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-12.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-13-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-13.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-14-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-14.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-15-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-15.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-2-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-2.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-20-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-20.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-25-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-25.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-3-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-3.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-30-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-30.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-4-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-4.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-5-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-5.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-50-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-50.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-6-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-6.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-7-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-7.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-8-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-8.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-9-b.png");
    imagesetname.push_back("graphics/elektrik/item-direnc-9.png");
    imagesetname.push_back("graphics/elektrik/item-kled-b.png");
    imagesetname.push_back("graphics/elektrik/item-kled-c.png");
    imagesetname.push_back("graphics/elektrik/item-kled.png");
    imagesetname.push_back("graphics/elektrik/item-pil.png");
    imagesetname.push_back("graphics/elektrik/item-pil-b.png");
    imagesetname.push_back("graphics/elektrik/item-pil-1v.png");
    imagesetname.push_back("graphics/elektrik/item-pil-2v.png");
    imagesetname.push_back("graphics/elektrik/item-pil-3v.png");
    imagesetname.push_back("graphics/elektrik/item-pil-4v.png");
    imagesetname.push_back("graphics/elektrik/item-pil-5v.png");
    imagesetname.push_back("graphics/elektrik/item-pil-10v.png");
    imagesetname.push_back("graphics/elektrik/item-pil-20v.png");
    imagesetname.push_back("graphics/elektrik/item-sled-a.png");
    imagesetname.push_back("graphics/elektrik/item-sled-b.png");
    imagesetname.push_back("graphics/elektrik/item-sled.png");
    imagesetname.push_back("graphics/elektrik/item-yled-b.png");
    imagesetname.push_back("graphics/elektrik/item-yled-c.png");
    imagesetname.push_back("graphics/elektrik/item-yled.png");
    imagesetname.push_back("graphics/elektrik/item-motor-1.png");
    imagesetname.push_back("graphics/elektrik/item-motor-2.png");
    imagesetname.push_back("graphics/elektrik/item-motor-3.png");
    imagesetname.push_back("graphics/elektrik/item-ampermetre.png");
    imagesetname.push_back("graphics/elektrik/hale-beyaz.png");
    imagesetname.push_back("graphics/elektrik/hale-kirmizi.png");
    imagesetname.push_back("graphics/elektrik/hale-sari.png");
    imagesetname.push_back("graphics/elektrik/hale-mavi.png");
    imagesetname.push_back("graphics/elektrik/hale-pembe.png");
    imagesetname.push_back("graphics/elektrik/hale-yesil.png");
    imagesetname.push_back("graphics/elektrik/hale-turuncu.png");

    mHale.insert(std::make_pair(1,"graphics/elektrik/hale-beyaz.png" ));
    mHale.insert(std::make_pair(2,"graphics/elektrik/hale-sari.png" ));
    mHale.insert(std::make_pair(3,"graphics/elektrik/hale-pembe.png" ));
    mHale.insert(std::make_pair(5,"graphics/elektrik/hale-mavi.png" ));
    mHale.insert(std::make_pair(10,"graphics/elektrik/hale-yesil.png" ));
    mHale.insert(std::make_pair(20,"graphics/elektrik/hale-kirmizi.png" ));
    mHale.insert(std::make_pair(50,"graphics/elektrik/hale-turuncu.png" ));

    for(iname = imagesetname.begin(); iname != imagesetname.end(); iname++)
    {
        ImageSet *compoImageSet;
        if ((*iname).find("hale") == std::string::npos)
            compoImageSet = resman->getImageSet((*iname),32,32);
        else
            compoImageSet = resman->getImageSet((*iname),64,64);
        mComponentImageSet[(*iname)] = compoImageSet;
    }

    toolRotate = false;
    toolMove = false;
    toolFromRight = false;
    toolFromLeft = false;
    toolToRight = false;
    toolToLeft = false;
    toolErase = false;
    toolSelect = false;
    mNodeCreate = true;
    mRefresh = false;

    toolCaption = new gcn::Label("");
    toolCaption->setPosition(100,getHeight()-200);
    toolCaption->setFont(font_bas_3_14);
    toolCaption->setForegroundColor(gcn::Color(56,84,184));
    add(toolCaption);

    toolValue = new gcn::Label();
    toolValue->setPosition(170,getHeight()-130);
    toolValue->setFont(font_bas_b_2_14);
    toolValue->setForegroundColor(gcn::Color(56,84,184));
    add(toolValue);

    mPopupLabel = new TextBox();
    mPopupLabel->setFont(font_bas_3_12);
    mPopupLabel->setForegroundColor(gcn::Color(56,84,184));
    mPopupLabel->setBaseColor(gcn::Color(156,184,184));
    mPopupLabel->setBackgroundColor(gcn::Color(156,184,184));
    mPopupLabel->setVisible(false);
    mPopupLabel->setFrameSize(1);
    mPopupLabel->setOpaque(true);
    add(mPopupLabel);

    closeButton = new BitButton("btn_degerlendir.png", "Degerlendir", "evaluate",this);
    closeButton->setPosition(10,120);
    add(closeButton);



    mHint = new gcn::Label("");
    mHint->setPosition(10,10);
    mHint->adjustSize();
    add(mHint);

//******
    solveButton = new BitButton("com_close_btn.png", "Degerlendir", "solve",this);
    solveButton->setPosition(10,140);

    clearButton = new BitButton("com_rotate_btn.png", "Degerlendir", "clear",this);
    clearButton->setPosition(10,100);

    mSb = new BrowserBox();
    mSb->setOpaque(true);


    mSs = new ScrollArea(mSb);
    mSs->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);
    mSs->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);
    mSs->setVisible(true);
    mSs->setWidth(350);
    mSs->setHeight(100);
    mSs->setY(getHeight()- mSs->getHeight());
    mSs->setX(getWidth()-mSs->getWidth());

    mX = new gcn::Label("X:");
    mX->setPosition(4,5);

    mY = new gcn::Label("Y:");
    mY->setPosition(4,15);

//    add(mX);
//    add(mSs);
//    add(clearButton);
//    add(mY);
//    add(solveButton);

//***********/



    globalHint="hint";
    setLocationRelativeTo(getParent());

    mMessageText = new BrowserBox();
    mMessageText->setOpaque(false);
    mMessageText->setVisible(false);

    mMessageScroll = new ScrollArea(mMessageText);
    mMessageScroll->setOpaque(false);
    mMessageScroll->setDimension(gcn::Rectangle(150,30, getWidth()-20, getHeight()-130));
    mMessageScroll->setVisible(false);
    add(mMessageScroll);

    mStartOk = new Button("Hemen Başla","startok",this);
    mStartCancel = new Button("Yok Almayım","startcancel",this);
    mFootOk = new Button("Tamam","FootOk",this);
    mWaitButton = new Button("İlerle","WaitOk",this);

    mStartCancel->setWidth(mStartOk->getWidth());
    mStartCancel->setHeight(mStartOk->getHeight());
    mFootOk->setWidth(150);
    mWaitButton->setWidth(150);

    mStartOk->setVisible(false);
    mStartCancel->setVisible(false);
    mFootOk->setVisible(false);
    mWaitButton->setVisible(false);

    add(mStartOk);
    add(mStartCancel);
    add(mFootOk);
    add(mWaitButton);
}

CircuitWindow::~CircuitWindow()
{
    delete mHint;
    delete toolCaption;
    delete mX;
    delete mY;
    delete mMessageText;
    delete mMessageScroll;
    deleteWidgets();

}

void
CircuitWindow::stateCheck()
{
    switch(mCircState)
    {
        case HEAD_MESSAGE_STATE:
            deleteWidgets();
            trashMeshMem();
            mMessageScroll->setVisible(true);
            mMessageScroll->setDimension(gcn::Rectangle(150,30, getWidth()-180, getHeight()-160));
            mMessageScroll->setOpaque(true);
            mMessageText->setVisible(true);
            mStartOk->setVisible(true);
            mStartCancel->setVisible(true);
            closeButton->setEnabled(false);
            int x = (getWidth()-mStartOk->getWidth()-mStartCancel->getWidth())/2 + 50;
            mStartOk->setPosition(x,getHeight()-120);
            x += mStartOk->getWidth() + 10 ;
            mStartCancel->setPosition(x,getHeight()-120);
            closeButton->setVisible(false);
            mFootOk->setVisible(false);
            mWaitButton->setVisible(false);
            if (mMessageAutoWrap)
                mMessageText->autoWrap(mMessageScroll);
            break;
        case FOOT_MESSAGE_STATE:
            deleteWidgets();
            trashMeshMem();
            mMessageScroll->setVisible(true);
            mMessageScroll->setDimension(gcn::Rectangle(150,30, getWidth()-180, getHeight()-160));
            mMessageScroll->setOpaque(true);
            mMessageText->setVisible(true);
            mFootOk->setVisible(true);
            mFootOk->setX( mMessageScroll->getX()+mMessageScroll->getWidth()/2- mFootOk->getWidth()/2);
            mFootOk->setY(mMessageScroll->getY()+mMessageScroll->getHeight()+20);
            closeButton->setEnabled(false);
            if (mMessageAutoWrap)
                mMessageText->autoWrap(mMessageScroll);
            mWaitButton->setVisible(false);

            break;
        case CIRCUIT_STATE:
            closeButton->setEnabled(true);
            mFootOk->setVisible(false);
            mMessageScroll->setVisible(false);
            mStartOk->setVisible(false);
            mStartCancel->setVisible(false);
            closeButton->setVisible(true);
            mWaitButton->setVisible(false);
            mRefresh = true;
            break;
        case TEST_STATE:
            closeButton->setEnabled(true);
            mMessageScroll->setVisible(false);
            mStartOk->setVisible(false);
            mStartCancel->setVisible(false);
            closeButton->setVisible(false);
            mFootOk->setVisible(false);
            mWaitButton->setVisible(false);
            break;
        case WAIT_STATE:
            closeButton->setEnabled(false);
            mFootOk->setVisible(false);
            mMessageScroll->setVisible(false);
            mStartOk->setVisible(false);
            mStartCancel->setVisible(false);
            closeButton->setVisible(false);
            mWaitButton->setPosition((getWidth()-mWaitButton->getWidth()) / 2 , getHeight()-150);
            mWaitButton->setVisible(true);
            mRefresh = true;
            break;
    }
}
void
CircuitWindow::logic()
{
    //silinen component'leri temizle
    std::vector<Component*>::iterator i = mvComponent.begin();
    while (i != mvComponent.end())
    {
        Component *compo = (*i);

        if (compo->mDead)
        {
            compo->node1->setDead(true);
            compo->node2->setDead(true);
            compo->node1=NULL;
            compo->node2=NULL;
            compo->mDead = false;
            delete compo;
            i = mvComponent.erase(i);
            mRefresh = true;
        }
        else {
            i++;
        }
    }

    //silinen nodları temizle
    std::vector<Node*>::iterator j = mvNode.begin();
    while (j != mvNode.end())
    {
        Node *nod = (*j);

        if (nod->getDead())
        {
            clearNodeConnection(nod);
            nod->setDead(false);
            delete nod;
            j = mvNode.erase(j);
            mRefresh = true;
            circuitWindow->setNodeCreate(true);
        }
        else {
            j++;
        }
    }


    if (collisionCheck && isVisible())
    {
        int mx=0, my=0;
        bool found=false;
        j = mvNode.begin();
        while (j != mvNode.end())
        {
            Node *nod = (*j);
            if (nod->getMoving())
            {
                mx = nod->getX();
                my = nod->getY();
                movingNode = nod;
                found = true;
                break;
            }
            j++;
        }
        if (found)
        {
            j = mvNode.begin();
            while (j != mvNode.end())
            {
                Node *nod = (*j);
                if (nod != movingNode)
                {
                    targetNode = nod;
                    int tx = nod->getX();
                    int ty = nod->getY();
                    if (mx > tx-10 && mx < tx+10  &&
                        my > ty-10 && my < ty+10 &&
                        nod->getFromLink())
                    {
                        collisionNodeX = tx;
                        collisionNodeY = ty;
                        nodeCollision = true;
                        break;
                    }
                    else
                        nodeCollision = false;
                }
                    j++;
            }
        }
    }
//    for(miBrowserBox = mvBrowserBox.begin();
//            miBrowserBox != mvBrowserBox.end();
//            miBrowserBox++)
//    {
//        (*miBrowserBox)->logic();
//    }
/****************************************************************/


//aradaki kabloları çiz
    if ((mWireRefresh || mRefresh) && isVisible())
    {
        mWireRefresh = false;
         for(mTellerIter = mTeller.begin(); mTellerIter != mTeller.end(); mTellerIter++)
         {
            for(mTekTelIter = (*mTellerIter).begin(); mTekTelIter != (*mTellerIter).end(); mTekTelIter++)
            {
                delete (*mTekTelIter);
            }
         }
        mTeller.clear();
        for (conListIter=conList.begin();conListIter<conList.end();conListIter ++)
        {
            if ((*conListIter)->draw)
            {
                int x1,y1,x2,y2;
                (*conListIter)->firstCon->getAbsolutePosition(x1,y1);
                (*conListIter)->secondCon->getAbsolutePosition(x2,y2);
                x1 -= getX();
                x2 -= getX();
                y1 -= getY();
                y2 -= getY();
                drawLine(graphics,x1+4,
                                  y1+4,
                                  x2+4,
                                  y2+4,1);
            }
        }
    }
    if (mRefresh && isVisible())
    {
        mRefresh=false;
        devreAnaliz();
    }
    Window::logic();
}

void
CircuitWindow::draw(gcn::Graphics *graphics)
{
Window::draw(graphics);
//    graphics->drawRectangle(gcn::Rectangle(20,20,getWidth()-40,getHeight()-40));
    Graphics *g = static_cast<Graphics*>(graphics);
    ResourceManager *resman = ResourceManager::getInstance();

    g->drawRescaledImage(mBackgroundPattern,
                         0,0,120,20,
                         mBackgroundPattern->getWidth(),mBackgroundPattern->getHeight(),
                         getWidth()-130,getHeight()-35,false);

    g->drawImage(cirToolBar,10,(getHeight()-cirToolBar->getHeight())/2);

     //kabloları çizdir
     for(mTellerIter = mTeller.begin(); mTellerIter != mTeller.end(); mTellerIter++)
     {
//        for(mTekTelIter = (*mTellerIter).begin(); mTekTelIter != (*mTellerIter).end(); mTekTelIter++)
        for(int z = 0;  z<(*mTellerIter).size(); z += 1)
        {
            mTekTelIter = (*mTellerIter).begin()+z;
            g->drawImage(mWireImage,
                         (*mTekTelIter)->x -3 ,
                          (*mTekTelIter)->y -3);
        }
     }


//    for(conLocateIter = conLocate.begin(); conLocateIter!= conLocate.end(); conLocateIter++)
//    {
//          graphics->setColor(gcn::Color(0x00ff00));
//            graphics->drawRectangle((*conLocateIter)->area);
//    }

    if (nodeCollision)
      g->drawImage(nodeConnectImage,collisionNodeX,collisionNodeY);

    int top = (getHeight()-cirToolBar->getHeight())/2 + 43;
    int x = 48;
    int dy = 30;

    if (toolFromRight)
        g->drawImage(cirFromRight, x, top);
    else
        g->drawImage(cirFromRightG, x, top);

    top  += dy;
    if (toolFromLeft)
        g->drawImage(cirFromLeft, x, top);
    else
        g->drawImage(cirFromLeftG, x, top);

    top  += dy+ 3;
    if (toolToRight)
        g->drawImage(cirToRight, x, top );
    else
        g->drawImage(cirToRightG, x, top );

    top  += dy+ 2;
    if (toolToLeft)
        g->drawImage(cirToLeft, x, top );
    else
        g->drawImage(cirToLeftG, x, top);

    top  += dy;
    if (toolErase)
        g->drawImage(cirErase, x, top);
    else
        g->drawImage(cirEraseG, x, top);

    top  += dy;
    if (toolSelect)
        g->drawImage(cirSelect, x, top);
    else
        g->drawImage(cirSelectG, x, top);

    top  += dy;
    if (toolRotate)
        g->drawImage(cirRotate, x, top);
    else
        g->drawImage(cirRotateG, x, top);

    top  += dy+2;
    if (toolMove)
        g->drawImage(cirMove,x , top );
    else
        g->drawImage(cirMoveG,x , top );

#ifdef DEBUG
    for(miComponent=mvComponent.begin(); miComponent<mvComponent.end(); miComponent++)
    {
        if ((*miComponent)->getSelected())
            {
                mX->setCaption("x:"+toString((*miComponent)->getX()));
                mY->setCaption("y:"+toString((*miComponent)->getY()));
                mX->adjustSize();
                mY->adjustSize();
            }
    }
#endif

    for (miImage=mvImage.begin();miImage!=mvImage.end();++miImage)
    {
         if (miImage->visible)
            g->drawImage(miImage->img,miImage->x,miImage->y);
    }

     for (miAnim=mvAnim.begin();miAnim!=mvAnim.end();++miAnim)
     {
        if (miAnim->visible)
        {
            Image *mImage1=miAnim->anim->getCurrentImage();
            g->drawImage(mImage1,miAnim->x,miAnim->y);
            miAnim->anim->update(miAnim->v);
        }
     }

//     g->setColor(gcn::Color(10,100,200));
//     if(mPopupLabel->isVisible())
//        graphics->fillRectangle(gcn::Rectangle(mPopupLabel->getX()+3,
//                                               mPopupLabel->getY()+20,
//                                               mPopupLabel->getWidth(),
//                                               mPopupLabel->getHeight()));

// Kontrolü bilgede olan düğmelerin altına işaret ekle
    for (conSwitchIter = conSwitch.begin(); conSwitchIter != conSwitch.end(); conSwitchIter++ )
    {
        Component *tmpcmp = findComponent((*conSwitchIter)->compId);
        if(tmpcmp)
        {
            g->drawImage(mUnControl, tmpcmp->getX() - 10, tmpcmp->getY() + 10);
        }
    }


    //pencere içindeki diğer widget'leri çizdir
    drawChildren(graphics);

     //lambaların ışıklarını üzerine bas
    for(miComponent=mvComponent.begin(); miComponent<mvComponent.end(); miComponent++)
    {
        if (isLamp((*miComponent)) &&
            (*miComponent)->getStatus() != PASIVE &&
            (*miComponent)->getStatus() !=BURNED)
        {
            logger->log("PARILTI : %s",mHale[(*miComponent)->getValue()].c_str());
            ImageSet *res = circuitWindow->mComponentImageSet[mHale[(*miComponent)->getValue()]];
            g->drawImage(res->get((*miComponent)->getParilti()), (*miComponent)->getX()-8, (*miComponent)->getY()+5);
        }
    }
 }

void
CircuitWindow:: clearNodeConnection(Node *n)
{
    conListIter=conList.begin();
    while(conListIter != conList.end())
    {
        if (((*conListIter)->firstCon == n ||
            (*conListIter)->secondCon == n))
        {
            if (((*conListIter)->firstCon->getOwner() != NULL && (*conListIter)->secondCon->getOwner() != NULL))
            {
                if ((*conListIter)->firstCon->getOwner() == (*conListIter)->secondCon->getOwner())
                {
                    conListIter++;
                    continue;
                }
            }
            delete (*conListIter);
            conList.erase(conListIter);
            mRefresh = true;
        }
        else conListIter++;
    }
}

void
CircuitWindow::mousePressed(gcn::MouseEvent &event)
{
    Window::mousePressed(event);
    Uint8* keys;
    keys = SDL_GetKeyState(NULL);
    if (!isVisible()) //kapat düğmesine basılınca
    {
        setVisible(false);
        if (current_npc) Net::getNpcHandler()->nextDialog(current_npc);
        current_npc = 0;

        for(miComponent=mvComponent.begin(); miComponent<mvComponent.end(); miComponent++)
            (*miComponent)->mDead=true;

        for(miNode=mvNode.begin(); miNode<mvNode.end(); miNode++)
            (*miNode)->setDead(true);
        conList.clear();

        miBrowserBox = mvBrowserBox.begin();
        while (miBrowserBox != mvBrowserBox.end())
        {
            delete (*miBrowserBox);
            miBrowserBox++;
        }

        miScrollArea = mvScrollArea.begin();
        while (miScrollArea != mvScrollArea.end())
        {
            delete (*miScrollArea);
            miScrollArea++;
        }

        mvScrollArea.clear();
        mvBrowserBox.clear();

        miLabel = mvLabel.begin();
        while (miLabel != mvLabel.end())
        {
            delete (*miLabel);
            miLabel++;
        }
        mvLabel.clear();
        mvImage.clear();
        mvAnim.clear();
    }
   //shifte basılıyken tıklanırsa yeni node oluştur
   else if (event.getButton() == gcn::MouseEvent::LEFT &&
            (keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT]) &&
            mNodeCreate)
   {
        Node *tempNode = new Node("com_node_btn.png","Hint", "com_node",this);
        tempNode->setId(findEmptyId());
        tempNode->setX(event.getX()-10);//+QALeftPad);
        tempNode->setY(event.getY()-25);//+QATopPad);
        tempNode->setEnabled(true);
        tempNode->setScroll(true);
        tempNode->setMovable(true);
        tempNode->setDeletable(true);
        tempNode->setSelectable(true);
        tempNode->setFree(true);
        tempNode->setToLink(true);
        tempNode->setFromLink(true);
        mvNode.push_back(tempNode);
        add(tempNode);
   }
}
void
CircuitWindow::swapNode(Node *find, Node *target)
{
    for (conListIter=conList.begin();conListIter<conList.end();conListIter++)
    {
        if ((*conListIter)->firstCon == find)
        {
            (*conListIter)->firstCon = target;
        }

        if ((*conListIter)->secondCon == find)
        {
            (*conListIter)->secondCon = target;
        }
    }
}

void
CircuitWindow::findConnectedNodeId()
{
    mvConnectedNodeId.clear();

    TmiNode nn, ns, ne;
    ns = mvNode.begin();
    ne = mvNode.end();
    for (nn = ns; nn<ne ; nn++)
    {
        TmvInt  tl;
        for (conListIter=conList.begin();conListIter<conList.end();conListIter++)
        {
             if ((*conListIter)->firstCon->getId()==(*nn)->getId()
                 && (*conListIter)->active )
                {
                    tl.push_back((*conListIter)->secondCon->getId());
                }
             if ((*conListIter)->secondCon->getId()==(*nn)->getId()
                 && (*conListIter)->active)
              {
                tl.push_back((*conListIter)->firstCon->getId());
              }
        }
        mvConnectedNodeId[(*nn)->getId()]=tl;
    }
}

//mvConnectedNodeId içinden komşu node'ları bul.
TmvInt
CircuitWindow::getConnectedNodeId(int target, int prev)
{
    TmvInt dondur;
    TmvInt hi = mvConnectedNodeId[target];
    for(TmiInt komsu = hi.begin(); komsu != hi.end(); komsu++)
    {
        if(*komsu != prev)
            dondur.push_back(*komsu);
    }
    return dondur;
}

void
CircuitWindow::winnowMesh()
{
    logger->log("aynı ilmekleri temizle");
    //birbirinin aynısı olanları temizle
    if (mvMesh.size() == 0) return;
    miMesh=mvMesh.begin();
    while(miMesh != mvMesh.end())
    {
        TmiIntMatris iki = miMesh;
        iki++;
        while(iki != mvMesh.end())
        {
            TmvInt yedek1, yedek2;
            yedek1 = miMesh->second;
            yedek2 = iki->second;
            std::sort(yedek1.begin(), yedek1.end());
            std::sort(yedek2.begin(), yedek2.end());

            if (std::equal( yedek1.begin(),
                            yedek1.end(),
                            yedek2.begin()))
            {
                mvMesh.erase(iki--);
            }
            iki++;
        }
        miMesh++;
    }
    logger->log("uzun düğümleri temizle");
    //uzun düğümleri temizle
    miMesh=mvMesh.begin();
    while(miMesh != mvMesh.end())
    {
        TmiIntMatris iki = miMesh;
        iki++;
        while(iki != mvMesh.end())
        {
            TmvInt yedek1, yedek2;
            yedek1 = miMesh->second;
            yedek2 = iki->second;
            if (*(yedek1.end()-1) == *(yedek2.end()-1) &&
                *yedek1.begin() == *yedek2.begin())
            {
                mvMesh.erase(iki--);
            }
            iki++;
        }
        miMesh++;
    }
    dugumListResort();
    logger->log("uzun düğümler temizlendi");

}

void
CircuitWindow::addLoopToMesh()
{
    TmvInt ust;
    TmiInt teki, i;
    TmiIntMatris miLoop = mvNodeLoop.begin();  // ilmek denemelerindeki her bir satır
    int prevNodeId = 0; // bu node gelmeye sebep olan node'un id'si
    int cevsinir = mvNodeLoop.size();
    bool dugumvar = false;

    //eğer eleman sayısı node sayısına eşitse kontrol etmeye en baştan başla
    if (mvNodeLoop.size() == mvNode.size())
    {
        miLoop = mvNodeLoop.begin();
    }

    while(miLoop!=mvNodeLoop.end())
    {
        ust = miLoop->second;   // map'in ikinci elemanı loop denemelerini tutan int vector
        if (ust.size() < 2)     // 2'den azsa loop eleman sayısı çevrim olamaz
            prevNodeId = 0;
        else                    // değilse kendisinden öncekiyle birlikte komşu bulma fonk'una yolla
            prevNodeId = ust.at(ust.size() - 2);
        TmvInt alt = getConnectedNodeId(ust.back(), prevNodeId);
        for (i = alt.begin(); i != alt.end(); i++)
        {
            bool kiv = false;
            TmiInt sin = ust.begin();
            sin++;
            for(teki = sin; teki != ust.end(); teki++)
              if (*teki == *i ) kiv = true;     //kıvrım bulununca yaz : aynı node tekrar ediyorsa ilmek olamaz
            if (!kiv)
            {
                if (( *i) == ust.front())       //ilmek bulununca yaz  ilk == son
                {
                    mvMesh[mvMesh.size()]=ust;
                }
                else
                {
                    ust.push_back(*i);
                    mvNodeLoop[mvNodeLoop.size()] = ust;
                    ust.pop_back();
                }
            }
        }
        miLoop++;
    }
}

Node*
CircuitWindow::findNode(int id)
{
    TmiNode nit;
    for(nit = mvNode.begin(); nit != mvNode.end(); nit++)
        if ((*nit)->getId() == id) return (*nit);
    return NULL;
}

Component*
CircuitWindow::findComponent(int id)
{
    TmiComponent cit;
    for(cit = mvComponent.begin(); cit != mvComponent.end(); cit++)
        if ((*cit)->getId() == id) return (*cit);
    return NULL;
}
void
CircuitWindow::turnoffAllLamp()
{
    if (mvComponent.size() == 0) return;
    for ( miComponent = mvComponent.begin();
          miComponent != mvComponent.end();
          miComponent++)
    {
        if ((*miComponent)->getType()==LAMP || (*miComponent)->getType()== DIODE)
        {
            if ((*miComponent)->getStatus() != BURNED)
                (*miComponent)->setStatus(PASIVE);
        }
    }
}

bool
CircuitWindow::hasDoubleNode(Component *comp,TmvInt mesh)
{
    if (!comp) return false;
    bool var1 =false, var2=false;
    for (TmiInt i = mesh.begin(); i != mesh.end(); i++)
    {
        if (comp->node1->getId() == *i)
        {
            var1 = true;
        }
        if (comp->node2->getId() == *i)
        {
            var2 = true;
        }
    }
    return (var1&&var2);
}

bool
CircuitWindow::isNodeInMesh(Node *n,TmvInt mesh)
{
    if (!n) return false;
    bool var1 =false;
    for (TmiInt i = mesh.begin(); i != mesh.end(); i++)
    {
        if (n->getId() == *i)
        {
            var1 = true;
            break;
        }
    }
    return var1;
}

void
CircuitWindow::makeMatris()
{
    logger->log("matrisi hazırla");
    if (mvMesh.size() == 0)
    {
    #ifdef DEBUG
      mSb->addRow("ilmek bulunamadı.");
    #endif
    // Bütün node'ların akımını sıfırla
        for(TmiNode nit = mvNode.begin(); nit != mvNode.end(); nit++)
            (*nit)->setCurrent(0.0);
    // Bütün comp'ların akımını sıfırla
        for(TmiComponent cit = mvComponent.begin(); cit != mvComponent.end(); cit++)
        (*cit)->setCurrent(0.0);
        turnoffAllLamp();
        return;
    }
    else
    {
        int dugsirasi = 0;

        for (miMesh = mvMesh.begin();
             miMesh != mvMesh.end();
             miMesh++)
        {
            TmvComponentMatris resistanceCompMatris(mvMesh.size());  //bulunan ilmek sayısı kadar elemanı olsun
            TmvComponentMatris batteryCompMatris(1);                 //tek sutunlu olacakmış!!!
                                                                     //dirençlerin adresini saklamak için
            TmvInt nodes = miMesh->second;                           //ilmeği oluşturan node'lar
            TmvComponent resistanceComp;                             //ilmekte bulunan direnç
            TmvComponent batteryComp;                                //ilmekte bulunan pil

            for (TmiInt ni = nodes.begin() ;
                 ni != nodes.end();
                 ni++)
            {
                Node *niNode = findNode((*ni));                       //ilmekte id'si verilen nodun adresini bul
                Component *own = niNode->getOwner();                  //node'un sahibi olan component için
                if (own)                                              //node'un sahibi varsa
                {
                    if (isBattery(own))                               //node'un sahibi pilse
                    {
                        if(!isExistComponent(own, batteryComp)
                            && hasDoubleNode(own, nodes))
                        {
                            batteryComp.push_back(own);
                        }
                    }
                    if (isResistance(own))
                    {
                        if (!isExistComponent(own, resistanceComp)     //Node'un sahibi dirençse ekle ->her iki node'u da listedeyse
                            && hasDoubleNode(own, nodes)
                            && own->getStatus() != BURNED)
                            resistanceComp.push_back(own);
                    }
                }
            }
            resistanceCompMatris[dugsirasi] = resistanceComp;
            batteryCompMatris[0] = batteryComp;

            resistanceCompDeter[dugsirasi] = resistanceCompMatris;
            batteryCompDeter[dugsirasi] = batteryCompMatris;

            resistanceCompMatris.clear();
            batteryCompMatris.clear();

            dugsirasi++;
        }

        // matrisin diğer sütunlarını ekle
        for(TmatrisIter matIt = resistanceCompDeter.begin();
             matIt != resistanceCompDeter.end();
             matIt++)  // ana matris döngüsü
        {
            int satir = matIt->first;
            TmvComponentMatris matSat = matIt->second;
            int sutun = 0;

            TmvComponent other = matrisElemanBul(satir);
            sutun++;
        }

        direncYonBelirle();
        resistanceMatris.clear();
        int str1 = 0, stn1 = 0;
        for( TmatrisIter a = resistanceCompDeter.begin(); a != resistanceCompDeter.end(); a++)
        {
            TmvComponentMatris b = a->second;
            rowResistanceValue.clear();
            stn1=0;
            for( TmiComponentMatris c = b.begin(); c != b.end(); c++)
            {
                float deger = 0;
                int ara = 1;
                for( TmiComponent d = (*c).begin(); d != (*c).end(); d++)
                {
                    deger += (*d)->getValue();
                    if ((*d)->getYon()== -1) ara = -1;
                }
                if (str1 != stn1)
                {
                    deger *= ara;
                }
                rowResistanceValue.push_back(deger);
                stn1++;
            }
            resistanceMatris.push_back(rowResistanceValue);
            str1++;
        }
    }
    calculateBatteryValue();

    TmvFloat gslDirenc;

    for( TmiFloatMatris a = resistanceMatris.begin(); a!= resistanceMatris.end(); a++)
    {
        for( TmiFloat b = (*a).begin(); b != (*a).end(); b++)
        {
            gslDirenc.push_back(*b);
        }
    }

    double direnc[gslDirenc.size()];
    for (int i = 0; i < gslDirenc.size(); i++)
    {
        direnc[i] = gslDirenc[i]*1.0;
    }

    double pil[batteryValue.size()];

    for (int i = 0; i < batteryValue.size(); i++)
    {
        pil[i] = batteryValue[i]*1.0;
    }
#ifdef DEBUG
    //Componetlerin matrisini yaz
    yaz_DirencCompMatris();
    yaz_PilCompMatris();

    //componentlerin değerini yaz
    yaz_DirencDegerMatris();
    yaz_PilDegerMatris();
#endif


    double akim[batteryValue.size()];
    // gsl kullanarak oluşan matrisi çöz
    // bilinen hata: birbirinden bağımsız iki devre oluşturursan tek devre gibi çözmeye çalışıyor
    TmiFloatMatris a = resistanceMatris.begin();
    gsl_matrix_view r = gsl_matrix_view_array (direnc,resistanceMatris.size(),(*a).size());
    gsl_vector_view v = gsl_vector_view_array (pil, resistanceMatris.size());
    gsl_vector *x = gsl_vector_alloc (resistanceMatris.size());
    int s;
    gsl_permutation * p = gsl_permutation_alloc (resistanceMatris.size());
    gsl_linalg_LU_decomp (&r.matrix, p, &s);
    gsl_linalg_LU_solve (&r.matrix, p, &v.vector, x);
    // stdout.txt dosyasına yazdırmak gerekirse
    //gsl_vector_fprintf (stdout, x, "%g");
#ifdef DEBUG
    for(int i=0; i<resistanceMatris.size(); i++)
    {
        mSb->addRow("akım "+toString(i)+" = " +toString(gsl_vector_get (x, i)));
    }
#endif

// Bütün node'ların akımını sıfırla
    for(TmiNode nit = mvNode.begin(); nit != mvNode.end(); nit++)
        (*nit)->setCurrent(0.0);
// Bütün comp'ların akımını sıfırla
    for(TmiComponent cit = mvComponent.begin(); cit != mvComponent.end(); cit++)
        (*cit)->setCurrent(0.0);

//matris'teki akım değerlerini componentlere aktar

//int i = 0;
//        for(miMesh=mvMesh.begin(); miMesh != mvMesh.end(); miMesh++)
//        {
//            std::stringstream b;
//            for(TmiInt uf= miMesh->second.begin(); uf != miMesh->second.end(); uf++)
//            {
//                if(gsl_isinf(gsl_vector_get (x, i)) != 0)
//                {
//                    Node *ndc = findNode(*uf);
//                    ndc->setCurrent(0);
//                    Component *cmc = ndc->getOwner();
//                    if (cmc && hasDoubleNode(cmc, miMesh->second))
//                        cmc->setCurrent(0);
//                }
//                else
//                {
//                    Node *ndc = findNode(*uf);
//                    ndc->setCurrent(gsl_vector_get (x, i));
//                    Component *cmc = ndc->getOwner();
//                    if (cmc && hasDoubleNode(cmc, miMesh->second))
//                        cmc->setCurrent(cmc->getCurrent() + gsl_vector_get (x, i));
//                    b<<*uf<<" : "<<gsl_vector_get (x, i) <<" *";
//                }
//
//            }
//            #ifdef DEBUG
//                mSb->addRow(b.str());
//            #endif
//            i++;
//        }


// yeni yöntem : her nodun ait olduğu ilmek'leri bul. ilmek akımlarını topla.
    for(TmiNode nit = mvNode.begin(); nit != mvNode.end(); nit++)
    {
        int i = 0;
        for(miMesh=mvMesh.begin(); miMesh != mvMesh.end(); miMesh++)
        {
            if(isNodeInMesh(*nit, miMesh->second))
            {
                if(gsl_isinf(gsl_vector_get (x, i)) || gsl_isnan(gsl_vector_get (x, i)))
                {
                    (*nit)->setCurrent(0);
                }
                else
                {
                    (*nit)->setCurrent((*nit)->getCurrent()+gsl_vector_get (x, i) );
                }
            }
            i++;
        }
    }
    for(TmiComponent cit = mvComponent.begin(); cit != mvComponent.end(); cit++)
    {
        if ((*cit)->getStatus() ==BURNED || ((*cit)->getType()==SWITCH && (*cit)->getStatus() !=PASIVE ))
            (*cit)->setCurrent(0);
        else
            (*cit)->setCurrent((*cit)->node1->getCurrent());
    }
/** Yeni yöntem sonu*/
    // lambaların parlaklıklarını kontrol et ve yak
    turnonLamps();
    //gsl kaynaklarını geri ver
    gsl_permutation_free (p);
}

void
CircuitWindow::calculateBatteryValue()
{
    batteryValue.clear();
        int i = 0;

    for(TmatrisIter matIt = batteryCompDeter.begin();
        matIt != batteryCompDeter.end();
        matIt++)
    {
        double pildeger = 0.0;

        TmvComponentMatris matSat = matIt->second;
        for(TmiComponentMatris satIt= matSat.begin();
            satIt != matSat.end();
            satIt++)
        {
            TmvComponent temp = *satIt;
            for (TmiComponent matEl = temp.begin();
                  matEl != temp.end(); matEl++)
            {
                if ((*matEl)->getStatus())
                    continue;
                Node *nii = (*matEl)->node1;
                Node *nis = (*matEl)->node2;
                int yon = elemanYonKontrol(i,nii->getId() ,nis->getId() );

                pildeger += (*matEl)->getValue() * yon * -1;
#ifdef DEBUG
                mSb->addRow("    V : "+toString((*matEl)->getValue())+"    Yön : "+toString(yon));
#endif
            }
        }
        batteryValue.push_back(pildeger);
        #ifdef DEBUG
//        mSb->addRow("ilmek pil : "+toString(pildeger));
        #endif
        i++;
    }
}

void
CircuitWindow::turnonLamps()
{
//    float minCurrent=  99.9;
//    float maxCurrent= -99.9;
//    for (miComponent = mvComponent.begin(); miComponent != mvComponent.end(); miComponent++)
//    {
//        // gerekirse sadece lambalar arası karşılaştırma yapılacak
//        if(isLamp(*miComponent))
//        {
//            if((*miComponent)->getCurrent() > maxCurrent)
//                maxCurrent = (*miComponent)->getCurrent();
//            if((*miComponent)->getCurrent() < minCurrent)
//                minCurrent = (*miComponent)->getCurrent();
//        }
//    }
//    float interval = (maxCurrent - minCurrent) / 3.0;
//    #ifdef DEBUG
////    mSb->addRow("minimum :"+toString(minCurrent));
////    mSb->addRow("maximum :"+toString(maxCurrent));
////    mSb->addRow("interval:"+toString(interval));
//    #endif
    for (miComponent = mvComponent.begin(); miComponent != mvComponent.end(); miComponent++)
    {
        // gerekirse sadece lambalar arası karşılaştırma yapılacak
        if((*miComponent)->getStatus() == BURNED)
            continue;
        if(isLamp(*miComponent))
        {
            if ((*miComponent)->getCurrent() == 0.0f)
                (*miComponent)->setStatus(PASIVE);
            else
                (*miComponent)->setStatus(ACTIVE);
        }
    }
}

void
CircuitWindow::mouseMoved(gcn::MouseEvent &event)
{
    int x = event.getX();
    int y = event.getY();
    std::string mesaj = "";
    if(rectFromRight.isPointInRect(x,y)) mesaj = "Sağdan bağlantı yapılabilir";
    else if(rectFromLeft.isPointInRect(x,y)) mesaj = "Soldan bağlantı yapılabilir";
    else if(rectToLeft.isPointInRect(x,y)) mesaj = "Sola bağlantı yapılabilir";
    else if(rectToRight.isPointInRect(x,y)) mesaj = "Sağa bağlantı yapılabilir";
    else if(rectSelect.isPointInRect(x,y)) mesaj = "Seçilebilir";
    else if(rectRotate.isPointInRect(x,y)) mesaj = "Döndürülebilir";
    else if(rectMove.isPointInRect(x,y)) mesaj = "Taşınabilir";
    else if(rectErase.isPointInRect(x,y)) mesaj = "Silinebilir";
    if (mesaj != "")
    {
        mPopupLabel->setText(mesaj);
        mPopupLabel->setPosition(x,y);
        mPopupLabel->setVisible(true);
        mPopupLabel->requestMoveToTop();
    }
    else
        mPopupLabel->setVisible(false);
}

bool
CircuitWindow::isExistComponent(Component *comp, TmvComponent vect)
{
    bool var = false;
    for(TmiComponent matEl = vect.begin();
        matEl != vect.end();
        matEl++)
    {
        if ( *matEl == comp )
        {
            var = true;
            break;
        }
    }
    return var;
}

bool
CircuitWindow::isResistance(Component *comp)
{
    if (comp->getType() == LAMP ||
        comp->getType() == RESISTANCE
        )
        return true;
    else
        return false;
}

bool
CircuitWindow::isBattery(Component *comp)
{
    if (comp->getType() == BATTERY)
        return true;
    else
        return false;
}

bool
CircuitWindow::isLamp(Component *comp)
{
    if (comp->getType() == LAMP ||
        comp->getType() == DIODE)
        return true;
    else
        return false;
}

void
CircuitWindow::direncYonBelirle()
{
    for (int str =  0; str<resistanceCompDeter.size(); str++)
    {
        TmvComponent kosegen;
        kosegen = matrisComponentXY (str,str);
        for(int stn = 0; stn<=str; stn++)
        {
            for( TmiComponent koseeleman = kosegen.begin();
                 koseeleman != kosegen.end();
                 koseeleman++)
            {
                for (int hedefstr =  str+1;
                     hedefstr<resistanceCompDeter.size();
                     hedefstr++)
                {
                    TmvComponent hedef;
                    hedef = matrisComponentXY(hedefstr,stn);
                    Node *mii = (*koseeleman)->node1;
                    Node *mis = (*koseeleman)->node2;
                    int durum1 = elemanYonKontrol(str, mii->getId(), mis->getId() );

                    for( TmiComponent hedefeleman = hedef.begin();
                         hedefeleman != hedef.end();
                         hedefeleman++ )
                    {
                        if ((*koseeleman)->getId() == (*hedefeleman)->getId())
                        {
                            Node *nii = (*hedefeleman)->node1;
                            Node *nis = (*hedefeleman)->node2;
                            int durum2 = elemanYonKontrol(hedefstr,nii->getId() ,nis->getId() );
                            if (durum1 != durum2)
                            {
                                (*hedefeleman)->setYon(-1);
                            }
                        }
                    }
                }
            }
        }
    }
}

TmvComponent
CircuitWindow::matrisComponentXY(int satir, int sutun)
{
    TmatrisIter resistanceCompMatris =resistanceCompDeter.find(satir);
    TmvComponentMatris matrisEl = resistanceCompMatris->second;
    TmvComponent kaynak = matrisEl[sutun];
    return kaynak;
}

int
CircuitWindow::elemanYonKontrol(int satir, int node1, int node2)
{
    std::stringstream a;
    TmvInt sat= mvMesh[satir];
mSb->addRow("##8 aranan :"+toString(node1)+"-"+toString(node2)+" -- Satır : "+ toString(satir));
    TmiInt it;
    int dizi[] = {node1,node2};
    it = std::search(sat.begin(), sat.end(),dizi, dizi+1);

    if (it != sat.end())
        return 1;

    int dizi2[] = {node2,node1};
    it = std::search(sat.begin(), sat.end(),dizi2, dizi2+1);
    if (it != sat.end())
        return -1;

    mSb->addRow("##8 Bulamadım");
    return 9;
}

void
CircuitWindow::dugumListResort()
{
    TmvIntMatris gecici;
    int sira= 0;
    for (miMesh = mvMesh.begin(); miMesh != mvMesh.end(); miMesh++)
    {
        gecici[sira] = miMesh->second;
        sira++;
    }
    mvMesh.clear();
    mvMesh = gecici;
}

TmvComponent
CircuitWindow::matrisElemanBul(int satir)
{
    TmatrisIter resistanceCompMatris =resistanceCompDeter.find(satir);
    TmvComponentMatris matrisEl = resistanceCompMatris->second;
    TmvComponent kaynak = matrisEl[satir];
    TmvComponent sonuc;

    for(int sat = 0; sat< resistanceCompDeter.size(); sat++)
    {
        if (satir != sat)
        {
            for (int sut = 0; sut<matrisEl.size(); sut++)
            {
                TmatrisIter matrisSatir2 =resistanceCompDeter.find(sat);
                TmvComponentMatris matrisEl2 = matrisSatir2->second;
                TmvComponent hedef = matrisEl2[sat];
                for(TmiComponent bak = kaynak.begin(); bak != kaynak.end(); bak++)
                {
                    for(TmiComponent sak = hedef.begin(); sak != hedef.end(); sak++)
                    {
                        if ((*bak)->getId() == (*sak)->getId())
                        {
                            sonuc.push_back(*bak);
                            break;
                        }
                    }
                }
             TmatrisIter tempSatir = resistanceCompDeter.find(satir);
             tempSatir->second[sat]=sonuc;
             sonuc.clear();
            }
        }
    }
    return sonuc;
}

void
CircuitWindow::devreAnaliz()
{
    if (isVisible())
    {
        trashMeshMem();
        findConnectedNodeId();
        //mvNodeLoop için node id'lerini ekle: kontrol için hazırlık
        for (TmiNode nn = mvNode.begin(); nn<mvNode.end() ; nn++)
        {
            mvNodeLoop[mvNodeLoop.size()].push_back((*nn)->getId());
        }
        addLoopToMesh();
//       yaz_ConnectedNodeId();
//        showNodeLoop();
//        showMesh();
        winnowMesh();
        showMesh();
        makeMatris();
    }
}

void CircuitWindow::trashMeshMem()
{
    mRefresh = false;
    mvMesh.clear();
    mvNodeLoop.clear();
    mvConnectedNodeId.clear();
    resistanceCompDeter.clear();
    batteryCompDeter.clear();
    matrisAkim.clear();
    matrisGerilim.clear();
    resistanceMatris.clear();
    rowResistanceValue.clear();
    mWireRefresh = true;
     for(mTellerIter = mTeller.begin(); mTellerIter != mTeller.end(); mTellerIter++)
     {
        for(mTekTelIter = (*mTellerIter).begin(); mTekTelIter != (*mTellerIter).end(); mTekTelIter++)
        {
            delete (*mTekTelIter);
        }
     }
    mTeller.clear();

}

void CircuitWindow::deleteWidgets()
{
    for(miComponent=mvComponent.begin(); miComponent<mvComponent.end(); miComponent++)
        (*miComponent)->mDead=true;

    for(miNode=mvNode.begin(); miNode<mvNode.end(); miNode++)
        (*miNode)->setDead(true);

    miBrowserBox = mvBrowserBox.begin();
    while (miBrowserBox != mvBrowserBox.end())
    {
        delete (*miBrowserBox);
        miBrowserBox++;
    }

    miScrollArea = mvScrollArea.begin();
    while (miScrollArea != mvScrollArea.end())
    {
        delete (*miScrollArea);
        miScrollArea++;
    }

    mvScrollArea.clear();
    mvBrowserBox.clear();

    miLabel = mvLabel.begin();
    while (miLabel != mvLabel.end())
    {
        delete (*miLabel);
        miLabel++;
    }

    mvLabel.clear();
    mvImage.clear();
    mvAnim.clear();
    conList.clear();
}
void
CircuitWindow::action(const gcn::ActionEvent &event)
{
    mHint->setCaption(event.getId());
    mHint->adjustSize();
    if (event.getId() == "clear")
    {
        mSb->clearRows();
    }
    else if (event.getId() == "solve")
    {
        devreAnaliz();
    }
    else if (event.getId()=="startcancel")
    {
        Net::getNpcHandler()->listInput(current_npc, 1);
        setVisible(false);
        current_npc=0;
        NPC::isTalking = false;
    }
    else if (event.getId()=="startok")
    {
        Net::getNpcHandler()->listInput(current_npc, 2); //ilk menü
    }

    else if (event.getId() == "evaluate")
    {
        int cevap = 1;
        for(conSwitchIter = conSwitch.begin(); conSwitchIter != conSwitch.end(); conSwitchIter++)
        {
            Switch *se;
            se = dynamic_cast<Switch*> (findComponent((*conSwitchIter)->compId));
            if (se)
            {
                if ((*conSwitchIter)->stat)
                    se->setStatus(ACTIVE);
                else
                    se->setStatus(PASIVE);
            }
        }
        devreAnaliz();

        for(conLampIter = conLamp.begin(); conLampIter != conLamp.end(); conLampIter++)
        {
            Component *tmp = findComponent((*conLampIter)->compId);
            if ((tmp->getStatus() == ACTIVE ||
                 tmp->getStatus() == PLUS ||
                 tmp->getStatus() == PLUS2) &&
                (*conLampIter)->stat == 1)
            {
                cevap *= 1;
                continue;
            }
            else if (tmp->getStatus() == PASIVE &&  (*conLampIter)->stat == 0)
            {
                cevap *= 1;
                continue;
            }
            else
            {
                cevap = 0;
            }
        }
        for(conNodeIter = conNode.begin(); conNodeIter!= conNode.end(); conNodeIter++)
        {
            Node *tmp = findNode((*conNodeIter)->compId);
            if (tmp->getCurrent() == (*conNodeIter)->compCurrent)
                cevap *= 1;
            else cevap = 0;
        }
        for(conLocateIter = conLocate.begin(); conLocateIter!= conLocate.end(); conLocateIter++)
        {
            Component *tmp = findComponent((*conLocateIter)->compId);
            if ((*conLocateIter)->area.x - this->getPadding() <= tmp->getX() &&
                (*conLocateIter)->area.x - this->getPadding() +  (*conLocateIter)->area.width >= tmp->getX()&&
                (*conLocateIter)->area.y - this->getTitleBarHeight() <= tmp->getY() &&
                (*conLocateIter)->area.y - this->getTitleBarHeight() +  (*conLocateIter)->area.height >= tmp->getY())
                cevap *= 1;
            else cevap = 0;
        }
        if (cevap == 1)
        {
            mGidecekCevap = 2;
//            Net::getNpcHandler()->listInput(current_npc, 2);
        }

        else
        {
            mGidecekCevap = 3;
  //          Net::getNpcHandler()->listInput(current_npc, 3);
        }
        if (mWait==false)
            Net::getNpcHandler()->listInput(current_npc, mGidecekCevap);
        else
        {
            mCircState = WAIT_STATE;
            stateCheck();
        }

// TEMİZLİK
        for(conLampIter = conLamp.begin(); conLampIter != conLamp.end(); conLampIter++)
        {
            delete (*conLampIter);
        }
        for(conNodeIter = conNode.begin(); conNodeIter!= conNode.end(); conNodeIter++)
        {
            delete (*conNodeIter);
        }
        for(conLocateIter= conLocate.begin(); conLocateIter!= conLocate.end(); conLocateIter++)
        {
            delete (*conLocateIter);
        }
        for(conSwitchIter= conSwitch.begin(); conSwitchIter!= conSwitch.end(); conSwitchIter++)
        {
            delete (*conSwitchIter);
        }
        conLamp.clear();
        conNode.clear();
        conLocate.clear();
        conSwitch.clear();
    }
    else if (event.getId() == "FootOk")
    {
        setVisible(false);
        deleteWidgets();
        trashMeshMem();
        if (current_npc)
            Net::getNpcHandler()->nextDialog(current_npc);

        current_npc = 0;
        NPC::isTalking = false;
        sendUsedItem();
    }

    else if (event.getId() == "close")
    {
        setVisible(false);
        deleteWidgets();
        trashMeshMem();
//        int cevap = 1;
//        for(conLampIter = conLamp.begin(); conLampIter != conLamp.end(); conLampIter++)
//        {
//            Component *tmp = findComponent(*conLampIter);
//            if (tmp->getCurrent()>0)
//                cevap *= 1;
//            else cevap = 0;
//        }
        if (current_npc)
            Net::getNpcHandler()->nextDialog(current_npc);

        current_npc = 0;
        NPC::isTalking = false;
        sendUsedItem();
    }

    else if (event.getId() == "com_close")
    {
        for (miComponent=mvComponent.begin(); miComponent<mvComponent.end(); miComponent++)
        {
            if ((*miComponent)->getSelected())
            {
                (*miComponent)->mDead = true;

                // TODO (Emin#1#): item list'e iade edilecek
            }  //if selected
        } //for
    } //if com_close
    else if (event.getId() == "com_rotate_y+")
    {
        mWireRefresh = true;
        for (miComponent=mvComponent.begin(); miComponent<mvComponent.end(); miComponent++)
        {
            if ((*miComponent)->getSelected())
            {
                (*miComponent)->getAngel()<22?
                    (*miComponent)->setAngel((*miComponent)->getAngel()+1):
                    (*miComponent)->setAngel(0);
            }
        }
    }
    else if (event.getId() == "com_rotate_y-")
    {
        mWireRefresh = true;
        for (miComponent=mvComponent.begin(); miComponent<mvComponent.end(); miComponent++)
        {
            if ((*miComponent)->getSelected())
            {
                (*miComponent)->getAngel()>0?
                    (*miComponent)->setAngel((*miComponent)->getAngel()-1):
                    (*miComponent)->setAngel(22);
            }
        }
    }

    else if (event.getId() == "node_shift")
    {
        mWireRefresh = true;                            //telleri yeniden çiz
        Node *creatorNode=NULL;
        for (miNode = mvNode.begin(); miNode<mvNode.end(); miNode++)    //hangi nod'un buraya gelmeye sebep olduğunu bul
        {
            if ((*miNode)->getCreator())
            {
                creatorNode=(*miNode);
                break;
            }
        }

        if (creatorNode)
        {
            Node *tempNode = new Node("com_node_btn.png","Hint", "com_node",this);  //yerine bırakılacak nod'u oluştur
            tempNode->esitle(creatorNode);                                          //eski nodun özelliklerini aynen al
            tempNode->setEnabled(true);
            tempNode->setScroll(true);

            creatorNode->setId(findEmptyId());                                      //eski nod'a yeni id ver
            creatorNode->setFree(true);                                             //özgürlüğüne kavuştur
            creatorNode->setEnabled(true);
            creatorNode->setMovable(true);
            creatorNode->setToLink(true);
            creatorNode->setFromLink(true);
            creatorNode->setSelectable(true);
            creatorNode->setDeletable(true);
            creatorNode->setOwner(NULL);
            creatorNode->setScroll(true);
            mvNode.push_back(tempNode);                                             //nod vektörüne ekle
            add(tempNode);
//yukarda yapıldı
//            tempNode->nodeConnect(creatorNode);
//            creatorNode->nodeConnect(tempNode);

            swapNode(creatorNode, tempNode);

            for (miComponent=mvComponent.begin(); miComponent<mvComponent.end(); miComponent++)
            {
                if ((*miComponent)->node1 == creatorNode)
                {
                    tempNode->setFree(false);
                    tempNode->setMovable(false);
                    (*miComponent)->node1 = tempNode;
                    mSb->addRow("hareketsiz yap"+toString(tempNode->getId()));
                }
                if ((*miComponent)->node2 == creatorNode)
                {
                    tempNode->setMovable(false);
                    tempNode->setFree(false);
                    (*miComponent)->node2 = tempNode;
                    mSb->addRow("hareketsiz yap"+toString(tempNode->getId()));
                }
            }
            ConnectList *c=new ConnectList;
            c->active=true;
            c->draw = true;
            c->firstCon = creatorNode;
            c->secondCon = tempNode;
            conList.push_back(c);
        }
    }else if (event.getId() == "node_clear")
    {
        Component *master;

        std::vector<Node*>::iterator j = mvNode.begin();
        while (j != mvNode.end())
        {
            Node *nod = (*j);
            if (nod->getClean() && (nod->getFromLink() || nod->getToLink()))
            {
                clearNodeConnection(nod);
                nod->setClean(false);
            }
            j++;
        }
    }else if (event.getId() == "WaitOk")
    {
        Net::getNpcHandler()->listInput(current_npc, mGidecekCevap);
        mWait = false;
    }
}

void
CircuitWindow::distributeOlay(Item *it)
{
    ItemInfo tempItem = ItemDB::get(it->getId());
    it->setQuantity(it->getQuantity()-1);
    std::string tempType = tempItem.getElektroType();
    mUsedItem.push_back(it);

    Node *tempNode1 = new Node("com_node_btn.png","Hint", "com_node",this);
    tempNode1->setId(findEmptyId());
    tempNode1->setX(20);//+QALeftPad);
    tempNode1->setY(20);//+QATopPad);
    tempNode1->setEnabled(true);
    tempNode1->setScroll(false);
    tempNode1->setSelectable(true);
    tempNode1->setFree(false);
    tempNode1->setMovable(false);
    tempNode1->setDeletable(false);
    tempNode1->setToLink(true);
    tempNode1->setFromLink(true);

    mvNode.push_back(tempNode1);
    add(tempNode1);

    Node *tempNode2 = new Node("com_node_btn.png","Hint", "com_node",this);
    tempNode2->setId(findEmptyId());
    tempNode2->setX(20);//+QALeftPad);
    tempNode2->setY(20);//+QATopPad);
    tempNode2->setEnabled(true);
    tempNode2->setScroll(false);
    tempNode2->setSelectable(true);
    tempNode2->setFree(false);
    tempNode2->setMovable(false);
    tempNode2->setDeletable(false);
    tempNode2->setToLink(true);
    tempNode2->setFromLink(true);

    mvNode.push_back(tempNode2);
    add(tempNode2);

    localChatTab->chatLog(tempType.c_str(),BY_SERVER);
    Component *tempComponent;

    if (tempType=="resistance") tempComponent = new Resistance (this, tempNode1, tempNode2);
    else if (tempType=="lamp") tempComponent = new Lamp (this, tempNode1, tempNode2);
    else if (tempType=="diode") tempComponent = new Diode (this, tempNode1, tempNode2);
    else if (tempType=="battery") tempComponent = new Battery (this, tempNode1, tempNode2);
    else if (tempType=="switch") tempComponent = new Switch (this, tempNode1, tempNode2);
    else if (tempType=="ampermetre") tempComponent = new Ampermetre(this, tempNode1, tempNode2);
    else if (tempType=="motor") tempComponent = new Motor(this, tempNode1, tempNode2);
    else return;

        tempComponent->setId(findEmptyId());
        tempComponent->setValue(tempItem.getElektroValue());
        tempComponent->setX(150);//+QALeftPad);
        tempComponent->setY(150);//+QATopPad);
        tempComponent->setAngel(0);
        tempComponent->setStatus(ACTIVE);
        tempComponent->setItemId(transItemId);
        tempComponent->setMovable(1);
        tempComponent->setSelectable(1);
        tempComponent->setBounce(tempComponent->getX(),tempComponent->getY(),40,40);

        mvComponent.push_back(tempComponent);

        ConnectList *c=new ConnectList;
        c->firstCon = tempComponent->node1;
        c->secondCon = tempComponent->node2;
        c->active = (tempType == "switch" ? false : true);
        c->draw=false;
        conList.push_back(c);

        add(tempComponent);
        for (miNode = mvNode.begin(); miNode < mvNode.end(); miNode++)
            (*miNode)->requestMoveToTop();
}

void
CircuitWindow::statusChanged(Component *sw , Status st)
{
    Node *n1 = sw->node1;
    Node *n2 = sw->node2;
    bool tmp;
    if (st == PASIVE || st == ACTIVE || st == PLUS || st == PLUS2)
        tmp = true;
    if (st==BURNED)
        tmp = false;
    if (st == PASIVE && sw->getType()==SWITCH)
        tmp = false;
    for (conListIter=conList.begin();conListIter<conList.end();conListIter++)
    {
         if ((*conListIter)->firstCon->getId()== n1->getId() &&
             (*conListIter)->secondCon->getId()== n2->getId())
        {
            (*conListIter)->active = tmp;
        }
        else if ((*conListIter)->firstCon->getId()== n2->getId() &&
             (*conListIter)->secondCon->getId()== n1->getId())
        {
            (*conListIter)->active = tmp;
        }
    }
     mRefresh = true;
}

int
CircuitWindow::nodeConnectCount(Node *count)
{
    int counter=0;
        for (conListIter=conList.begin();conListIter<conList.end();conListIter++)
        {
             if ((*conListIter)->firstCon->getId()==count->getId())
                counter++;
             if ((*conListIter)->secondCon->getId()==count->getId())
                counter++;
        }
    return counter;
}

int
CircuitWindow::nodeConnectCount(int count)
{
    int counter=0;
        for (conListIter=conList.begin();conListIter<conList.end();conListIter++)
        {
             if ((*conListIter)->firstCon->getId()==count)
                counter++;
             if ((*conListIter)->secondCon->getId()==count)
                counter++;
        }
    return counter;
}

void
CircuitWindow::clearSelections()
{
    for(miComponent=mvComponent.begin(); miComponent<mvComponent.end(); miComponent++)
        (*miComponent)->setSelected(false);
}

void
CircuitWindow::moveComponents(int x, int y)
{
        mHint->setCaption(toString(x)+" - "+toString(y));
        mHint->adjustSize();

    for(miComponent=mvComponent.begin(); miComponent<mvComponent.end(); miComponent++)
    {
        if ((*miComponent)->getSelected())
            {
                (*miComponent)->setX(x);
                (*miComponent)->setY(y);
            }
    }
}

void
CircuitWindow::circuitFromXML(std::string mDoc)
{
    if (mDoc=="") return;
    xmlDocPtr mxmlDoc;

    mxmlDoc=  xmlParseMemory(mDoc.c_str(), mDoc.size());
    elektroWidget->padX = 120;
    elektroWidget->padY = 5;
    if (!mxmlDoc)
    {
        localChatTab->chatLog("Bu üstad'ın morali bozuk :(", BY_SERVER);
        localChatTab->chatLog("Bu durumu bir yöneticiye haber versen çok iyi olur.", BY_SERVER);
        return;
        //logger->error("circuitwindow.cpp: Error while parsing item database (from npc.xml)!"+mDoc);

    }
    xmlNodePtr rootNode = xmlDocGetRootElement(mxmlDoc);
    if (!rootNode || !xmlStrEqual(rootNode->name, BAD_CAST "circuit"))
    {
        localChatTab->chatLog("Bu üstad'ın morali bozuk :(", BY_SERVER);
        localChatTab->chatLog("Bu durumu bir yöneticiye haber versen çok iyi olur.", BY_SERVER);
        return;
//
//        logger->error("circuitwindow.cpp: rootNode not circuit!"+mDoc);
    }
   for_each_xml_child_node(node, rootNode)
    {
        // devreden önce gösterilen mesaj
        if (xmlStrEqual(node->name, BAD_CAST "head_mesaj"))
        {
            mCircState = HEAD_MESSAGE_STATE;
            mMessageText->clearRows();
            mMessageAutoWrap = false;
            setWidth(600);
            for_each_xml_child_node(subnode, node)
            {
                if (xmlStrEqual(subnode->name, BAD_CAST "addrow"))
                {
                    std::string mt = XML::getProperty(subnode, "text", "");
                    if (mt != "#autowrap#")
                        mMessageText->addRow(mt);
                    else
                        mMessageAutoWrap = true;
                }
                else if (xmlStrEqual(subnode->name, BAD_CAST "effect"))
                {
                    std::string effecttype= XML::getProperty(subnode, "type", "particle");
                    std::string effectname= XML::getProperty(subnode, "name", "dogru1");
                    std::string effectsound= XML::getProperty(subnode, "sound", "dogru1");
                    makeEffect(effecttype,effectname,effectsound);
                }
            }

        }

        else if (xmlStrEqual(node->name, BAD_CAST "foot_mesaj"))
        {
            mCircState = FOOT_MESSAGE_STATE;
            mMessageText->clearRows();
            mMessageAutoWrap = false;
            setWidth(600);
            bool bitir = false;
            for_each_xml_child_node(subnode, node)
            {
                if (xmlStrEqual(subnode->name, BAD_CAST "addrow"))
                {
                    std::string mt = XML::getProperty(subnode, "text", "");
                    std::string mtt = mt;
                    std::remove(mtt.begin(), mtt.end(), ' ');
                    if ( mtt == "#close# ")
                    {
                        bitir = true;
                    }
                    else if (mtt == "#autowrap#")
                        mMessageAutoWrap = true;
                    else
                        mMessageText->addRow(mt);
                }
                else if (xmlStrEqual(subnode->name, BAD_CAST "effect"))
                {
                    std::string effecttype= XML::getProperty(subnode, "type", "particle");
                    std::string effectname= XML::getProperty(subnode, "name", "dogru1");
                    std::string effectsound= XML::getProperty(subnode, "sound", "dogru1");
                    makeEffect(effecttype,effectname,effectsound);
                }
            }
            if (bitir)
            {
                addActionListener(this);
                const std::string &actionEventId="FootOk";
                setActionEventId(actionEventId);
                distributeActionEvent();
            }
        }
        else if (xmlStrEqual(node->name, BAD_CAST "window"))
        {
            //şartları temizle
            conLamp.clear();
            mWait = false;
            mGidecekCevap  = 0;
            int w =  XML::getProperty(node, "width", 350);
            int h =  XML::getProperty(node, "height", 275);
            int l =  XML::getProperty(node, "left", (800-getWidth())/2);
            int t =  XML::getProperty(node, "top", (600-getHeight())/2);
            setContentSize(w, h);
            setPosition(l,t);
            #ifdef DEBUG
                mSs->setY(getHeight()- mSs->getHeight()-10);
                mSs->setX(getWidth()-mSs->getWidth()-10);
            #endif
            int top = (getHeight()-cirToolBar->getHeight())/2;
            toolCaption->setPosition(60-toolCaption->getWidth()/2 ,top-3 );
            toolValue->setPosition(62,top +268 );
            closeButton->setY(((getHeight()-cirToolBar->getHeight())/2) + cirToolBar->getHeight()-30);
            solveButton->setY(closeButton->getY()+30);
            clearButton->setPosition(solveButton->getX()+30, solveButton->getY());
            int left = 40;
            int dy = 30;
            top += 38;
            rectFromRight = gcn::Rectangle(left,top,37,29);
            top += dy;
            rectFromLeft= gcn::Rectangle(left,top,37,29);
            top += dy;
            rectToRight = gcn::Rectangle(left,top,37,29);
            top += dy;
            rectToLeft= gcn::Rectangle(left,top,37,29);
            top += dy;
            rectErase = gcn::Rectangle(left,top,37,29);
            top += dy;
            rectSelect= gcn::Rectangle(left,top,37,29);
            top += dy;
            rectRotate= gcn::Rectangle(left,top,37,29);
            top += dy;
            rectMove= gcn::Rectangle(left,top,37,29);

        }
        else if (xmlStrEqual(node->name, BAD_CAST "info"))
        {
            mTotalTime = XML::getProperty(node, "totaltime", 0);
            mPunish = XML::getProperty(node, "punish", 0);
            mAward = XML::getProperty(node, "award", 0);
        }
        else if (xmlStrEqual(node->name, BAD_CAST "node"))
        {
            mCircState = CIRCUIT_STATE;
            Node *tempNode = new Node("com_node_btn.png","Hint", "com_node",this);

            tempNode->setId(XML::getProperty(node, "id", 0));
            tempNode->setX(XML::getProperty(node, "x", 0)+elektroWidget->padX);
            tempNode->setY(XML::getProperty(node, "y", 0)+elektroWidget->padY);
            tempNode->setEnabled(true);
            tempNode->setScroll(true);
            tempNode->setMovable(XML::getProperty(node, "movable", 1));
            tempNode->setSelectable(XML::getProperty(node, "selectable", 1));
            tempNode->setFree(true);
            tempNode->setDeletable(XML::getProperty(node, "deletable", 1));
            tempNode->setToLink(XML::getProperty(node, "tolink", 1));
            tempNode->setFromLink(XML::getProperty(node, "fromlink", 1));
            mvNode.push_back(tempNode);

            add(tempNode);
        }
        else if (xmlStrEqual(node->name, BAD_CAST "component"))
        {
            mCircState = CIRCUIT_STATE;
            int const itemid = XML::getProperty(node, "item_id", 0);
            ItemInfo tempItem = ItemDB::get(itemid);

            std::string tempType = tempItem.getElektroType();

            Node *tempNode1 = new Node("com_node_btn.png","Hint", "com_node",this);
            tempNode1->setId(XML::getProperty(node, "id", 0));
            tempNode1->setX(20);
            tempNode1->setY(20);
            tempNode1->setEnabled(true);
            tempNode1->setScroll(false);
            tempNode1->setSelectable(true);
            tempNode1->setFree(false);
            tempNode1->setMovable(false);
            tempNode1->setDeletable(false);
            tempNode1->setToLink(XML::getProperty(node, "tolink1", 1));
            tempNode1->setFromLink(XML::getProperty(node, "fromlink1", 1));
            mvNode.push_back(tempNode1);
            add(tempNode1);
            Node *tempNode2 = new Node("com_node_btn.png","Hint", "com_node",this);
            tempNode2->setId(XML::getProperty(node, "id", 0)+1);
            tempNode2->setX(20);
            tempNode2->setY(20);
            tempNode2->setEnabled(true);
            tempNode2->setScroll(false);
            tempNode2->setSelectable(true);
            tempNode2->setFree(false);
            tempNode2->setMovable(false);
            tempNode2->setDeletable(false);
            tempNode2->setToLink(XML::getProperty(node, "tolink2", 1));
            tempNode2->setFromLink(XML::getProperty(node, "fromlink2", 1));
            mvNode.push_back(tempNode2);
            add(tempNode2);

            Component *tempComponent;

            if (tempType=="resistance") tempComponent = new Resistance (this, tempNode1, tempNode2);
            else if (tempType=="lamp") tempComponent = new Lamp (this, tempNode1, tempNode2);
            else if (tempType=="diode") tempComponent = new Diode (this, tempNode1, tempNode2);
            else if (tempType=="battery") tempComponent = new Battery (this, tempNode1, tempNode2);
            else if (tempType=="switch") tempComponent = new Switch (this, tempNode1, tempNode2);
            else if (tempType=="motor") tempComponent = new Motor (this, tempNode1, tempNode2);
            else if (tempType=="ampermetre") tempComponent = new Ampermetre (this, tempNode1, tempNode2);
            else return;
            tempComponent->setItemId(itemid);
            tempComponent->setValue(tempItem.getElektroValue());
            tempComponent->setId(XML::getProperty(node, "id", 0));
            tempComponent->setX(XML::getProperty(node, "x", 0)+elektroWidget->padX);
            tempComponent->setY(XML::getProperty(node, "y", 0)+elektroWidget->padY);
            tempComponent->setAngel(XML::getProperty(node, "angel", 0));
            tempComponent->setMovable(XML::getProperty(node, "movable", 1));
            tempComponent->setSelectable(XML::getProperty(node, "selectable", 1));
            tempComponent->setDeletable(XML::getProperty(node, "deletable", 0));
            tempComponent->setStatus(PASIVE);
            tempComponent->setBounce(tempComponent->getX(),tempComponent->getY(),40,40);

            tempNode1->setOwner (tempComponent);
            tempNode2->setOwner (tempComponent);

            mvComponent.push_back(tempComponent);

            ConnectList *c=new ConnectList;
            c->firstCon = tempComponent->node1;
            c->secondCon = tempComponent->node2;
            c->active=true; //status değişince tekrar kontrol ediliyor
            c->draw=false;
            conList.push_back(c);
            add(tempComponent);
            tempComponent->setStatus(XML::getProperty(node, "status", PASIVE)); //herşey bittikten sonra statüyü değiştir
            if(tempComponent->getType() == SWITCH && tempComponent->getStatus()==PASIVE) c->active=false;
            if(tempComponent->getType() == MOTOR || tempComponent->getType() == AMPERMETRE)
            {
                tempComponent->setStatus(ACTIVE);
                c->active = true;
            }
            for (miNode = mvNode.begin(); miNode < mvNode.end(); miNode++)
                (*miNode)->requestMoveToTop();
        }
        else if (xmlStrEqual(node->name, BAD_CAST "circuitcondition"))
        {
            std::string type = XML::getProperty(node, "type", "");
            if(type == "lampturnon")
            {
                ConditionLamp *tmp = new ConditionLamp;
                tmp->compId = XML::getProperty(node, "componentid", 0);
                tmp->stat = XML::getProperty(node, "status", 0);
                conLamp.push_back(tmp);
            }
            else if(type == "nodecurrent")
            {
                std::stringstream hes;
                ConditionCurrent *cct = new ConditionCurrent;
                cct->compId = XML::getProperty(node, "componentid", 0);
                hes << XML::getProperty(node, "current", "");
                float hesf;
                hes >> hesf;
                cct->compCurrent = hesf;
                conNode.push_back(cct);
            }
            else if(type == "locate")
            {
                ConditionLocate *mmp = new ConditionLocate;
                mmp->compId = XML::getProperty(node, "componentid", 0);
                mmp->area.x = XML::getProperty(node, "x", 0)+elektroWidget->padX + this->getPadding();
                mmp->area.y = XML::getProperty(node, "y", 0)+elektroWidget->padY + this->getTitleBarHeight() ;
                mmp->area.width = XML::getProperty(node, "w", 0);
                mmp->area.height = XML::getProperty(node, "h", 0);
                conLocate.push_back(mmp);
            }
            else if(type == "evulationswitch")
            {
                mWait = true;
                Switch *se;
                logger->log("evulationswitch");
                ConditionSwitch *ftp = new ConditionSwitch;
                ftp->compId = XML::getProperty(node, "componentid", 0);
                ftp->stat = XML::getProperty(node, "status", 0);
                conSwitch.push_back(ftp);
                se = dynamic_cast<Switch*> (findComponent(ftp->compId));
                if (se)
                {
                    se->setControl(true);
                }
            }
        }
        else if (xmlStrEqual(node->name, BAD_CAST "connect"))
        {
            mCircState = CIRCUIT_STATE;
            ConnectList *c=new ConnectList;

            int c_id=XML::getProperty(node, "id", 0);
            int c_first=XML::getProperty(node, "first", 0);
            int c_second=XML::getProperty(node, "second", 0);
            int c_active=XML::getProperty(node, "active", 1);
            int c_firstn=XML::getProperty(node, "firstn", 0);
            int c_secondn=XML::getProperty(node, "secondn", 0);
            bool found=false;
            if (c_firstn>0)
            for(miComponent=mvComponent.begin(); miComponent<mvComponent.end(); miComponent++)
            {
                if ((*miComponent)->getId()==c_first)
                {
                    if(c_firstn==1)
                    {
                        c->firstCon = (*miComponent)->node1;
                    }
                    else
                    {
                        c->firstCon = (*miComponent)->node2;
                    }
                }
            }
        else
            for(miNode=mvNode.begin();miNode<mvNode.end();miNode++)
            {
                 if ((*miNode)->getId()==c_first)
                {
                    c->firstCon = (*miNode);
                }
            }

        if (c_secondn>0)
            for(miComponent=mvComponent.begin(); miComponent<mvComponent.end(); miComponent++)
            {
                if ((*miComponent)->getId()==c_second)
                {
                    c_secondn==1?c->secondCon = (*miComponent)->node1:
                                 c->secondCon = (*miComponent)->node2;
                }
            }
        else
            for(miNode=mvNode.begin();miNode<mvNode.end();miNode++)
            {
                 if ((*miNode)->getId()==c_second)
                {
                    c->secondCon = (*miNode);
                }
            }
        c->active=true;
        c->draw = true;
        conList.push_back(c);
        }
/****************************************************************/
        else if (xmlStrEqual(node->name, BAD_CAST "label"))
        {
            gcn::Label *templabel = elektroWidget->addLabel(node);
            add(templabel);
            mvLabel.push_back(templabel);
        }
        else if (xmlStrEqual(node->name, BAD_CAST "image"))
        {
            SmImage temp = elektroWidget->addImage(node);
            temp.y += this->getTitleBarHeight();
            temp.x += this->getPadding();
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
            temp.browserbox->autoWrap(temp.scrollarea);
            mvScrollArea.push_back(temp.scrollarea);
            mvBrowserBox.push_back(temp.browserbox);
        }
    }
    stateCheck();
}

int
CircuitWindow::findEmptyId()
{
    // kullanılmayan bir nod id'si bul
    int sonnode = 0;
    bool bulundu;
    do{
        sonnode++;
        bulundu = false;
        for (miNode = mvNode.begin();
                miNode<mvNode.end(); miNode++)
        {
            if ((*miNode)->getId() == sonnode) bulundu=true;
        }
    } while(bulundu);
    return sonnode;
}

void CircuitWindow::bindingNodes()
{

    //while ile değiştirilecek
   for (int z=0; z <5; z++)
    for (conListIter=conList.begin();conListIter<conList.end();conListIter++)
        {
            if ((*conListIter)->firstCon == movingNode)
                (*conListIter)->firstCon = targetNode;

            if ((*conListIter)->secondCon == movingNode)
                (*conListIter)->secondCon = targetNode;
        }
    movingNode->setDead(true);
}

void CircuitWindow::handleLink(const std::string& link)
{
    //
}


int CircuitWindow::sgn (long a)
{
  if (a > 0) return +1;
  else if (a < 0) return -1;
  else return 0;
}

void CircuitWindow::drawCircle(gcn::Graphics *g, int xc, int yc, int r)
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

void CircuitWindow::drawLine(gcn::Graphics *g,int a,int b,int c,int d, int t)
{
  TTekTel mTekTel;
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
//    drawCircle(g,a,b,t);
    SmPoint *point = new SmPoint;
    point->x = a;
    point->y = b;
    mTekTel.push_back(point);
//Draw_FillCircle(static_cast<SDL_Surface>(g),
//                        a, b, t,
//                        0xaabbcc);
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
  mTeller.push_back(mTekTel);
  mTekTel.clear();
}


void
CircuitWindow::makeEffect(std::string type,std::string name, std::string ssound)
{
    if (type=="particle")
    {
        Particle *dogruFX;
        dogruFX = particleEngine->addEffect("graphics/particles/"+name+".particle.xml", 0, 0);
        player_node->controlParticle(dogruFX);
    }
    else
    {
        particleEngine->addTextSplashEffect(name,
                                        player_node->getPixelX() + 16,
                                        player_node->getPixelY() + 16,
                                        &gcn::Color(255, 0, 255),
                                        boldFont);
    }
    if (ssound !="")
    {
        sound.playSfx("sfx/"+ssound+".ogg");
    }

}

void
CircuitWindow::sendUsedItem ()
{
    for(mIterUsedItem = mUsedItem.begin(); mIterUsedItem != mUsedItem.end(); mIterUsedItem++)
    {
        Net::getInventoryHandler()->useItem(*mIterUsedItem);
    }
    mUsedItem.clear();
}

void
CircuitWindow::yaz_DirencCompMatris()
{
    mSb->addRow("##1~~~~~~Direnç Component Matris~~~~~~");
    for(TmatrisIter matIt = resistanceCompDeter.begin(); matIt != resistanceCompDeter.end(); matIt++)
    {
        std::stringstream b;
        b<<matIt->first<<" | ";
        TmvComponentMatris matSat = matIt->second;
        for(TmiComponentMatris satIt= matSat.begin(); satIt != matSat.end(); satIt++)
        {
            TmvComponent temp = *satIt;
            for (std::vector< Component* >::iterator matEl = temp.begin();
                  matEl != temp.end(); matEl++)
            {
                b<<(*matEl)->getId();
                if (matEl+1 != temp.end()) b<<" +";
            }
            if (satIt+1 !=matSat.end()) b<<"   -   ";
        }
        b<<" |";
    mSb->addRow(b.str());
    }
}

void
CircuitWindow::yaz_PilCompMatris()
{
    mSb->addRow("##1~~~~~~Pil Component Matris~~~~~~");
    for(TmatrisIter matIt = batteryCompDeter.begin();
        matIt != batteryCompDeter.end();
        matIt++)
    {
        std::stringstream b;
        b<<matIt->first<<" | ";
        TmvComponentMatris matSat = matIt->second;
        for(TmiComponentMatris satIt= matSat.begin();
            satIt != matSat.end();
            satIt++)
        {
            TmvComponent temp = *satIt;
            for (TmiComponent matEl = temp.begin();
                  matEl != temp.end(); matEl++)
            {
                b<<(*matEl)->getId();
                if (matEl+1 != temp.end()) b<<" +";
            }
            if (satIt+1 !=matSat.end()) b<<"   -   ";
        }
        b<<" |";
        mSb->addRow(b.str());
    }
}

void
CircuitWindow::yaz_ConnectedNodeId()
{
    mSb->addRow("~~~~~~ Node'un komşuları ~~~~~~");
    for(miConnectedNodeId = mvConnectedNodeId.begin(); miConnectedNodeId != mvConnectedNodeId.end(); miConnectedNodeId++)
    {
        std::stringstream b;
        b<<miConnectedNodeId->first<<" - ";
        for(TmiInt uf= miConnectedNodeId->second.begin(); uf != miConnectedNodeId->second.end(); uf++)
        {
            b<<*uf;
            if (uf+1 != miConnectedNodeId->second.end())
                b <<" * ";
        }
        mSb->addRow(b.str());
    }
}

void
CircuitWindow::showNodeLoop()
{
    mSb->addRow("~~~~cevrim~~~~~~");

    for(miNodeLoop=mvNodeLoop.begin(); miNodeLoop != mvNodeLoop.end(); miNodeLoop++)
    {
        std::stringstream b;
        b<<miNodeLoop->first<<" - ";
        for(TmiInt uf= miNodeLoop->second.begin(); uf != miNodeLoop->second.end(); uf++)
            b<<*uf <<" *";
    mSb->addRow(b.str());
    }
}

void
CircuitWindow::showMesh()
{
    mSb->addRow("##3~~~~ İlmek ~~~~~~");

    for(miMesh=mvMesh.begin(); miMesh != mvMesh.end(); miMesh++)
    {
        std::stringstream b;
        b<<"##3"<<miMesh->first<<" - ";
        for(TmiInt uf= miMesh->second.begin(); uf != miMesh->second.end(); uf++)
            b<<*uf <<" *";
        mSb->addRow(b.str());
    }
}

void
CircuitWindow::yaz_DirencDegerMatris()
{
    mSb->addRow("##1~~~~~~Direnç Değer Matris~~~~~~");
    for( TmiFloatMatris a = resistanceMatris.begin(); a!= resistanceMatris.end(); a++)
    {
        std::stringstream yaz;
        yaz<<" | ";
        for( TmiFloat b = (*a).begin(); b != (*a).end(); b++)
        {
            if ( *b >= 0 ) yaz<<" ";
            yaz<<*b;
//            if ( *b >= 0 ) yaz<<" ";
            if ( b+1 != (*a).end()) yaz<<"     ";
        }
        yaz<<" |";
        mSb->addRow("##4 "+yaz.str());
    }
}

void
CircuitWindow::yaz_PilDegerMatris()
{
    mSb->addRow("##1~~~~~~Pil Değer Matris~~~~~~");
    for( TmiFloat a = batteryValue.begin(); a!= batteryValue.end(); a++)
    {
        std::stringstream yaz;
        yaz<<" | "<< (*a);

        yaz<<" |";
        mSb->addRow("##4 "+yaz.str());
    }
}
