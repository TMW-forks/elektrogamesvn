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

    mCancel =new Button("Kapat","Sim_Cancel",this);
    mStart =new Button("Başla","Sim_Start",this);
    mSoru = new BrowserBox();
    mSoruArea = new ScrollArea(mSoru);
    nesneleriAyarla();
    add(mCancel);
    add(mStart);
}

SimilasyonPenceresi::~SimilasyonPenceresi()
{
    delete  mCancel;
    delete  mStart;
    delete  mSoru;
    delete  mSoruArea;
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
        //Pencere kapatıldığında nesneleri bellekten siler
        clearComponent();
    }
    else if (event.getId() == "Sim_Start");
    {
        mStart->setVisible(false);
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
    {
        logger->error("circuitwindow.cpp: Error while parsing item database (from npc.xml)!"+mDoc);
    }
    if (!rootNode || !xmlStrEqual(rootNode->name, BAD_CAST "similasyon"))
    {
        logger->error("circuitwindow.cpp: rootNode not similasyon!"+mDoc);
    }


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
                    logger->log("text ekleme");
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

            nesne = new BesKiloGram(this);
            nesne->setX(x);
            nesne->setY(y);
            nesne->setWidth(w);
            nesne->setHeight(h);
            nesne->setVisible(true);
            mvKutle.push_back(nesne);
            add(nesne);
        }
    }
//logger->log("parse bitti");
}

void
SimilasyonPenceresi::nesneleriAyarla()
{
    mCancel->setX(250);
    mCancel->setY(500);
    mCancel->setVisible(true);

    mStart->setX(200);
    mStart->setY(500);
    mStart->setVisible(true);
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
