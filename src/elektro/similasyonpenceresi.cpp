#include <cstdlib>
#include <ctime>

#include "similasyonpenceresi.h"
#include "elektrowidget.h"
#include "log.h"
#include "npc.h"
#include "net/ea/npchandler.h"

extern int current_npc;
extern ElektroWidget *elektroWidget;

SimilasyonPenceresi::SimilasyonPenceresi():
    Window(_("Simulasyon"))
{
    setMinWidth(400);
    setMinHeight(200);
    setResizable(true);
    setVisible(false);
    pencereDurum= false;
    startCancelDurum = true;
    toplam = 0;

    mCancel = new Button("İptal","Sim_Cancel",this);
    mClose = new Button("Kapat","Sim_Close",this);
    mStart = new Button("Başla","Sim_Start",this);
    mControl = new Button("Kontrol Et","Sim_Control",this);

    nesneleriAyarla();
    add(mCancel);
    add(mStart);
    add(mClose);
    add(mControl);
}

SimilasyonPenceresi::~SimilasyonPenceresi()
{
    delete mCancel;
    delete mClose;
    delete mStart;
    delete mControl;
}

void
SimilasyonPenceresi::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "Sim_Cancel")
    {
        Net::getNpcHandler()->listInput(current_npc, 1);
        current_npc=0;
        NPC::isTalking = false;
        setVisible(false);
        clearComponent();
    }
    else if (event.getId()=="Sim_Close")
    {
        Net::getNpcHandler()->listInput(current_npc,  0xff);
        current_npc=0;
        NPC::isTalking = false;
        setVisible(false);
        startCancelDurum = true;
        clearComponent();
        idKefe.clear();
    }
    else if (event.getId() == "Sim_Start")
    {
        Net::getNpcHandler()->listInput(current_npc,2);
        mStart->setVisible(false);
        mCancel->setVisible(false);
        mClose->setVisible(true);
    }
    else if (event.getId() == "Sim_Control")
    {
        kontrolEt();
    }
}

void
SimilasyonPenceresi::draw(gcn::Graphics *graphics)
{
    Graphics *g = static_cast<Graphics*>(graphics);
    //Window sınıfının draw() fonksiyonunu çağırmadan
    //ekrana pencere çizilmez
    Window::draw(graphics);

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
SimilasyonPenceresi::parseXML(std::string mDoc)
{
    mxmlDoc = xmlParseMemory(mDoc.c_str(), mDoc.size());
    rootNode = xmlDocGetRootElement(mxmlDoc);
    if (mDoc=="") return;
//    logger->log("%s",mDoc.c_str());

    if (!mxmlDoc)
        logger->error("similasyonpenceresi.cpp: Error while parsing item database (from npc.xml)!"+mDoc);

    if (!rootNode || !xmlStrEqual(rootNode->name, BAD_CAST "similasyon"))
        logger->error("similasyonpenceresi.cpp: rootNode not similasyon!"+mDoc);


    //npchandler dan gelen veri parse ediliyor
    for_each_xml_child_node(node, rootNode)
    {
        if (xmlStrEqual(node->name, BAD_CAST "window"))
        {
            logger->log("parseXML");
            nesneleriAyarla();
            int w =  XML::getProperty(node, "width", 0);
            int h =  XML::getProperty(node, "height", 0);
            int x =  XML::getProperty(node, "left", 0);
            int y =  XML::getProperty(node, "top", 0);
            setContentSize(w, h);
            setPosition(x,y);

        }

        else if (xmlStrEqual(node->name, BAD_CAST "simwindow"))
        {
            startCancelDurum = false;
            nesneleriAyarla();
            clearComponent();
            int w =  XML::getProperty(node, "width", 0);
            int h =  XML::getProperty(node, "height", 0);
            int x =  XML::getProperty(node, "left", 0);
            int y =  XML::getProperty(node, "top", 0);
            setContentSize(w, h);
            setPosition(x,y);
            pencereDurum = true;

        }

        else if (xmlStrEqual(node->name, BAD_CAST "text"))
        {
            mSoru = new BrowserBox();
            mSoruArea = new ScrollArea(mSoru);
            int x = XML::getProperty(node, "x", 50);
            int y = XML::getProperty(node, "y", 50);
            int w = XML::getProperty(node, "width", 50);
            int h = XML::getProperty(node, "height", 50);
            mSoruArea->setWidth(w);
            mSoruArea->setHeight(h);
            mSoruArea->setPosition(x,y);

            //Eklenecek yazının arka planını
            //Kendi arka planına uygun hale getirir
            mSoru->setOpaque(false);
            mSoruArea->setVisible(true);

            for_each_xml_child_node(subNode,node)
            {
                if (xmlStrEqual(subNode->name, BAD_CAST "addrow"))
                {
                    mSoru->addRow(XML::getProperty(subNode, "text", ""));
                }
            }

            add(mSoruArea);

        }
        else if (xmlStrEqual(node->name, BAD_CAST "component"))
        {
            Kutle *nesne;
            std::vector<int> kefeAgirlik;
            int x = XML::getProperty(node, "x", 50);
            int y = XML::getProperty(node, "y", 50);
            int w = XML::getProperty(node, "width", 50);
            int h = XML::getProperty(node, "height", 50);
            int id = XML::getProperty(node, "id", 0);

            nesne = new BesKiloGram(this);
            nesne->setID(id);
            kefeAgirlik.push_back(0);
            kefeAgirlik.push_back(nesne->getAgirlik());
logger->log("IDsi:%d",nesne->getID());
            idKefe.insert(std::make_pair(nesne->getID(),kefeAgirlik));

            nesne->setX(x);
            nesne->setY(y);
            nesne->setWidth(w);
            nesne->setHeight(h);
            nesne->setVisible(true);
            mvKutle.push_back(nesne);
            add(nesne);
        }
        else if (xmlStrEqual(node->name, BAD_CAST "nesne"))
        {
            Kaldirac *nesne;
            int x = XML::getProperty(node, "x", 50);
            int y = XML::getProperty(node, "y", 50);
            int w = XML::getProperty(node, "width", 50);
            int h = XML::getProperty(node, "height", 50);
//
            kefe1=XML::getProperty(node, "kefe1", 0);
            kefe2=XML::getProperty(node, "kefe2", 0);
            kefe3=XML::getProperty(node, "kefe3", 0);

            nesne = new Kaldirac(this);
            nesne->setX(x);
            nesne->setY(y);
            nesne->setWidth(w-150);
            nesne->setHeight(h+150);
            nesne->setVisible(true);
            mvKaldirac.push_back(nesne);
            add(nesne);
        }
        else if (xmlStrEqual(node->name, BAD_CAST "simpleanim"))
        {
            SmAnim temp = elektroWidget->addAnim(node);
            mvAnim.push_back(temp);
        }
    }

//logger->log("parse bitti");
}

void
SimilasyonPenceresi::nesneleriAyarla()
{

    if (startCancelDurum)
    {
        mCancel->setX(85);
        mCancel->setY(150);
        mCancel->setVisible(true);

        mStart->setX(30);
        mStart->setY(150);
        mStart->setVisible(true);

        mClose->setX(500);
        mClose->setY(10);
        mClose->setVisible(false);

        mControl->setX(10);
        mControl->setY(250);
        mControl->setVisible(false);
    }
    else
    {
        mClose->setVisible(true);
        mControl->setVisible(true);
        mStart->setVisible(false);
        mCancel->setVisible(false);
    }
}

void
SimilasyonPenceresi::clearComponent()
{
    miKutle = mvKutle.begin();
    miKaldirac = mvKaldirac.begin();

    while(miKutle!=mvKutle.end())
    {
        delete (*miKutle);
        miKutle = mvKutle.erase(miKutle);
    }

    while(miKaldirac!=mvKaldirac.end())
    {
        delete (*miKaldirac);
        miKaldirac = mvKaldirac.erase(miKaldirac);
    }

    mvAnim.clear();

    delete mSoru;
    delete mSoruArea;
}

bool
SimilasyonPenceresi::getPencereDurum()
{
    return pencereDurum;
}
void
SimilasyonPenceresi::nesneyiAl(Item *it)
{
    Kutle *nesne;
    std::vector<int> kefeAgirlik;
    std::string nesneTipi = ItemDB::get(it->getId()).getName();
    int itemID = it->getId();

    if (itemID >= 1100)
    {
        nesne = new BesKiloGram(this);
        nesne->setID(findEmptyID());
        kefeAgirlik.push_back(0);
        kefeAgirlik.push_back(nesne->getAgirlik());
        logger->log("IDsi:%d",nesne->getID());
        idKefe.insert(std::make_pair(nesne->getID(),kefeAgirlik));

        nesne->setX(100);
        nesne->setY(150);
        nesne->setWidth(50);
        nesne->setHeight(50);
        nesne->setVisible(true);
        mvKutle.push_back(nesne);
        add(nesne);
    }
}

void
SimilasyonPenceresi::kontrolEt()
{
    toplam =0;
    for (idKefeIt = idKefe.begin();idKefeIt!=idKefe.end();idKefeIt++)
    {
        if ((*idKefeIt).second[0] != 0)
            toplam += (*idKefeIt).second[0] * (*idKefeIt).second[1];

            logger->log("Kefe:%d",(*idKefeIt).second[0]);
            logger->log("Agirlik:%d",(*idKefeIt).second[1]);
    }

    logger->log("Toplam:%d",toplam);
}

int
SimilasyonPenceresi::findEmptyID()
{
    srand(time(NULL));
    int yeniID=0;
    for (miKutle = mvKutle.begin();miKutle< mvKutle.end();miKutle++)
    {
        yeniID = rand()%1000 + 1;
        if ((*miKutle)->getID()== yeniID)
        {
            miKutle = mvKutle.begin();
            continue;
        }
    }

    return yeniID;
}

//void
//SimilasyonPenceresi::autoWrap()
//{
//    gcn::Font *font = getFont();
//    unsigned int y = 0;
//    unsigned int nextChar;
//    const char *hyphen = "~";
//    int hyphenWidth = font->getWidth(hyphen);
//    int x = 0;
//
//    for (TextRowIterator i = mTextRows.begin(); i != mTextRows.end(); i++)
//    {
//        std::string row = *i;
//        for (unsigned int j = 0; j < row.size(); j++)
//        {
//            std::string character = row.substr(j, 1);
//            x += font->getWidth(character);
//            nextChar = j + 1;
//
//            // Wraping between words (at blank spaces)
//            if ((nextChar < row.size()) && (row.at(nextChar) == ' '))
//            {
//                int nextSpacePos = row.find(" ", (nextChar + 1));
//                if (nextSpacePos <= 0)
//                {
//                    nextSpacePos = row.size() - 1;
//                }
//                int nextWordWidth = font->getWidth(
//                        row.substr(nextChar,
//                            (nextSpacePos - nextChar)));
//
//                if ((x + nextWordWidth + 10) > getWidth())
//                {
//                    x = 15; // Ident in new line
//                    y += 1;
//                    j++;
//                }
//            }
//            // Wrapping looong lines (brutal force)
//            else if ((x + 2 * hyphenWidth) > getWidth())
//            {
//                x = 15; // Ident in new line
//                y += 1;
//            }
//        }
//    }
//
//    setHeight(font->getHeight() * (mTextRows.size() + y));
//}
