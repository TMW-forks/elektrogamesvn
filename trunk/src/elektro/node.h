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
         * Seçilebilirlik ata
         */
        void setSelectable (bool s) { mSelectable = s; }

        /**
         * Seçilebilirlik deðerini geri döndür
         */
        bool getSelectable () { return mSelectable; }

        /**
         * Seç
         */
        void setSelected (bool s) { mSelected = s; }

        /**
         * Seçilili olup olmadýðýný döndür
         */
        bool getSelected () { return mSelected; }

        /**
         * ID deðerini ata
         */
        void setId(int id) { mId= id; }

        /**
         * ID deðerini döndür
         */
        int getId() { return mId; }

        /**
         * Serbest hareket deðeri ata
         */
        void setFree(bool f) { mFree = f; }

        /**
         * Serbest hareket deðeri döndür
         */
        int getFree () { return mFree; }

        /**
         * Silinebilirlik deðeri ata
         */
        void setDeletable(bool s) { mDeletable = s; }

        /**
         * Silinebilir mi
         */
        bool getDeletable() { return mDeletable; }

        /**
         * Baðlantý yapma deðeri ata
         */
        void setToLink(bool s) { mToLink = s; }

        /**
         * Baðlantý yapma deðeri döndür
         */
        bool getToLink() { return mToLink; }

        /**
         * Baðlantý kabul etme deðeri
         */
        void setFromLink(bool s) { mFromLink = s; }

        /**
         * Baðlantý kabulme deðeri döndür
         */
        bool getFromLink() { return mFromLink; }

        /**
         * Hareket Edebilir mi ata
         */
        void setMovable(bool s) { mMovable = s; }

        /**
         * hareket edebilir mi deðeri döndür
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
         * Silinmiþ mi
         */
        bool getDead() { return mDead; }

        /**
         * Yeni node oluþmasýna sebep ol
         */
        void setCreator(bool s) { mCreator = s; }

        /**
         * Yeni node oluþmasýna sebep olan bu mu
         */
        bool getCreator() { return mCreator; }

        /**
         * Node'un üzerindeki akýmý belirle
         */
        void setCurrent(float s) { mCurrent = s; }

        /**
         * Node'un üzerindeki akýmý ver
         */
        float getCurrent() { return mCurrent; }

        /**
         * Sað klik basýlý
         */
        void setRightClick(bool s) { mRightClick = s; }

        /**
         * Sað klik basýlý mý
         */
        bool getRightClick() { return mRightClick; }

        /**
         * Sahip olan component'i ata
         */

        void setOwner(Component *s) { mOwner = s; }

        /**
         * Sahibi olan componenti döndür
         */
        Component* getOwner() { return mOwner; }

        /**
         * Node'u ekrana çiz
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Atama iþlemlerini yap
         */
        void esitle(Node* atanan);

        /**
         * mouse olaylarýnýn yönetimi
         */
        void mousePressed(gcn::MouseEvent& mouseEvent);

        void mouseDragged(gcn::MouseEvent& mouseEvent);

        void mouseReleased(gcn::MouseEvent& mouseEvent);

        void mouseEntered(gcn::MouseEvent &mouseEvent);

        void mouseExited(gcn::MouseEvent &mouseEvent);

        void mouseMoved(gcn::MouseEvent &event);

    protected:
        int mId;                        /**< Her node'a özel kimlik no */
        bool mSelectable;
        bool mSelected;
        bool mFree;                     /**< Neydi acaba */
        bool mRightClick;
        bool mDead;                     /**< Node logic'de silinecekse */
        bool mCreator;                  /**< Shift ile sürüklemede çoðaltmaya sebep olan node */
        bool mMoving;
        bool mMovable;
        bool mDeletable;
        bool mToLink;
        bool mFromLink;
        float mCurrent;                 /**< Node üzerinden geçen akým */

        Component *mOwner;              /**< Sahibi olan component varsa gösterir */
        void firstValues();

    private:
};

#endif // NODE_H
