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
     SWITCH};
/**
 * Componentin bulunabileceði durumlar
 */
enum Status {
     PASIVE, //Kapalý:Lambalar için sönük, anahtar için açýk
     ACTIVE, //Çalýþýyor:Lambalar için yanýyor, anahtar için kapalý
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
         * Deconstructor
         */
        void setX(int x);

        /**
         * Deconstructor
         */
        void setY(int y);

        /**
         * Deconstructor
         */
        void setW(int w);

        /**
         * Deconstructor
         */
        void setH(int h);

        /**
         * Deconstructor
         */
        int getW();

        /**
         * Deconstructor
         */
        int getH();

        /**
         * Deconstructor
         */
        void setBounce(int x, int y, int w, int h);

        /**
         * Deconstructor
         */
        void setSelectable (bool s);

        /**
         * Deconstructor
         */
        bool getSelectable ();

        /**
         * Deconstructor
         */
        void setSelected (bool s);

        /**
         * Deconstructor
         */
        bool getSelected ();

        /**
         * Deconstructor
         */
        void setType (Type t);

        Type getType ();
        /**
         * Deconstructor
         */

        /**
         * Deconstructor
         */
        void setStatus (Status s);

        void setStatus (int s);

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

        void setAngel(int a);

        int getAngel ();

        /**
         * Nodelarýn yerini hesapla
         */
        void nodesCalc();

        void setValue(float v);

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

        float getCurrent() {return mCurrent; }

        void setCurrent(float s) {mCurrent = s; }

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
        float mValue;
        int mYon;
        float mCurrent;     //Nesne üzerinden geçen akým


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

};

#endif // COMPONENT_H
