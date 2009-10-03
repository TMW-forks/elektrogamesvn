#include "hesapmak.h"
#include <guichan/widgets/label.hpp>

#include <string>

#include "gui/widgets/button.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/windowcontainer.h"
#include "gui/gui.h"

#include "../localplayer.h"

#include "../utils/dtor.h"
#include "utils/stringutils.h"
#include "elektro/lang_tr.h"

HesapMak::HesapMak():
        Window(LBLCALCULATOR)
{
    setDefaultSize(windowContainer->getWidth() - 255, 25, 230, 200);

    mOndalik =0;
    mSayi1=0;
    mSayi2=0;
    mIslem=YOK;
    mSonuc=0;
    mIslemVar=false;

    s1 = new gcn::Label("1:");
    s1->setPosition(0,0);
    s1->adjustSize();
    add(s1);

    s2 = new gcn::Label("2:");
    s2->setPosition(40,0);
    s2->adjustSize();
    add(s2);

    is = new gcn::Label("I:");
    is->setPosition(80,0);
    is->adjustSize();
    add(is);

    mGosterge = new gcn::Label("0");
    mGosterge->setPosition(20,15);
    mGosterge->setSize(150,25);
    mGosterge->setForegroundColor(gcn::Color(255,255,255));
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

    mButSayiSil= new Button("CE","SayiSil",this);
    mButSayiSil->setPosition(mbutx,mbuty);
    mButSayiSil->setSize(mbutw,mbuth);
    mButSayiSil->setAlignment(gcn::Graphics::CENTER);
    add(mButSayiSil);
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
    delete mButSayiSil;
    delete mButTumSil;
    delete mButGeriSil;
    delete mButIsaret;
    delete mButOndalik;
    delete mGosterge;
}

void HesapMak::action(const gcn::ActionEvent &event) {

}
void HesapMak::esit(){

}

void HesapMak::tumunuTemizle(){

}

void HesapMak::ekraniTemizle(){
    /*
    if (event.getId() == "+"){
        std::stringstream gecici(mGosterge->getCaption());
        if (mSonuc==0){
            if (mIslem==0){
                gecici>>mSayi1;
            }
            else if (mIslem==1){
                if (mIslemVar==false) gecici>> mSayi2;
                mSayi1 = mSayi1+mSayi2;
                mSayi2 = 0;
                mGosterge->setCaption(toString(mSayi1));
            }else if (mIslem>1) {esit();}

        }else {
            std::stringstream gecici(mGosterge->getCaption());
            gecici>>mSayi1;
            mSayi2 = 0;
            mSonuc = 0;

        }
        mIslemVar=true;
        mIslem = 1;
        mOndalik=0;
    }

    if (event.getId() == "-"){
        std::stringstream gecici(mGosterge->getCaption());
        if (mSonuc==0){
            if (mIslem==0){
                gecici>>mSayi1;
            }
            else {
                if (mIslemVar==false) gecici>> mSayi2;
                mSayi1 = mSayi1-mSayi2;
                mSayi2 = 0;
                mGosterge->setCaption(toString(mSayi1));
            }
        }else {
            std::stringstream gecici(mGosterge->getCaption());
            gecici>>mSayi1;
            mSayi2 = 0;
            mSonuc = 0;

        }
        mIslemVar=true;
        mIslem = 2;
        mOndalik=0;
    }

    if (event.getId() == "x"){
        std::stringstream gecici(mGosterge->getCaption());
        if (mSonuc==0){
            if (mIslem==0){
                gecici>>mSayi1;
            }
            else {
                if (mIslemVar==false) gecici>> mSayi2;
                mSayi1 = mSayi1*mSayi2;
                mSayi2 = 1;
                mGosterge->setCaption(toString(mSayi1));
            }
        }else {
            std::stringstream gecici(mGosterge->getCaption());
            gecici>>mSayi1;
            mSayi2 = 1;
            mSonuc = 0;

        }
        mIslemVar=true;
        mIslem = 3;
        mOndalik=0;
    }

    if (event.getId() == "/"){
        std::stringstream gecici(mGosterge->getCaption());
        if (mSonuc==0){
            if (mIslem==0){
                gecici>>mSayi1;
            }
            else {
                if (mIslemVar==false) gecici>> mSayi2;
                mSayi1 = mSayi1/mSayi2;
                mSayi2 = 1;
                mGosterge->setCaption(toString(mSayi1));
            }
        }else {
            std::stringstream gecici(mGosterge->getCaption());
            gecici>>mSayi1;
            mSayi2 = 1;
            mSonuc = 0;

        }
        mIslemVar=true;
        mIslem = 4;
        mOndalik=0;
    }

    if (event.getId() == "="&&mIslem!=0){
        esit();
    }

*/
}
