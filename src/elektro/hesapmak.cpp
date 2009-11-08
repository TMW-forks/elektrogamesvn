#include "hesapmak.h"
#include <guichan/widgets/label.hpp>

#include "gui/widgets/button.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/windowcontainer.h"
#include "gui/gui.h"
#include "gui/sdlinput.h"

#include "../localplayer.h"
#include "../log.h"
#include "../utils/dtor.h"

#include "utils/stringutils.h"
#include "elektro/lang_tr.h"


HesapMak::HesapMak():
        Window(LBLCALCULATOR)
{
    logger->log("hesap makinesi");
    setDefaultSize(windowContainer->getWidth() - 255, 25, 230, 200);

    setWidth(230);
    setHeight(225);
    setX(100);
    setY(100);

    mOndalik =0;
    mSayi1=0;
    mSayi2=0;
    mIslem=YOK;
    mSonuc=0;
    mIslemVar=false;

    mGosterge = new gcn::Label("0");
    mGosterge->setPosition(20,15);
    mGosterge->setSize(150,25);
    mGosterge->setForegroundColor(gcn::Color(0,0,0));
    mGosterge->setFont(boldFont);
    mGosterge->setAlignment(gcn::Graphics::CENTER);

    add(mGosterge);

    int mbutx=20, mbuty=160, mbutw=30, mbuth=28;

    mBut0 = new Button("0","0",this);
    mBut0->setPosition(mbutx,mbuty);
    mBut0->setSize(mbutw,mbuth);
    mBut0->setAlignment(gcn::Graphics::LEFT);
    add(mBut0);
    mbutx += mbutw +10;

    mButIsaret = new Button("+/-","+/-",this);
    mButIsaret->setPosition(mbutx,mbuty);
    mButIsaret->setSize(mbutw,mbuth);
    mButIsaret->setAlignment(gcn::Graphics::CENTER);
    add(mButIsaret);
    mbutx += mbutw +10;

    mButOndalik= new Button(",",".",this);
    mButOndalik->setPosition(mbutx,mbuty);
    mButOndalik->setSize(mbutw,mbuth);
    mButOndalik->setAlignment(gcn::Graphics::CENTER);
    add(mButOndalik);
    mbutx += mbutw +10;

    mButTopla= new Button("+","+",this);
    mButTopla->setPosition(mbutx,mbuty);
    mButTopla->setSize(mbutw,mbuth);
    mButTopla->setAlignment(gcn::Graphics::CENTER);
    add(mButTopla);
    mbutx =20;
    mbuty -= mbuth+10;

    mBut1 = new Button("1","1",this);
    mBut1->setPosition(mbutx,mbuty);
    mBut1->setSize(mbutw,mbuth);
    mBut1->setAlignment(gcn::Graphics::CENTER);
    add(mBut1);
    mbutx += mbutw +10;

    mBut2 = new Button("2","2",this);
    mBut2->setPosition(mbutx,mbuty);
    mBut2->setSize(mbutw,mbuth);
    mBut2->setAlignment(gcn::Graphics::CENTER);
    add(mBut2);
    mbutx += mbutw +10;

    mBut3= new Button("3","3",this);
    mBut3->setPosition(mbutx,mbuty);
    mBut3->setSize(mbutw,mbuth);
    mBut3->setAlignment(gcn::Graphics::CENTER);
    add(mBut3);
    mbutx += mbutw +10;

    mButCikar= new Button("-","-",this);
    mButCikar->setPosition(mbutx,mbuty);
    mButCikar->setSize(mbutw,mbuth);
    mButCikar->setAlignment(gcn::Graphics::CENTER);
    add(mButCikar);
    mbutx += mbutw +10;

    mButEsit= new Button("=","=",this);
    mButEsit->setPosition(mbutx,mbuty);
    mButEsit->setSize(mbutw,mbuth*2+10);
    mButEsit->setAlignment(gcn::Graphics::CENTER);
    add(mButEsit);
    mbutx =20;
    mbuty -= mbuth+10;

    mBut4 = new Button("4","4",this);
    mBut4->setPosition(mbutx,mbuty);
    mBut4->setSize(mbutw,mbuth);
    mBut4->setAlignment(gcn::Graphics::CENTER);
    add(mBut4);
    mbutx += mbutw +10;

    mBut5 = new Button("5","5",this);
    mBut5->setPosition(mbutx,mbuty);
    mBut5->setSize(mbutw,mbuth);
    mBut5->setAlignment(gcn::Graphics::CENTER);
    add(mBut5);
    mbutx += mbutw +10;

    mBut6= new Button("6","6",this);
    mBut6->setPosition(mbutx,mbuty);
    mBut6->setSize(mbutw,mbuth);
    mBut6->setAlignment(gcn::Graphics::CENTER);
    add(mBut6);
    mbutx += mbutw +10;

    mButCarp= new Button("x","x",this);
    mButCarp->setPosition(mbutx,mbuty);
    mButCarp->setSize(mbutw,mbuth);
    mButCarp->setAlignment(gcn::Graphics::CENTER);
    add(mButCarp);
    mbutx += mbutw +10;

    mbutx =20;
    mbuty -= mbuth+10;

    mBut7 = new Button("7","7",this);
    mBut7->setPosition(mbutx,mbuty);
    mBut7->setSize(mbutw,mbuth);
    mBut7->setAlignment(gcn::Graphics::CENTER);
    add(mBut7);
    mbutx += mbutw +10;

    mBut8 = new Button("8","8",this);
    mBut8->setPosition(mbutx,mbuty);
    mBut8->setSize(mbutw,mbuth);
    mBut8->setAlignment(gcn::Graphics::CENTER);
    add(mBut8);
    mbutx += mbutw +10;

    mBut9= new Button("9","9",this);
    mBut9->setPosition(mbutx,mbuty);
    mBut9->setSize(mbutw,mbuth);
    mBut9->setAlignment(gcn::Graphics::CENTER);
    add(mBut9);
    mbutx += mbutw +10;

    mButBol= new Button("/","/",this);
    mButBol->setPosition(mbutx,mbuty);
    mButBol->setSize(mbutw,mbuth);
    mButBol->setAlignment(gcn::Graphics::CENTER);
    add(mButBol);
    mbutx += mbutw +10;

    mbuty += mbuth + 10;
    mButTumSil= new Button("C","TumSil",this);
    mButTumSil->setPosition(mbutx,mbuty);
    mButTumSil->setSize(mbutw,mbuth);
    mButTumSil->setAlignment(gcn::Graphics::CENTER);
    add(mButTumSil);
    mbuty -= mbuth+10;
    mbutx =180;

    mButGeriSil= new Button("<-","GeriSil",this);
    mButGeriSil->setPosition(mbutx,mbuty);
    mButGeriSil->setSize(mbutw,mbuth);
    mButGeriSil->setAlignment(gcn::Graphics::CENTER);
    add(mButGeriSil);
}

HesapMak::~HesapMak() {
    delete mBut0;
    delete mBut1;
    delete mBut2;
    delete mBut3;
    delete mBut4;
    delete mBut5;
    delete mBut6;
    delete mBut7;
    delete mBut8;
    delete mBut9;
    delete mButTopla;
    delete mButCikar;
    delete mButCarp;
    delete mButBol;
    delete mButEsit;
//    delete mButSayiSil;
    delete mButTumSil;
    delete mButGeriSil;
    delete mButIsaret;
    delete mButOndalik;
    delete mGosterge;
}

void HesapMak::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "+")
    {
        std::stringstream gecici(mGosterge->getCaption());
        if (mIslem == YOK)
        {
            gecici>>mSayi1;
            mIslem = TOPLAMA;
        }
        else if (mIslem == TOPLAMA)
        {
            if (mIslemVar==false)
                gecici>> mSayi2;

            mSayi1 = mSayi1+mSayi2;
            mSonuc = mSayi1;
            mSayi2 = 0;
            mGosterge->setCaption(toString(mSonuc));
        }

        mIslemVar = true;
    }
    else if (event.getId() == "-")
    {
        std::stringstream gecici(mGosterge->getCaption());
        if (mIslem == YOK)
        {
            gecici>>mSayi1;
            mIslem = CIKARTMA;
        }
        else if (mIslem == CIKARTMA)
        {
            if (mIslemVar==false)
                gecici>> mSayi2;

            mSayi1 = mSayi1- mSayi2;
            mSonuc = mSayi1;
            mSayi2 = 0;
            mGosterge->setCaption(toString(mSonuc));
        }

        mIslemVar = true;
    }
    else if (event.getId() == "x")
    {
        std::stringstream gecici(mGosterge->getCaption());
        if (mIslem == YOK)
        {
            gecici>>mSayi1;
            mIslem = CARPMA;
        }
        else if (mIslem == CARPMA)
        {
            if (mIslemVar==false)
                gecici>> mSayi2;

            mSayi1 = mSayi1*mSayi2;
            mSonuc = mSayi1;
            mSayi2 = 0;
            mGosterge->setCaption(toString(mSonuc));
        }

        mIslemVar = true;
    }
    else if (event.getId() == "/")
    {
        std::stringstream gecici(mGosterge->getCaption());
        if (mIslem == YOK)
        {
            gecici>>mSayi1;
            mIslem = BOLME;
        }
        else if (mIslem == BOLME)
        {
            if (mIslemVar==false)
                gecici>> mSayi2;

            mSayi1 = mSayi1/mSayi2;
            mSonuc = mSayi1;
            mSayi2 = 0;
            mGosterge->setCaption(toString(mSonuc));
        }

        mIslemVar = true;
    }
    else if (event.getId()=="+/-")
    {
        std::stringstream gecici(mGosterge->getCaption());
        if (mIslem == YOK)
        {
            gecici>>mSayi1;
            mSayi1 *= -1.0;
            mGosterge->setCaption(toString(mSayi1));
        }
        else
        {
            gecici>>mSayi2;
            mSayi2 *= -1.0;
            mGosterge->setCaption(toString(mSayi2));
        }
    }
    else if (event.getId()== "=")
    {
        std::stringstream gecici(mGosterge->getCaption());
        gecici<<mGosterge->getCaption();
        gecici>>mSayi2;

        if (mIslem == TOPLAMA)
            mSonuc = mSayi1 + mSayi2;
        else if (mIslem == CIKARTMA)
            mSonuc = mSayi1 - mSayi2;
        else if (mIslem == CARPMA)
            mSonuc = mSayi1 * mSayi2;
        else if (mIslem == BOLME)
            mSonuc = mSayi1 / mSayi2;

        gecici<<mSonuc;
        gecici>>mS1;
        mGosterge->setCaption(toString(mSonuc));
        mIslemVar = true;
    }
    else if (event.getId() == "GeriSil")
    {
        std::string strSayi = mGosterge->getCaption();
        if(strSayi.length()>0)
        {
            strSayi = strSayi.substr(0,strSayi.length()-1);
            if (strSayi.length() == 0)
                mGosterge->setCaption("0");
            else
                mGosterge->setCaption(toString(strSayi));
        }
    }
    else if (event.getId() == "TumSil")
    {
        mSayi1 = 0;
        mSayi2 = 0;
        mSonuc = 0;
        mGosterge->setCaption(toString(mSonuc));
    }

    else if (event.getId()==".")
    {
        std::string strSayi = mGosterge->getCaption();
        if (mIslem == YOK)
        {
            strSayi += ".";
            mGosterge->setCaption(toString(strSayi));
        }
        else
        {
            strSayi += ".";
            mGosterge->setCaption(toString(strSayi));
        }
    }
    else
        mGosterge->setCaption(getString(event.getId()));
}

std::string HesapMak::getString(std::string stringSayi)
{
    std::string gosterge = mGosterge->getCaption();

    if (stringSayi=="1")
    {
        if((gosterge=="0" && mIslem==YOK) || mIslemVar)
        {
            gosterge = "1";
            mIslemVar = false;
        }
        else
            gosterge +="1";
    }
    else if (stringSayi=="2")
    {
        if((gosterge=="0" && mIslem==YOK) || mIslemVar)
        {
            gosterge = "2";
            mIslemVar = false;
        }
        else
            gosterge +="2";
    }
    else if (stringSayi=="3")
    {
        if((gosterge=="0" && mIslem==YOK) || mIslemVar)
        {
            gosterge = "3";
            mIslemVar = false;
        }
        else
            gosterge +="3";
    }
    else if (stringSayi=="4")
    {
        if((gosterge=="0" && mIslem==YOK) || mIslemVar)
        {
            gosterge = "4";
            mIslemVar = false;
        }
        else
            gosterge +="4";
    }
    else if (stringSayi=="5")
    {
        if((gosterge=="0" && mIslem==YOK) || mIslemVar)
        {
            gosterge = "5";
            mIslemVar = false;
        }
        else
            gosterge +="5";
    }
    else if (stringSayi=="6")
    {
        if((gosterge=="0" && mIslem==YOK) || mIslemVar)
        {
            gosterge = "6";
            mIslemVar = false;
        }
        else
            gosterge +="6";
    }
    else if (stringSayi=="7")
    {
        if((gosterge=="0" && mIslem==YOK) || mIslemVar)
        {
            gosterge = "7";
            mIslemVar = false;
        }
        else
            gosterge +="7";
    }
    else if (stringSayi=="8")
    {
        if((gosterge=="0" && mIslem==YOK) || mIslemVar)
        {
            gosterge = "8";
            mIslemVar = false;
        }
        else
            gosterge +="8";
    }
    else if (stringSayi=="9")
    {
        if((gosterge=="0" && mIslem==YOK) || mIslemVar)
        {
            gosterge = "9";
            mIslemVar = false;
        }
        else
            gosterge +="9";
    }
    else
    {
        if((gosterge=="0" && mIslem==YOK) || mIslemVar)
        {
            gosterge = "0";
            mIslemVar = false;
        }
        else
            gosterge +="0";
    }

    return gosterge;
}

//void HesapMak::ekraniTemizle(){
//
//    if (event.getId() == "+"){
//        std::stringstream gecici(mGosterge->getCaption());
//        if (mSonuc==0){
//            if (mIslem==0){
//                gecici>>mSayi1;
//            }
//            else if (mIslem==1){
//                if (mIslemVar==false) gecici>> mSayi2;
//                mSayi1 = mSayi1+mSayi2;
//                mSayi2 = 0;
//                mGosterge->setCaption(toString(mSayi1));
//            }else if (mIslem>1) {esit();}
//
//        }else {
//            std::stringstream gecici(mGosterge->getCaption());
//            gecici>>mSayi1;
//            mSayi2 = 0;
//            mSonuc = 0;
//
//        }
//        mIslemVar=true;
//        mIslem = 1;
//        mOndalik=0;
//    }
//    if (event.getId() == "="&&mIslem!=0){
//        esit();
//    }
//
//}
