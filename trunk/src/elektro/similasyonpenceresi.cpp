#include <cstdlib>
#include <ctime>

#include "similasyonpenceresi.h"
#include "elektrowidget.h"
#include "log.h"
#include "npc.h"
#include "net/ea/npchandler.h"

#include <guichan/font.hpp>

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
    mText = "";

    mmTurkce.insert(std::make_pair("ç","c"));
    mmTurkce.insert(std::make_pair("ÄŸ","g"));
    mmTurkce.insert(std::make_pair("ı","i"));
    mmTurkce.insert(std::make_pair("ö","o"));
    mmTurkce.insert(std::make_pair("ş","s"));
    mmTurkce.insert(std::make_pair("ü","u"));
    mmTurkce.insert(std::make_pair("Ç","C"));
    mmTurkce.insert(std::make_pair("Ğ","G"));
    mmTurkce.insert(std::make_pair("İ","I"));
    mmTurkce.insert(std::make_pair("Ö","O"));
    mmTurkce.insert(std::make_pair("Ş","S"));
    mmTurkce.insert(std::make_pair("Ü","U"));

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
//                if (xmlStrEqual(subNode->name, BAD_CAST "addrow"))
//                {
//                    mSoru->addRow(XML::getProperty(subNode, "text", ""));
//                }
                if (xmlStrEqual(subNode->name, BAD_CAST "addrow"))
                {
                    mText += XML::getProperty(subNode, "text", "");
                }
            }

            autoWrap(mSoruArea,mSoru,mText);
            add(mSoruArea);
            mText = "";
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

void
SimilasyonPenceresi::autoWrap(ScrollArea *textArea,BrowserBox *browserBox,std::string text)
{
    gcn::Font *font = getFont();
    std::string karakter;
    std::string temp;
    std::string sonKarakter="";
    std::string kelime="";
    std::string sonrakiKelime="";

    int x = 0;
    sonKarakter = text[text.size()-1];

//    while(text.find("ç") != std::string::npos)
//    {
//        harfSirasi.push_back(text.find("ç"));
//        harf[sayac] = harfSirasi;
//        text.replace(text.find("ç"),2,"c");
//    }

    for (int i=0;i<text.size();i++)
    {
        karakter = text[i];

        x+=font->getWidth(karakter);
        if (karakter == " " || karakter == sonKarakter )
        {



            int sonrakiBosluk = text.find(" ",i+1);
            sonrakiKelime = text.substr(i+1,sonrakiBosluk-(i+1));
            int sonrakiKelimeGen = font->getWidth(sonrakiKelime);

            int sayi =0;
            //çğıöşüÇĞİÖŞÜ
//            if (sonrakiKelime.find("ç")!=std::string::npos)
//                sayi += farkBul(sonrakiKelime,"ç","c");

            if (sonrakiKelime.find("ğ")!=std::string::npos)
                sayi += farkBul(sonrakiKelime,"ğ","g");

//            if (sonrakiKelime.find("ı")!=std::string::npos)
//                sayi += farkBul(sonrakiKelime,"ı","i");
//
//            if (sonrakiKelime.find("ö")!=std::string::npos)
//                sayi += farkBul(sonrakiKelime,"ö","o");
//
//            if (sonrakiKelime.find("ş")!=std::string::npos)
//                sayi += farkBul(sonrakiKelime,"ş","s");
//
//            if (sonrakiKelime.find("ü")!=std::string::npos)
//                sayi += farkBul(sonrakiKelime,"ü","u");
//
//            if (sonrakiKelime.find("Ç")!=std::string::npos)
//                sayi += farkBul(sonrakiKelime,"Ç","C");
//
//            if (sonrakiKelime.find("Ğ")!=std::string::npos)
//                sayi += farkBul(sonrakiKelime,"Ğ","G");
//
//            if (sonrakiKelime.find("İ")!=std::string::npos)
//                sayi += farkBul(sonrakiKelime,"İ","I");
//
//            if (sonrakiKelime.find("Ö")!=std::string::npos)
//                sayi += farkBul(sonrakiKelime,"Ö","O");
//
//            if (sonrakiKelime.find("Ş")!=std::string::npos)
//                sayi += farkBul(sonrakiKelime,"Ş","S");
//
//            if (sonrakiKelime.find("Ü")!=std::string::npos)
//                sayi += farkBul(sonrakiKelime,"Ü","U");
//

            x -= sayi;
            kelime +=" ";

            if(x + sonrakiKelimeGen>textArea->getWidth() || i == text.size()-1)
            {
                mvRow.push_back(kelime);
                kelime= "";
                x = 0;
            }
        }
        else
        {
            kelime += karakter;
        }
    }

    //Yüksekliği ayarla
    textArea->setHeight(font->getHeight() * mvRow.size());

    //BrowserBox'a verileri yazdır
    for (int i=0;i<mvRow.size();i++)
    {
        browserBox->addRow(mvRow.at(i));
    }

    mvRow.clear();
}

int
SimilasyonPenceresi::farkBul(std::string word,std::string harf1,std::string harf2)
{
    gcn::Font *font = getFont();
    int sayac=0;
    int sayi;
    int a,b,tut;
    std::string h;
    a = font->getWidth(harf1);
    b = font->getWidth(harf2);

    if (b>a)
    {
        tut = a;
        a = b;
        b=tut;
    }

    for (int i=0;i<word.size();i++)
    {
        h =word[i];
        if (strcmp(h.c_str(),harf1.c_str()))
            sayac++;
    }

    sayac = sayac /2;
    sayi = font->getWidth(word) - sayac*(a-b);

    logger->log("sayac:%d",sayac);
    logger->log("kelime:%s",word.c_str());

    return sayi;
}

