#ifndef _TMW_HESAPMAK_H
#define _TMW_HESAPMAK_H


#include <guichan/actionlistener.hpp>
#include <guichan/widgets/label.hpp>
#include "gui/widgets/button.h"
#include "gui/widgets/window.h"

enum ISLEMLER {YOK,TOPLAMA,CIKARTMA,CARPMA,BOLME};
class HesapMak : public Window, public gcn::ActionListener
{
    public:
        HesapMak();
        virtual ~HesapMak();
        void action(const gcn::ActionEvent &event);

    protected:
    private:
        gcn::Label *mGosterge;
        Button *mBut0;
        Button *mBut1;
        Button *mBut2;
        Button *mBut3;
        Button *mBut4;
        Button *mBut5;
        Button *mBut6;
        Button *mBut7;
        Button *mBut8;
        Button *mBut9;
        Button *mButTopla;
        Button *mButCikar;
        Button *mButCarp;
        Button *mButBol;
        Button *mButEsit;
        Button *mButSayiSil;
        Button *mButTumSil;
        Button *mButGeriSil;
        Button *mButIsaret;
        Button *mButOndalik;
gcn::Label *s1;
gcn::Label *s2;
gcn::Label *is;
       bool mOndalik;
        bool mIslemVar;
        float mSayi1,mSayi2,mSonuc;
        //int mIslem;
        ISLEMLER mIslem;

                // 0: yok
                // 1: toplama
                // 2: ��kartma
                // 3: �arpma
                // 4: b�lme
void esit();
void tumunuTemizle();
void ekraniTemizle();

};
#endif // _TMW_HESAPMAK_H