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
//#include "board.h"
//#include "windoweleman.h"

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

#include "elektro/lang_tr.h"

#include "utils/stringutils.h"
#include "../utils/xml.h"
#include <libxml/xmlwriter.h>

#include "net/ea/npchandler.h"

#include "../npc.h"
#include "gsl/gsl_linalg.h"
#include "gsl/gsl_vector.h"

extern int current_npc;
extern std::string globalHint;
extern ElektroWidget *elektroWidget;

class Resistance;

CircuitWindow::CircuitWindow():
    Window("Devre Penceresi"),
    collisionCheck(false),
    nodeCollision(false)
{
    setWindowName("Devreci");
    logger->log("circuit window a geldi");

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
    add(toolCaption);

    toolValue = new gcn::Label("");
    toolValue->setPosition(170,getHeight()-130);
    add(toolValue);

    closeButton = new BitButton("dugme.png", "Degerlendir", "close",this);
    closeButton->setPosition(10,120);
    add(closeButton);

    solveButton = new BitButton("com_close_btn.png", "Degerlendir", "solve",this);
    solveButton->setPosition(10,140);
    add(solveButton);

    clearButton = new BitButton("com_rotate_btn.png", "Degerlendir", "clear",this);
    clearButton->setPosition(10,100);
    add(clearButton);

    mHint = new gcn::Label("");
    mHint->setPosition(10,10);
    mHint->adjustSize();
    add(mHint);

//    gcn::Label *mHint1 = new gcn::Label("");
//    mHint1->setPosition(100,10);
//    mHint1->setFont(verdana14);
//    mHint1->setBaseColor(gcn::Color(255,0,0));
//    mHint1->setForegroundColor(gcn::Color(50,100,150));
//    mHint1->setBackgroundColor(gcn::Color(255,255,255));
//    mHint1->adjustSize();
//    add(mHint1);

//******
    mSb = new BrowserBox();
    mSb->setOpaque(false);


    mSs = new ScrollArea(mSb);
    mSs->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);
    mSs->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);
    mSs->setVisible(true);
    mSs->setWidth(250);
    mSs->setHeight(200);
    mSs->setY(getHeight()- mSs->getHeight()-10);
    mSs->setX(getWidth()-mSs->getWidth()-10);

    add(mSs);
//***********/

    mX = new gcn::Label("X:");
    mX->setPosition(300,80);
    add(mX);

    mY = new gcn::Label("Y:");
    mY->setPosition(350,80);
    add(mY);
    globalHint="hint";
    setLocationRelativeTo(getParent());
}

CircuitWindow::~CircuitWindow(){
    delete mHint;
    delete toolCaption;
    delete mX;
    delete mY;
    deleteWidgets();

}

void
CircuitWindow::logic()
{
//    if (isVisible() == false) return;
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

    std::vector<Node*>::iterator j = mvNode.begin();
    while (j != mvNode.end())
    {
        Node *nod = (*j);

        if (nod->getDead())
        {
           for (int z=0; z <10; z++) //????????????????????????????????
            for (conListIter=conList.begin();conListIter<conList.end();conListIter++)
                {
                    if ((*conListIter)->firstCon == nod ||
                        (*conListIter)->secondCon == nod)
                        conList.erase(conListIter);
                }
            nod->setDead(false);
            delete nod;
            j = mvNode.erase(j);
            mRefresh = true;
        }
        else {
            j++;
        }
    }
    if (collisionCheck)
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
    if (mRefresh)
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
    if (nodeCollision)
      g->drawImage(nodeConnectImage,collisionNodeX,collisionNodeY);

    int top = getHeight() - 141;
    int bottom = getHeight() - 103;
    int x = 58;
    int dx = 30;

    g->drawImage(cirToolBar,50,getHeight()-180);

    if (toolFromRight)
        g->drawImage(cirFromRight, x, top);

    if (toolFromLeft)
        g->drawImage(cirFromLeft, x, bottom);
    x += dx;
    if (toolToRight)
        g->drawImage(cirToRight, x, top + 2);

    if (toolToLeft)
        g->drawImage(cirToLeft, x, bottom + 2);
    x += dx;

    if (toolErase)
        g->drawImage(cirErase, x, top);
    x += dx;

    if (toolSelect)
        g->drawImage(cirSelect, x, top);
    x += dx;

    if (toolRotate)
        g->drawImage(cirRotate, x, top);
    x += dx;

    if (toolMove)
        g->drawImage(cirMove,x , top - 1);

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

    graphics->setColor(gcn::Color(0x2299cc));
    for (conListIter=conList.begin();conListIter<conList.end();conListIter++)
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
        drawChildren(graphics);
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
        tempNode->setX(event.getX());//+QALeftPad);
        tempNode->setY(event.getY());//+QATopPad);
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
             if ((*conListIter)->firstCon->getId()==(*nn)->getId())
                {
                    tl.push_back((*conListIter)->secondCon->getId());
                }
             if ((*conListIter)->secondCon->getId()==(*nn)->getId())
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
CircuitWindow::showConnectedNodeId()
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
    mSb->addRow("##3~~~~dugum~~~~~~");

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
    logger->log("komsu ekle");

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
void
CircuitWindow::turnoffAllLamp()
{
    if (mvComponent.size() == 0) return;
    for ( miComponent = mvComponent.begin();
          miComponent != mvComponent.end();
          miComponent++)
    {
        if ((*miComponent)->getType()==LAMP)
        {
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
    logger->log("Bakalım bulmuşmu :%d - %d", comp->getId(), var1&&var2);
    return (var1&&var2);
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
        turnoffAllLamp();
        return;
    }
    else
    {
        #ifdef DEBUG
        mSb->addRow("Düğüm bulundu ve çözülecek");
        #endif
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
                            Node *nii = own->node1;
                            Node *nis = own->node2;
                            int yon = elemanYonKontrol(dugsirasi,nii->getId() ,nis->getId() );
                            own->setYon(yon);
                            batteryComp.push_back(own);
                        }
                    }
                    if (isResistance(own))
                    {
                        if (!isExistComponent(own, resistanceComp)     //Node'un sahibi dirençse ekle ->her iki node'u da listedeyse
                            && hasDoubleNode(own, nodes))
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
        matrisYaz();
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
//    mSb->addRow(" >> eşit değil>>> str1: " +toString(str1)+ " :stn1 "+toString(stn1));

                    deger *= ara;
                }
                rowResistanceValue.push_back(deger);
                stn1++;
            }
            resistanceMatris.push_back(rowResistanceValue);
            str1++;
        }
        direncMatrisYaz();
    }
    calculateBatteryValue();

    #ifdef DEBUG
    showBatteryMatris();
    #endif

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
    #ifdef DEBUG
         mSb->addRow(" batteryValue SIZE-----: "+toString(batteryValue.size()));
    #endif

    for (int i = 0; i < batteryValue.size(); i++)
    {
        pil[i] = batteryValue[i]*1.0;
    }

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
        mSb->addRow("akım =" +toString(gsl_vector_get (x, i)));
    }
#endif

// Bütün node'ların akımını sıfırla
    for(TmiNode nit = mvNode.begin(); nit != mvNode.end(); nit++)
        (*nit)->setCurrent(0.0);
// Bütün comp'ların akımını sıfırla
    for(TmiComponent cit = mvComponent.begin(); cit != mvComponent.end(); cit++)
        (*cit)->setCurrent(0.0);

int i = 0;
        for(miMesh=mvMesh.begin(); miMesh != mvMesh.end(); miMesh++)
        {
            std::stringstream b;
            for(TmiInt uf= miMesh->second.begin(); uf != miMesh->second.end(); uf++)
            {
                Node *ndc = findNode(*uf);
                ndc->setCurrent(gsl_vector_get (x, i));
                Component *cmc = ndc->getOwner();
                if (cmc && hasDoubleNode(cmc, miMesh->second))
                    cmc->setCurrent(cmc->getCurrent() + gsl_vector_get (x, i));
                b<<*uf<<" : "<<gsl_vector_get (x, i) <<" *";
            }
            #ifdef DEBUG
                mSb->addRow(b.str());
            #endif
            i++;
        }
    // lambaların parlaklıklarını kontrol et ve yak
    turnonLamps();
    //gsl kaynaklarını geri ver
    gsl_permutation_free (p);
}

void
CircuitWindow::calculateBatteryValue()
{
    batteryValue.clear();
    for(TmatrisIter matIt = batteryCompDeter.begin();
        matIt != batteryCompDeter.end();
        matIt++)
    {
        double pildeger = 0;
        TmvComponentMatris matSat = matIt->second;
        for(TmiComponentMatris satIt= matSat.begin();
            satIt != matSat.end();
            satIt++)
        {
            TmvComponent temp = *satIt;
            for (TmiComponent matEl = temp.begin();
                  matEl != temp.end(); matEl++)
            {
                pildeger += (*matEl)->getValue() * (*matEl)->getYon()* -1;
            }
        }
        turnonLamps();
        batteryValue.push_back(pildeger);
        #ifdef DEBUG
        mSb->addRow("ilmek pil : "+toString(pildeger));
        #endif
    }
}

void
CircuitWindow::turnonLamps()
{
    float minCurrent=  99.9;
    float maxCurrent= -99.9;
    for (miComponent = mvComponent.begin(); miComponent != mvComponent.end(); miComponent++)
    {
        // gerekirse sadece lambalar arası karşılaştırma yapılacak
        if((*miComponent)->getCurrent() > maxCurrent)
            maxCurrent = (*miComponent)->getCurrent();
        if((*miComponent)->getCurrent() < minCurrent)
            minCurrent = (*miComponent)->getCurrent();
    }
    float interval = (maxCurrent - minCurrent) / 3.0;
    mSb->addRow("minimum :"+toString(minCurrent));
    mSb->addRow("maximum :"+toString(maxCurrent));
    mSb->addRow("interval:"+toString(interval));
    for (miComponent = mvComponent.begin(); miComponent != mvComponent.end(); miComponent++)
    {
        // gerekirse sadece lambalar arası karşılaştırma yapılacak
        if(isLamp(*miComponent))
        {
            if ((*miComponent)->getCurrent() == 0)
                (*miComponent)->setStatus(PASIVE);
            else if((*miComponent)->getCurrent() >= minCurrent &&
                (*miComponent)->getCurrent() <= minCurrent + interval)
                (*miComponent)->setStatus(ACTIVE);
            else if((*miComponent)->getCurrent() >= minCurrent + interval &&
                (*miComponent)->getCurrent() < minCurrent + 2*interval)
                (*miComponent)->setStatus(PLUS);
            else if((*miComponent)->getCurrent() >= minCurrent + 2*interval &&
                (*miComponent)->getCurrent() < minCurrent + 3*interval)
                (*miComponent)->setStatus(PLUS2);
        }
    }
}

void
CircuitWindow::mouseMoved(gcn::MouseEvent &event)
{

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
    TmvInt sat= mvMesh[satir];
    TmiInt it;
    int dizi[] = {node1,node2};
    it = std::search(sat.begin(), sat.end(),dizi, dizi+2);
    if (it != sat.end())
        return 1;
    int dizi2[] = {node2,node1};
    it = std::search(sat.begin(), sat.end(),dizi2, dizi2+2);
    if (it != sat.end())
        return -1;
    return 0;
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

void
CircuitWindow::direncMatrisYaz()
{
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
CircuitWindow::matrisYaz()
{
    mSb->addRow("##1~~~~~~matris~~~~~~");
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
CircuitWindow::showBatteryMatris()
{
    mSb->addRow("##1~~~~~~pil matris~~~~~~");
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
    //    showConnectedNodeId();
    //    showNodeLoop();
    //    showMesh();
        winnowMesh();
        mSb->addRow("---   ----");
        showMesh();
        makeMatris();
        logger->log("************ analiz yapıldı ****************");
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
    logger->log("temizlik tamam");
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
    logger->log("CIRCUIT ACTION");
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
    else if (event.getId() == "close")
    {
        setVisible(false);
        deleteWidgets();
        trashMeshMem();
        if (current_npc)
            Net::getNpcHandler()->nextDialog(current_npc);

        current_npc = 0;
        NPC::isTalking = false;;
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
        Node *creatorNode=NULL;
        for (miNode = mvNode.begin(); miNode<mvNode.end(); miNode++)
        {
            if ((*miNode)->getCreator())
            {
                creatorNode=(*miNode);
                break;
            }
        }

        if (creatorNode)
        {
            Node *tempNode = new Node("com_node_btn.png","Hint", "com_node",this);
            tempNode->esitle(creatorNode);
            tempNode->setEnabled(true);
            tempNode->setScroll(true);

            creatorNode->setId(findEmptyId());
            creatorNode->setFree(true);
            creatorNode->setMovable(true);
            creatorNode->setToLink(true);
            creatorNode->setFromLink(true);
            creatorNode->setSelectable(true);
            creatorNode->setDeletable(true);
            creatorNode->setOwner(NULL);
//            creatorNode->clearConnect();
            mvNode.push_back(tempNode);
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
                }
                if ((*miComponent)->node2 == creatorNode)
                {
                    tempNode->setMovable(false);
                    tempNode->setFree(false);
                    (*miComponent)->node2 = tempNode;
                }
            }

            ConnectList *c=new ConnectList;
            c->active=true;
            c->draw = true;
            c->firstCon = creatorNode;
            c->secondCon = tempNode;
            conList.push_back(c);

        }
    }
}

void
CircuitWindow::distributeOlay(Item *it)
{
    localChatTab->chatLog("Geldim",BY_SERVER);
    ItemInfo tempItem = ItemDB::get(it->getId());

    std::string temp = tempItem.getElektroType();


    Node *tempNode1 = new Node("com_node_btn.png","Hint", "com_node",this);
    tempNode1->setId(findEmptyId());
    tempNode1->setX(20);//+QALeftPad);
    tempNode1->setY(20);//+QATopPad);
    tempNode1->setEnabled(true);
    tempNode1->setScroll(false);
    tempNode1->setSelectable(false);
    tempNode1->setFree(false);
    mvNode.push_back(tempNode1);
    add(tempNode1);

        Node *tempNode2 = new Node("com_node_btn.png","Hint", "com_node",this);
        tempNode2->setId(findEmptyId());
        tempNode2->setX(20);//+QALeftPad);
        tempNode2->setY(20);//+QATopPad);
        tempNode2->setEnabled(true);
        tempNode2->setScroll(false);
        tempNode2->setSelectable(true);
        tempNode2->setFree(true);
        mvNode.push_back(tempNode2);
        add(tempNode2);

//        std::string tempType = tempItem.getName();
        std::string tempType = ItemDB::get(it->getId()).getName();
        localChatTab->chatLog(tempType,BY_SERVER);
        Component *tempComponent;
////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//* Itemlar isimleriyle değil id'leriyle oluşturulacak

        if (tempType=="Direnç") tempComponent = new Resistance (this, tempNode1, tempNode2);
        else if (tempType=="Lamba") tempComponent = new Lamp (this, tempNode1, tempNode2);
        else if (tempType=="Yesil Led") tempComponent = new Diode (this, tempNode1, tempNode2);
        else if (tempType=="Kirmizi Led") tempComponent = new Diode (this, tempNode1, tempNode2);
        else if (tempType=="Beyaz Led") tempComponent = new Diode (this, tempNode1, tempNode2);
        else if (tempType=="Sari Led") tempComponent = new Diode (this, tempNode1, tempNode2);
        else if (tempType=="Uretec") tempComponent = new Battery (this, tempNode1, tempNode2);
        else if (tempType=="Anahtar") tempComponent = new Switch (this, tempNode1, tempNode2);

        tempComponent->setId(999);
        tempComponent->setX(150);//+QALeftPad);
        tempComponent->setY(150);//+QATopPad);
        tempComponent->setAngel(0);
        tempComponent->setStatus(0);
        tempComponent->setItemId(transItemId);
        tempComponent->setMovable(1);
        tempComponent->setSelectable(1);
// art yok!! yerine bişey bul
        tempComponent->setValue(1);
        tempComponent->setBounce(tempComponent->getX(),tempComponent->getY(),40,40);

        mvComponent.push_back(tempComponent);

        ConnectList *c=new ConnectList;
        c->firstCon = tempComponent->node1;
        c->secondCon = tempComponent->node2;
        c->active=true;
        c->draw=false;
        conList.push_back(c);

        add(tempComponent);
        for (miNode = mvNode.begin(); miNode < mvNode.end(); miNode++)
            (*miNode)->requestMoveToTop();
    localChatTab->chatLog("Gördüm",BY_SERVER);

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
        if (xmlStrEqual(node->name, BAD_CAST "window"))
        {
            int w =  XML::getProperty(node, "width", 350);
            int h =  XML::getProperty(node, "height", 275);
            int l =  XML::getProperty(node, "left", (800-getWidth())/2);
            int t =  XML::getProperty(node, "top", (600-getHeight())/2);
            setContentSize(w, h);
            setPosition(l,t);
            toolCaption->setPosition(115,h-165);
            toolValue->setPosition(170,h-95);
            #ifdef DEBUG
                mSs->setY(getHeight()- mSs->getHeight()-10);
                mSs->setX(getWidth()-mSs->getWidth()-10);
            #endif
        }
        if (xmlStrEqual(node->name, BAD_CAST "info"))
        {
            mTotalTime = XML::getProperty(node, "totaltime", 0);
            mPunish = XML::getProperty(node, "punish", 0);
            mAward = XML::getProperty(node, "award", 0);

        }
        else if (xmlStrEqual(node->name, BAD_CAST "node"))
        {
                Node *tempNode = new Node("com_node_btn.png","Hint", "com_node",this);
                tempNode->setId(XML::getProperty(node, "id", 0));
                tempNode->setX(XML::getProperty(node, "x", 0));//+QALeftPad);
                tempNode->setY(XML::getProperty(node, "y", 0));//+QATopPad);
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
            int const itemid = XML::getProperty(node, "item_id", 0);
            ItemInfo tempItem = ItemDB::get(itemid);

            std::string tempType = tempItem.getElektroType();

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
            tempNode1->setToLink(XML::getProperty(node, "tolink1", 1));
            tempNode1->setFromLink(XML::getProperty(node, "fromlink1", 1));
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
            tempNode2->setToLink(XML::getProperty(node, "tolink2", 1));
            tempNode2->setFromLink(XML::getProperty(node, "fromlink2", 1));
            mvNode.push_back(tempNode2);
            add(tempNode2);

//            tempNode1->nodeConnect(tempNode2);
//            tempNode2->nodeConnect(tempNode1);
            Component *tempComponent;

//!!!!!!!!!!!!!!!!!!!!!
            if (tempType=="resistance") tempComponent = new Resistance (this, tempNode1, tempNode2);
            else if (tempType=="lamp") tempComponent = new Lamp (this, tempNode1, tempNode2);
            else if (tempType=="diode") tempComponent = new Diode (this, tempNode1, tempNode2);
            else if (tempType=="battery") tempComponent = new Battery (this, tempNode1, tempNode2);
            else if (tempType=="switch") tempComponent = new Switch (this, tempNode1, tempNode2);
            else return;
tempComponent->setItemId(itemid);
            tempComponent->setValue(tempItem.getElektroValue());
            tempComponent->setId(XML::getProperty(node, "id", 0));
            tempComponent->setX(XML::getProperty(node, "x", 0));//+QALeftPad);
            tempComponent->setY(XML::getProperty(node, "y", 0));//+QATopPad);
            tempComponent->setAngel(XML::getProperty(node, "angel", 0));
            tempComponent->setStatus(XML::getProperty(node, "status", 0));
            tempComponent->setMovable(XML::getProperty(node, "movable", 1));
            tempComponent->setSelectable(XML::getProperty(node, "selectable", 1));
//            tempComponent->setValue(XML::getProperty(node, "value", 0));
            tempComponent->setDeletable(XML::getProperty(node, "deletable", 0));
            tempComponent->setBounce(tempComponent->getX(),tempComponent->getY(),40,40);
            tempComponent->setSelectable(1);
            tempComponent->setMovable(1);
            tempComponent->setDeletable(1);

            tempNode1->setOwner (tempComponent);
            tempNode2->setOwner (tempComponent);

            mvComponent.push_back(tempComponent);

            ConnectList *c=new ConnectList;
            c->firstCon = tempComponent->node1;
            c->secondCon = tempComponent->node2;
            c->active=true;
            c->draw=false;
            conList.push_back(c);
            add(tempComponent);
            for (miNode = mvNode.begin(); miNode < mvNode.end(); miNode++)
                (*miNode)->requestMoveToTop();
        }
        else if (xmlStrEqual(node->name, BAD_CAST "connect"))
        {
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
    }
    mRefresh = true;
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
    drawCircle(g,a,b,t);
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
}
