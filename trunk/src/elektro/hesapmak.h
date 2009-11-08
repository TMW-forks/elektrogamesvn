#ifndef _TMW_HESAPMAK_H
#define _TMW_HESAPMAK_H

#include <sstream>
#include <string>

#include <guichan/actionlistener.hpp>
#include <guichan/keylistener.hpp>
#include <guichan/selectionlistener.hpp>
#include <guichan/widgets/label.hpp>
#include "gui/widgets/button.h"
#include "gui/widgets/window.h"

enum ISLEMLER {YOK,TOPLAMA,CIKARTMA,CARPMA,BOLME};
class HesapMak : public Window, public gcn::ActionListener,public gcn::KeyListener, public gcn::SelectionListener
{
    public:
        HesapMak();
        virtual ~HesapMak();
        void action(const gcn::ActionEvent &event);
        std::string getString(std::string stringSayi);

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
        bool mOndalik;
        bool mIslemVar;
        float mSayi1,mSayi2,mSonuc;
        std::string mS1;

        ISLEMLER mIslem;

                // 0: yok
                // 1: toplama
                // 2: çýkartma
                // 3: çarpma
                // 4: bölme
};
#endif // _TMW_HESAPMAK_H
