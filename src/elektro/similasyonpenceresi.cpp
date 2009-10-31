#include "similasyonpenceresi.h"
#include "beskilogram.h"

#include "log.h"
#include "npc.h"
#include "net/ea/npchandler.h"

extern int current_npc;

SimilasyonPenceresi::SimilasyonPenceresi()
{
    setWindowName("Similasyon Penceresi");
    logger->log("Similasyon Window Açılır");
    setMinWidth(300);
    setMinHeight(400);
    setResizable(true);
    setVisible(false);

    //Nesneler
    mCancel =new Button("Kapat","Sim_Cancel",this);
    mStart =new Button("Başla","Sim_Start",this);

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
    }
    else if (event.getId() == "Sim_Start");
    {
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
    mxmlDoc=  xmlParseMemory(mDoc.c_str(), mDoc.size());
    rootNode = xmlDocGetRootElement(mxmlDoc);

    //npchandler dan gelen veri parse ediliyor
    for_each_xml_child_node(node, rootNode)
    {
        if (xmlStrEqual(node->name, BAD_CAST "window"))
        {
            int w =  XML::getProperty(node, "width", 0);
            int h =  XML::getProperty(node, "height", 0);
            int x =  XML::getProperty(node, "left", 0);
            int y =  XML::getProperty(node, "top", 0);
            setContentSize(w, h);
            setPosition(x,y);
        }
        else if (xmlStrEqual(node->name, BAD_CAST "component"))
        {
            int x = XML::getProperty(node, "x", 50);
            int y = XML::getProperty(node, "y", 50);
            int w = XML::getProperty(node, "width", 50);
            int h = XML::getProperty(node, "height", 50);
            BesKiloGram *nesne = new BesKiloGram(this);
            nesne->setX(x);
            nesne->setY(y);
            nesne->setWidth(w);
            nesne->setHeight(h);
            nesne->setVisible(true);
            add(nesne);
            logger->log("nesne eklendi");
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
                    logger->log("text ekleme");
                    mSoru->addRow(XML::getProperty(subNode, "text", ""));
                }
            }

            add(mSoruArea);
        }
    }
}

void
SimilasyonPenceresi::nesneleriAyarla()
{
    mCancel->setX(150);
    mCancel->setY(getHeight()-150);
    mCancel->setVisible(true);

    mStart->setX(250);
    mStart->setY(getHeight()-150);
    mStart->setVisible(true);
}
