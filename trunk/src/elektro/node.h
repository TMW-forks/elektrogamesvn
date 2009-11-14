#ifndef NODE_H
#define NODE_H

#include <vector>
#include <guichan/graphics.hpp>
#include "../graphics.h"
#include "bitbutton.h"
#include "component.h"
class Component;

class Node : public BitButton
{
    public:

        /**
         * constructor
         */
        Node(const std::string& caption,
             const std::string& hint,
             const std::string &actionEventId,
             gcn::ActionListener *listener);

        /**
         * Destructor.
         */
        virtual ~Node();

        /**
         * Se�ilebilirlik ata
         */
        void setSelectable (bool s) { mSelectable = s; }

        /**
         * Se�ilebilirlik de�erini geri d�nd�r
         */
        bool getSelectable () { return mSelectable; }

        /**
         * Se�
         */
        void setSelected (bool s) { mSelected = s; }

        /**
         * Se�ilili olup olmad���n� d�nd�r
         */
        bool getSelected () { return mSelected; }

        /**
         * ID de�erini ata
         */
        void setId(int id) { mId= id; }

        /**
         * ID de�erini d�nd�r
         */
        int getId() { return mId; }

        /**
         * Serbest hareket de�eri ata
         */
        void setFree(bool f) { mFree = f; }

        /**
         * Serbest hareket de�eri d�nd�r
         */
        int getFree () { return mFree; }

        /**
         * Silinebilirlik de�eri ata
         */
        void setDeletable(bool s) { mDeletable = s; }

        /**
         * Silinebilir mi
         */
        bool getDeletable() { return mDeletable; }

        /**
         * Ba�lant� yapma de�eri ata
         */
        void setToLink(bool s) { mToLink = s; }

        /**
         * Ba�lant� yapma de�eri d�nd�r
         */
        bool getToLink() { return mToLink; }

        /**
         * Ba�lant� kabul etme de�eri
         */
        void setFromLink(bool s) { mFromLink = s; }

        /**
         * Ba�lant� kabulme de�eri d�nd�r
         */
        bool getFromLink() { return mFromLink; }

        /**
         * Hareket Edebilir mi ata
         */
        void setMovable(bool s) { mMovable = s; }

        /**
         * hareket edebilir mi de�eri d�nd�r
         */
        bool getMovable() { return mMovable; }

        /**
         * Hareket Ediyor
         */
        void setMoving(bool s) { mMoving = s; }

        /**
         * hareket ediyor mu
         */
        bool getMoving() { return mMoving; }

        /**
         * Sil
         */
        void setDead(bool s) { mDead = s; }

        /**
         * Silinmi� mi
         */
        bool getDead() { return mDead; }

        /**
         * Yeni node olu�mas�na sebep ol
         */
        void setCreator(bool s) { mCreator = s; }

        /**
         * Yeni node olu�mas�na sebep olan bu mu
         */
        bool getCreator() { return mCreator; }

        /**
         * Node'un �zerindeki ak�m� belirle
         */
        void setCurrent(float s) { mCurrent = s; }

        /**
         * Node'un �zerindeki ak�m� ver
         */
        float getCurrent() { return mCurrent; }

        /**
         * Sa� klik bas�l�
         */
        void setRightClick(bool s) { mRightClick = s; }

        /**
         * Sa� klik bas�l� m�
         */
        bool getRightClick() { return mRightClick; }

        /**
         * Sahip olan component'i ata
         */

        void setOwner(Component *s) { mOwner = s; }

        /**
         * Sahibi olan componenti d�nd�r
         */
        Component* getOwner() { return mOwner; }

        /**
         * Node'u ekrana �iz
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Atama i�lemlerini yap
         */
        void esitle(Node* atanan);

        /**
         * mouse olaylar�n�n y�netimi
         */
        void mousePressed(gcn::MouseEvent& mouseEvent);

        void mouseDragged(gcn::MouseEvent& mouseEvent);

        void mouseReleased(gcn::MouseEvent& mouseEvent);

        void mouseEntered(gcn::MouseEvent &mouseEvent);

        void mouseExited(gcn::MouseEvent &mouseEvent);

        void mouseMoved(gcn::MouseEvent &event);

    protected:
        int mId;                        /**< Her node'a �zel kimlik no */
        bool mSelectable;
        bool mSelected;
        bool mFree;                     /**< Neydi acaba */
        bool mRightClick;
        bool mDead;                     /**< Node logic'de silinecekse */
        bool mCreator;                  /**< Shift ile s�r�klemede �o�altmaya sebep olan node */
        bool mMoving;
        bool mMovable;
        bool mDeletable;
        bool mToLink;
        bool mFromLink;
        float mCurrent;                 /**< Node �zerinden ge�en ak�m */

        Component *mOwner;              /**< Sahibi olan component varsa g�sterir */
        void firstValues();

    private:
};

#endif // NODE_H
