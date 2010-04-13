#ifndef COMPONENT_H
#define COMPONENT_H

#include <guichan/widgets/window.hpp>
#include <guichan/actionlistener.hpp>
#include <guichan/graphics.hpp>

#include "../simpleanimation.h"

#include "../resources/animation.h"
#include "../resources/image.h"
#include "../resources/resourcemanager.h"
#include "../resources/imageset.h"

#include "bitbutton.h"

#include "node.h"
class Node;
/**
 * Component türü
 */
enum Type {
     UNKNOWN,
     WIRE,
     RESISTANCE,
     BATTERY,
     DIODE,
     LAMP,
     SWITCH,
     MOTOR,
     AMPERMETRE};
/**
 * Componentin bulunabileceği durumlar
 */
enum Status {
     PASIVE = 0, //Kapalı:Lambalar için sönük, anahtar için açık
     ACTIVE, //Çalışıyor:Lambalar için yanıyor, anahtar için kapalı
     BURNED, //Bozuk
     PLUS,   //Parlak
     PLUS2  //Daha Parlak
     };

class Component : public gcn::Window,
                  public gcn::ActionListener
{
    public:
        /**
         * Constructor
         */
        Component(gcn::ActionListener *listener, Node *n1 = NULL, Node *n2 = NULL);

        /**
         * Deconstructor
         */
        virtual ~Component();

        /**
         * sol koordinat
         */
        void setX(int x);

        /**
         * üst koordinat
         */
        void setY(int y);

        /**
         * genişlik
         */
        void setW(int w);

        /**
         * yükseklik
         */
        void setH(int h);

        /**
         * genişlik geri dön
         */
        int getW();

        /**
         * yükseklik geri dön
         */
        int getH();

        /**
         * oluşturan çerçeveyi belirle
         */
        void setBounce(int x, int y, int w, int h);

        /**
         * seçilebilirlik ata
         */
        void setSelectable (bool s);

        /**
         * seçilebilirlik döndür
         */
        bool getSelectable ();

        /**
         * seçili mi?
         */
        void setSelected (bool s);

        /**
         * seçili mi?
         */
        bool getSelected ();

        /**
         * Tipini ata
         */
        void setType (Type t);

        /**
         * Tipini döndür
         */
        Type getType ();


        /**
         * Durumunu ata (active, passive vs)
         */
        void setStatus (Status s);

        /**
         * Durumunu sayı olarak ata
         */
        void setStatus (int s);


        /**
         * Durumunu öğren
         */
        Status getStatus ();

        void setActionListener (gcn::ActionListener *listener);

        void action(const gcn::ActionEvent &event);

        void draw(gcn::Graphics *graphics);

        void mousePressed(gcn::MouseEvent &event);

        void mouseDragged(gcn::MouseEvent &event);

        void mouseReleased(gcn::MouseEvent &event);

        void mouseEntered(gcn::MouseEvent &event);

        void mouseExited(gcn::MouseEvent &event);

        void mouseMoved(gcn::MouseEvent &event);


        /**
         * açıyı ata (a *15 derece)
         */
        void setAngel(int a);

        /**
         * açıyı öğren
         */
        int getAngel ();

        /**
         * Nodeların yerini hesapla
         */
        void nodesCalc();

        /**
        * Item'ın bir değeri varsa ata (direnç ohm, pil V gibi)
        */
        void setValue(float v);

        /**
        * Değeri döndür
        */
        float getValue();

        /**
         * Grafik çizimlei için gerekli
         */
        void drawLine(gcn::Graphics *g,int a,int b,int c,int d, int t);

        void drawCircle(gcn::Graphics *g, int xc, int yc, int r);

        int sgn (long a);

        void setId(int id);

        int getId();

        void setItemId(int id);

        int getItemId();

        void setDeletable(bool s);

        bool getDeletable();

        void setDead(bool s) { mDead=s; }

        bool getDead() {return mDead; }

        void setYon(int s) { mYon=s; }

        int getYon() {return mYon; }

        void setInterval(bool s) { mInterval=s; }

        bool getInterval() {return mInterval; }

        double getCurrent() {return mCurrent; }

        void setCurrent(double s);

        char getParilti() {return mParilti;}


        /**
         * yükseklik
         */
        void setYariCap(int h) {mYariCap = h;}

        /**
         * genişlik geri dön
         */
        int getYariCap() {return mYariCap;}

        Node *node1;
        Node *node2;
        int mX;     /** center */
        int mY;
        int mW;
        int mH;
        int mAngel;
        int mId;
        int mItemId;
        int mFirstX;
        int mFirstY;
        int mInterval;
        double mValue;
        int mYon;
        double mCurrent;     //Nesne üzerinden geçen akım
        int mParilti;      //lambaların parlaklıkları için


        SimpleAnimation *mSelectedAnime;

        Type mType;
        Status mStatus;
        gcn::ActionListener *mListener;

        bool mReturning;
        bool mSelectable;
        bool mSelected;
        bool mNewSelected;
        bool mDeletable;
        bool mDead;
        bool mIsMoving;
    protected:

    private:
        int xx,yy;
        int mYariCap;

};

#endif // COMPONENT_H
