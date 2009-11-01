#ifndef _TMW_CIRCUITWINDOW_H
#define _TMW_CIRCUITWINDOW_H

#include <guichan/actionlistener.hpp>
#include "window_iki.h"
#include "gui/widgets/button.h"
#include "testwindow.h"
//#include "linkhandler.h"

#include "gui/widgets/browserbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/textbox.h"
#include "resources/animation.h"
#include "resources/imageset.h"
#include "resources/iteminfo.h"
#include "resources/itemdb.h"

#include "./node.h"
#include "./component.h"
#include "./resistance.h"
#include "./diode.h"
#include "./lamp.h"
#include "./switch.h"
#include "./battery.h"
#include "./wire.h"
#include "./typedef.h"

class BrowserBox;
class ScrollArea;
class CircuitWindow : public Window,
                      public gcn::ActionListener
{
    public:
        CircuitWindow();

        ~CircuitWindow();

        void logic();

        void action(const gcn::ActionEvent &event);

        void draw(gcn::Graphics *graphics);

        void mousePressed(gcn::MouseEvent &event);

        /**
         * Pil matrisini yaz
         */
         void showBatteryMatris();

        /**
         * se�ili olan componentleri b�rak
         */
        void clearSelections();

        /**
         * Se�ili komponentleri hareket ettir --> todo: sadece movable ise
         */
        void moveComponents(int x, int y);

        /**
         * Bir node'un kom�u say�s�n� bul
         */
        int nodeConnectCount(Node *count);

        /**
         * Bir node'un kom�u say�s�n� bul id'ye g�re
         */
        int nodeConnectCount(int count);

        /**
         * NPC'den gelen XML'i oku ve devre elemanlar�n� olu�tur
         */
        void circuitFromXML(std::string mDoc);

        /**
         * �ki nodu birbirine ba�la
         */
        void bindingNodes();

        /**
         * Handles link action.
         */
        void handleLink(const std::string& link);

        /**
         * Kullan�lmayan bir node id'si bul
         */
        int findEmptyId();

        /**
         * Connect list'te bulunan node'u de�i�tir
         */
        void swapNode(Node *find, Node *target);

        /**
         * hint penceresindeki a��klamalar i�in
         */
        gcn::Label *toolCaption;
        gcn::Label *toolValue;
        bool toolRotate;
        bool toolMove;
        bool toolFromRight;
        bool toolFromLeft;
        bool toolToRight;
        bool toolToLeft;
        bool toolErase;
        bool toolSelect;

        void setNodeCreate(bool s) { mNodeCreate=s; }
        bool getNodeCreate() { return mNodeCreate; }

        bool collisionCheck; /**< �ak��ma olup olmad��� kontrol edilecek */

        bool nodeCollision;  /**< Hareketli node �ak��t� */

        Node *movingNode;
        Node *targetNode;

        std::vector<ConnectList*> conList;
        std::vector<ConnectList*>::iterator conListIter;

        int transItemId;
        gcn::Label *mHint;

    private:
        BrowserBox *mSb;
        ScrollArea *mSs;

        gcn::Label *mX;
        gcn::Label *mY;

        int mTotalTime;
        int mPunish;
        int mAward;

        bool mNodeCreate;
        bool mRefresh;

        BitButton *closeButton;
        BitButton *solveButton;
        BitButton *clearButton;

        Image *nodeConnectImage;
        Image *cirToolBar;
        Image *cirRotate;
        Image *cirMove;
        Image *cirFromRight;
        Image *cirFromLeft;
        Image *cirToRight;
        Image *cirToLeft;
        Image *cirErase;
        Image *cirSelect;

        int collisionNodeX;
        int collisionNodeY;

        TmvNode mvNode;           //Ba�lant� noktas� vector
        TmiNode miNode;           //Ba�lant� noktas� iter

        TmvComponent mvComponent;       //Devre pencersindeki t�m componentler (diren�, pil ...) i�in vector
        TmiComponent miComponent;       //compnent iter

        TmvLabel mvLabel;           //Label Vector�
        TmiLabel miLabel;           //Label Iteratoru

        TmvImage mvImage;           //TImage Vectoru
        TmiImage miImage;           //TImage Iteratoru

        TmvAnim mvAnim;             //TAnim Vectoru
        TmiAnim miAnim;             //TAnim Iteratoru

        TmvBrowserBox mvBrowserBox;     //BrowserBox vector
        TmiBrowserBox miBrowserBox;     //BrowserBox iter

        TmvScrollArea mvScrollArea;     //ScrollArea vector
        TmiScrollArea miScrollArea;     //ScrollArea iter

//kir�of i�in

        TmvIntMatris mvConnectedNodeId;     //Node'un ba�lant�l� oldu�u kom�ular�n�n listesi
        TmiIntMatris miConnectedNodeId;

        TmvIntMatris mvNodeLoop;             //�lmek olu�turma ihtimali olan node'lar�n listesi -map
        TmiIntMatris miNodeLoop;

        TmvIntMatris mvMesh;                 //ilmeklerin listesi ->temizleniyor
        TmiIntMatris miMesh;


        Tmatris resistanceCompDeter;            // diren�lerden olu�an en soldaki matris i�in
        Tmatris batteryCompDeter;              // pillerden olu�an en sa�daki matris i�in
        TmvFloat rowResistanceValue;
        TmvFloat batteryValue;
        TmvFloatMatris resistanceMatris;

        std::vector< float > matrisAkim;    // ak�mlar i�in say�lardan olu�an vector
        TmvComponentMatris matrisGerilim;    // pillerden olu�an en sa�daki matris <component*>

    /**
     * Pilin de�erini y�n�yle birlikte hesapla
     */
    void calculateBatteryValue();


    /**
     * G�nderilen vector i�inde o component var m�?
     */
    bool isExistComponent(Component *comp, TmvComponent vect);

    /**
     * Component diren� t�rlerinden biri mi
     */
    bool isResistance(Component *comp);


    /**
     * Component pil t�rlerinden biri mi
     */
    bool isBattery(Component *comp);

    /**
     * B�t�n lambalar� kapat
     */
     void turnoffAllLamp();

    /**
     * T�m gui elemanlar�n� sil
     */
     void deleteWidgets();

    /**
     * T�m vector ve map'leri bo�alt
     */
     void trashMeshMem();

    /**
     * Her nod'un ba�lant�l� kom�ular�n� belirler
     */
    void findConnectedNodeId();

    /**
     * kom�u nodelar�n listesini metin kutusuna yazar --debug
     */
    void showConnectedNodeId();

    /**
     * �evrimlerin listesini metin kutusuna yazar --debug
     */
    void showNodeLoop();

    /**
     * kesinle�mi� d���mlerin listesini metin kutusuna yazar --debug
     */
    void showMesh();

    /**
     * mvNodeLoop map'inin i�indeki tekrar eden d���mleri temizler
     */
    void winnowMesh();

    /**
     * ilmek ihtimali olanlar� bulur aras�ndan ilmek se�imi yapar :mvNodeLoop --> mvMesh
     */
    void addLoopToMesh();

    /**
     * kesinle�mi� d���mlerin listesin kirchoff denklemini yazar
     */
    void makeMatris();

    /**
     * id'si verilen node'un nodelist adresini d�nd�r�r
     */
    Node* findNode(int id);

    /**
     * Devrenin analizini yap ve gereken componentlerin status'unu de�i�tir
     */
    void devreAnaliz();

    /**
     * Kirchhoff i�in olu�turulan matrisi textbox i�ine yaz
     */
    void matrisYaz();

    /**
     * Matrisin sat�r s�tun vectorunu bul k��egen haricindeki eleman listesini olu�tur
     */
    TmvComponent matrisElemanBul(int satir);

    /**
     * Diren�lerin matrisini g�ster --
     */
    void direncMatrisYaz();

    /**
     * resistanceCompDeter map'inin i�inden sat�r ve s�tuna g�re TmvComponent d�nd�recek
     */
     TmvComponent matrisComponentXY(int satir, int sutun);

    /**
     * mvDugum'in i�inde verilen iki node s�rayla var m�?
     * @satir :
     * @node1 :
     * @node2 :
     * -1 : ter s�rada
     *  0 : yok
     *  1 : var
     */
     int elemanYonKontrol(int satir, int node1, int node2);

     /**
      * mvDugum map'ini yeniden numaraland�r
      */
      void dugumListResort();

      /**
       * ak�mlar�n y�n�ne g�re direnlerin y�n�n� belirle
       */
      void direncYonBelirle();

      /**
       * Nodun kom�ular�n� mvConnectedNodeId vect�r�n al
       * @hedef: kom�ular� bulunacak node
       * @geri : bu noda gelinmesini sa�layan �nceki node
       * return: node id'i listesini geri d�nd�r�r
       */
      TmvInt getConnectedNodeId(int target, int prev);

      /**
       * Graph : �izgi �iz
      */
      void drawLine(gcn::Graphics *g,int a,int b,int c,int d, int t);

      /**
       * Graph : Daire �iz
       */
      void drawCircle(gcn::Graphics *g, int xc, int yc, int r);

      /**
       * Graph : �izim i�in i�aret +/- bul
       */
      int sgn (long a);

};
extern CircuitWindow *circuitWindow;
#endif
