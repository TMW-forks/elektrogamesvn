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
         * seçili olan componentleri býrak
         */
        void clearSelections();

        /**
         * Seçili komponentleri hareket ettir --> todo: sadece movable ise
         */
        void moveComponents(int x, int y);

        /**
         * Bir node'un komþu sayýsýný bul
         */
        int nodeConnectCount(Node *count);

        /**
         * Bir node'un komþu sayýsýný bul id'ye göre
         */
        int nodeConnectCount(int count);

        /**
         * NPC'den gelen XML'i oku ve devre elemanlarýný oluþtur
         */
        void circuitFromXML(std::string mDoc);

        /**
         * Ýki nodu birbirine baðla
         */
        void bindingNodes();

        /**
         * Handles link action.
         */
        void handleLink(const std::string& link);

        /**
         * Kullanýlmayan bir node id'si bul
         */
        int findEmptyId();

        /**
         * Connect list'te bulunan node'u deðiþtir
         */
        void swapNode(Node *find, Node *target);

        /**
         * hint penceresindeki açýklamalar için
         */

        std::map<std::string, ImageSet*>mComponentImageSet;

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

        bool collisionCheck; /**< Çakýþma olup olmadýðý kontrol edilecek */

        bool nodeCollision;  /**< Hareketli node çakýþtý */

        Node *movingNode;
        Node *targetNode;

        std::vector<ConnectList*> conList;
        std::vector<ConnectList*>::iterator conListIter;

        int transItemId;
        gcn::Label *mHint;

    /**
     * DistrubureActionEvent katatküllisi
     */

     void distributeOlay(Item *it);

     void mouseMoved(gcn::MouseEvent &event);

    // item bilgisini itemDB'den alabilmek için
    const ItemInfo &getInfo(int id) const { return ItemDB::get(id); }

    // anahtar açýlýp kapatýldýðýnda connection durumunu deðiþtir
    void statusChanged(Component *sw , Status st);

    bool mWireRefresh;

    private:
        enum CIRCSTATE{
            HEAD_MESSAGE_STATE = 0,
            FOOT_MESSAGE_STATE,
            CIRCUIT_STATE,
            TEST_STATE,
            ENDSTATE
        };

        CIRCSTATE mCircState;

        std::vector<int> conLamp;
        std::vector<int>::iterator conLampIter;

        //kullanılan itemları silecek vector
        std::vector < Item *> mUsedItem;
        std::vector < Item *>::iterator mIterUsedItem;

        //tellerin çizilmesi için vektör
        typedef std::vector < SmPoint *> TTekTel;
        typedef std::vector < SmPoint *>::iterator TTekTelIter;

//        TTekTel mTekTel;
        TTekTelIter mTekTelIter;

        std::vector<TTekTel> mTeller;
        std::vector<TTekTel>::iterator mTellerIter;

        BrowserBox *mSb;
        ScrollArea *mSs;

        BrowserBox *mMessageText;
        ScrollArea *mMessageScroll;

        Button *mStartOk;
        Button *mStartCancel;
        Button *mFootOk;

        gcn::Label *mX;
        gcn::Label *mY;

        int mTotalTime;
        int mPunish;
        int mAward;

        bool mMessageAutoWrap;
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
        Image *cirRotateG;
        Image *cirMoveG;
        Image *cirFromRightG;
        Image *cirFromLeftG;
        Image *cirToRightG;
        Image *cirToLeftG;
        Image *cirEraseG;
        Image *cirSelectG;
        Image *mWireImage;
        Image *mBackgroundPattern;

        //imleç üzerine gelince popup hint çýkmasý için
        gcn::Rectangle rectRotate;
        gcn::Rectangle rectMove;
        gcn::Rectangle rectFromRight;
        gcn::Rectangle rectFromLeft;
        gcn::Rectangle rectErase;
        gcn::Rectangle rectSelect;
        gcn::Rectangle rectToRight;
        gcn::Rectangle rectToLeft;

        TextBox *mPopupLabel;

        int collisionNodeX;
        int collisionNodeY;

        TmvNode mvNode;           //Baðlantý noktasý vector
        TmiNode miNode;           //Baðlantý noktasý iter

        TmvComponent mvComponent;       //Devre pencersindeki tüm componentler (direnç, pil ...) için vector
        TmiComponent miComponent;       //compnent iter

        TmvLabel mvLabel;           //Label Vectorü
        TmiLabel miLabel;           //Label Iteratoru

        TmvImage mvImage;           //TImage Vectoru
        TmiImage miImage;           //TImage Iteratoru

        TmvAnim mvAnim;             //TAnim Vectoru
        TmiAnim miAnim;             //TAnim Iteratoru

        TmvBrowserBox mvBrowserBox;     //BrowserBox vector
        TmiBrowserBox miBrowserBox;     //BrowserBox iter

        TmvScrollArea mvScrollArea;     //ScrollArea vector
        TmiScrollArea miScrollArea;     //ScrollArea iter

//kirþof içinmMessageText

        TmvIntMatris mvConnectedNodeId;     //Node'un baðlantýlý olduðu komþularýnýn listesi
        TmiIntMatris miConnectedNodeId;

        TmvIntMatris mvNodeLoop;             //Ýlmek oluþturma ihtimali olan node'larýn listesi -map
        TmiIntMatris miNodeLoop;

        TmvIntMatris mvMesh;                 //ilmeklerin listesi ->temizleniyor
        TmiIntMatris miMesh;

        Tmatris resistanceCompDeter;            // dirençlerden oluþan en soldaki matris için
        Tmatris batteryCompDeter;              // pillerden oluþan en saðdaki matris için
        TmvFloat rowResistanceValue;
        TmvFloat batteryValue;
        TmvFloatMatris resistanceMatris;

        std::vector< float > matrisAkim;    // akýmlar için sayýlardan oluþan vector
        TmvComponentMatris matrisGerilim;    // pillerden oluþan en saðdaki matris <component*>



    /**
     * Pencerenin hangi durumda olduðunu kontrol ederek ayarlama yapar.
     */
    void stateCheck();

    /**
     * efekt yapmak için
     */
    void makeEffect(std::string type,std::string name, std::string ssound);

    /**
     * id'si verilen componenti geri döndürür.
     */
    Component* findComponent(int id);

    /**
     * Üzerinden geçen lambalarýn parlaklýklarýný hesapla ve yak
     */
    void turnonLamps();

    /**
     * Pilin deðerini yönüyle birlikte hesapla
     */
    void calculateBatteryValue();

    /**
     * Gönderilen vector içinde o component var mý?
     */
    bool isExistComponent(Component *comp, TmvComponent vect);

    /**
     * Component direnç türlerinden biri mi
     */
    bool isResistance(Component *comp);

    /**
     * Component pil türlerinden biri mi
     */
    bool isBattery(Component *comp);

    /**
     * Component lamba türlerinden biri mi
     */
    bool isLamp(Component *comp);

    /**
     * Bütün lambalarý kapat
     */
     void turnoffAllLamp();

    /**
     * Tüm gui elemanlarýný sil
     */
     void deleteWidgets();

    /**
     * Tüm vector ve map'leri boþalt
     */
     void trashMeshMem();

    /**
     * Her nod'un baðlantýlý komþularýný belirler
     */
    void findConnectedNodeId();

    /**
     * komþu nodelarýn listesini metin kutusuna yazar --debug
     */
    void showConnectedNodeId();

    /**
     * çevrimlerin listesini metin kutusuna yazar --debug
     */
    void showNodeLoop();

    /**
     * kesinleþmiþ düðümlerin listesini metin kutusuna yazar --debug
     */
    void showMesh();

    /**
     * mvNodeLoop map'inin içindeki tekrar eden düðümleri temizler
     */
    void winnowMesh();

    /**
     * ilmek ihtimali olanlarý bulur arasýndan ilmek seçimi yapar :mvNodeLoop --> mvMesh
     */
    void addLoopToMesh();

    /**
     * kesinleþmiþ düðümlerin listesin kirchoff denklemini yazar
     */
    void makeMatris();

    /**
     * id'si verilen node'un nodelist adresini döndürür
     */
    Node* findNode(int id);

    /**
     * Devrenin analizini yap ve gereken componentlerin status'unu deðiþtir
     */
    void devreAnaliz();

    /**
     * Kirchhoff için oluþturulan matrisi textbox içine yaz
     */
    void matrisYaz();

    /**
     * Matrisin satýr sütun vectorunu bul köþegen haricindeki eleman listesini oluþtur
     */
    TmvComponent matrisElemanBul(int satir);

    /**
     * Dirençlerin matrisini göster --
     */
    void direncMatrisYaz();

    /**
     * resistanceCompDeter map'inin içinden satýr ve sütuna göre TmvComponent döndürecek
     */
     TmvComponent matrisComponentXY(int satir, int sutun);

    /**
     * mvDugum'in içinde verilen iki node sýrayla var mý?
     * @satir :
     * @node1 :
     * @node2 :
     * -1 : ter sýrada
     *  0 : yok
     *  1 : var
     */
     int elemanYonKontrol(int satir, int node1, int node2);

     /**
      * mvDugum map'ini yeniden numaralandır
      */
      void dugumListResort();

      /**
       * akýmlarýn yönüne göre dirençlerin yönünü belirle
       */
      void direncYonBelirle();

      /**
       * Nodun komşularını mvConnectedNodeId vectörün al
       * @hedef: komþuları bulunacak node
       * @geri : bu noda gelinmesini saðlayan önceki node
       * return: node id'i listesini geri döndürür
       */
      TmvInt getConnectedNodeId(int target, int prev);

     /**
      * Düðüm listesinde componentin her iki node'u da var mı?
      * @component: kontrol edilecek component
      * @liste    :
      */
      bool hasDoubleNode(Component *comp,TmvInt mesh);

      /**
       * Graph : Çizgi çiz
      */
      void drawLine(gcn::Graphics *g,int a,int b,int c,int d, int t);

      /**
       * Graph : Daire Çiz
       */
      void drawCircle(gcn::Graphics *g, int xc, int yc, int r);

      /**
       * Graph : Çizim için işaret +/- bul
       */
      int sgn (long a);

      /**
       * Inventoryde için kullanılan item'ların sunucuya iletilmesi
       */
      void sendUsedItem ();
};
extern CircuitWindow *circuitWindow;
#endif
