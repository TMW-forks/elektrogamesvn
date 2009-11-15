#include "similasyonpenceresi.h"
#include "log.h"
#include "npc.h"
#include "net/ea/npchandler.h"

extern int current_npc;

SimilasyonPenceresi::SimilasyonPenceresi():
    Window(_("Simulasyon"))
{
    setMinWidth(300);
    setMinHeight(400);
    setResizable(true);
    setVisible(false);
    pencereDurum= false;

    mCancel = new Button("İptal","Sim_Cancel",this);
    mClose = new Button("Kapat","Sim_Close",this);
    mStart = new Button("Başla","Sim_Start",this);

    nesneleriAyarla();
    add(mCancel);
    add(mStart);
    add(mClose);
}

SimilasyonPenceresi::~SimilasyonPenceresi()
{
    delete  mCancel;
    delete  mClose;
    delete  mStart;
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
    }
    else if (event.getId()=="Sim_Close")
    {
        Net::getNpcHandler()->listInput(current_npc,  0xff);
        current_npc=0;
        NPC::isTalking = false;
        setVisible(false);
        //Pencere kapatıldığında nesneleri bellekten siler
        clearComponent();
    }
    else if (event.getId() == "Sim_Start");
    {
        mStart->setVisible(false);
        mCancel->setVisible(false);
        mClose->setX(250);
        mClose->setY(250);
        mClose->setVisible(true);
        Net::getNpcHandler()->listInput(current_npc,2);
    }
}

void
SimilasyonPenceresi::draw(gcn::Graphics *graphics)
{
    //Window sınıfının draw() fonksiyonunu çağırmadan
    //ekrana pencere çizilmez
    Window::draw(graphics);
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
        logger->error("circuitwindow.cpp: Error while parsing item database (from npc.xml)!"+mDoc);

    if (!rootNode || !xmlStrEqual(rootNode->name, BAD_CAST "similasyon"))
        logger->error("circuitwindow.cpp: rootNode not similasyon!"+mDoc);

    //npchandler dan gelen veri parse ediliyor
    for_each_xml_child_node(node, rootNode)
    {
        if (xmlStrEqual(node->name, BAD_CAST "window"))
        {
            nesneleriAyarla();
            int w =  XML::getProperty(node, "width", 0);
            int h =  XML::getProperty(node, "height", 0);
            int x =  XML::getProperty(node, "left", 0);
            int y =  XML::getProperty(node, "top", 0);
            setContentSize(w, h);
            setPosition(x,y);
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
            int x = XML::getProperty(node, "x", 50);
            int y = XML::getProperty(node, "y", 50);
            int w = XML::getProperty(node, "width", 50);
            int h = XML::getProperty(node, "height", 50);

            pencereDurum = true;
            nesne = new BesKiloGram(this);
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

            //pencereDurum = true;
            nesne = new Kaldirac(this);
            nesne->setX(x);
            nesne->setY(y);
            nesne->setWidth(w-10);
            nesne->setHeight(h);
            nesne->setVisible(true);
            //mvKutle.push_back(nesne);
            add(nesne);
        }
    }
//logger->log("parse bitti");
}

void
SimilasyonPenceresi::nesneleriAyarla()
{
    mCancel->setX(250);
    mCancel->setY(350);
    mCancel->setVisible(true);

    mStart->setX(200);
    mStart->setY(350);
    mStart->setVisible(true);
    mClose->setVisible(false);
}

void
SimilasyonPenceresi::clearComponent()
{
    miKutle = mvKutle.begin();

    while(miKutle!=mvKutle.end())
    {
        delete (*miKutle);
        miKutle = mvKutle.erase(miKutle);
    }

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
    std::string nesneTipi = ItemDB::get(it->getId()).getName();

    if (nesneTipi=="Direnç")
    {
        nesne = new BesKiloGram(this);
        nesne->setX(100);
        nesne->setY(150);
        nesne->setWidth(50);
        nesne->setHeight(50);
        nesne->setVisible(true);
        mvKutle.push_back(nesne);
        add(nesne);
    }
}

