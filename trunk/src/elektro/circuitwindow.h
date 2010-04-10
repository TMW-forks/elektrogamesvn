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
#include "./motor.h"
#include "./ampermetre.h"


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
         void yaz_PilCompMatris();

        /**
         * seçili olan componentleri bırak
         */
        void clearSelections();

        /**
         * Seçili komponentleri hareket ettir --> todo: sadece movable ise
         */
        void moveComponents(int x, int y);

        /**
         * Bir node'un komşu sayısını bul
         */
        int nodeConnectCount(Node *count);

        /**
         * Bir node'un komşu sayısını bul id'ye göre
         */
        int nodeConnectCount(int count);

        /**
         * NPC'den gelen XML'i oku ve devre elemanlarını oluştur
         */
        void circuitFromXML(std::string mDoc);

        /**
         * Ýki nodu birbirine bağla
         */
        void bindingNodes();

        /**
         * Handles link action.
         */
        void handleLink(const std::string& link);

        /**
         * Kullanılmayan bir node id'si bul
         */
        int findEmptyId();

        /**
         * Connect list'te bulunan node'u değiştir
         */
        void swapNode(Node *find, Node *target);

        /**
         * hint penceresindeki açıklamalar için
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

        bool collisionCheck; /**< Çakışma olup olmadığı kontrol edilecek */

        bool nodeCollision;  /**< Hareketli node çakıştı */

        Node *movingNode;
        Node *targetNode;

        std::vector<ConnectList*> conList;
        std::vector<ConnectList*>::iterator conListIter;

        int transItemId;
        gcn::Label *mHint;

    void setRefresh(bool s) {mRefresh = s;}

    /**
     * DistrubureActionEvent katatküllisi
     */

     void distributeOlay(Item *it);

     void mouseMoved(gcn::MouseEvent &event);

    // item bilgisini itemDB'den alabilmek için
    const ItemInfo &getInfo(int id) const { return ItemDB::get(id); }

    // anahtar açılıp kapatıldığında connection durumunu değiştir
    void statusChanged(Component *sw , Status st);

    bool mWireRefresh;

    /**
     * id'si verilen componenti geri döndürür.
     */
    Component* findComponent(int id);

        struct ConditionLocate
        {
            int compId;
            gcn::Rectangle area;
        };

// nesnelerin konması gereken yerler
        std::vector<ConditionLocate *> conLocate;
        std::vector<ConditionLocate *>::iterator conLocateIter;
    private:
        enum CIRCSTATE{
            HEAD_MESSAGE_STATE = 0,
            FOOT_MESSAGE_STATE,
            CIRCUIT_STATE,
            TEST_STATE,
            ENDSTATE
        };

        CIRCSTATE mCircState;
//kontrol edilmesi gerekenler
        struct ConditionCurrent
        {
            int compId;
            float compCurrent;
        };

        struct ConditionLamp
        {
            int compId;
            bool stat;
        };


//yanması gereken lambalar
        std::vector<ConditionLamp *> conLamp;
        std::vector<ConditionLamp *>::iterator conLampIter;

// akım geçmesi gereken node'lar
        std::vector<ConditionCurrent *> conNode;
        std::vector<ConditionCurrent *>::iterator conNodeIter;



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
        bool mNodeCreate;               // Pencerede yeni node oluşturulmasına izin vermek için
        bool mRefresh;                  // devrenin yeniden hesaplanması gerekince true olur

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

        //imleç üzerine gelince popup hint çıkması için
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

        TmvNode mvNode;           //Bağlantı noktası vector
        TmiNode miNode;           //Bağlantı noktası iter

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

//kirşof içinmMessageText

        TmvIntMatris mvConnectedNodeId;     //Node'un bağlantılı olduğu komşularının listesi
        TmiIntMatris miConnectedNodeId;

        TmvIntMatris mvNodeLoop;             //Ýlmek oluşturma ihtimali olan node'ların listesi -map
        TmiIntMatris miNodeLoop;

        TmvIntMatris mvMesh;                 //ilmeklerin listesi ->temizleniyor
        TmiIntMatris miMesh;

        Tmatris resistanceCompDeter;            // dirençlerden oluşan en soldaki matris için
        Tmatris batteryCompDeter;              // pillerden oluşan en sağdaki matris için
        TmvFloat rowResistanceValue;
        TmvFloat batteryValue;
        TmvFloatMatris resistanceMatris;

        std::vector< float > matrisAkim;    // akımlar için sayılardan oluşan vector
        TmvComponentMatris matrisGerilim;    // pillerden oluşan en sağdaki matris <component*>

        std::map <int, std::string> mHale;

    /**
     * Bir node'un verilen mesh'in üyesi olup olmadığını kontrol eder.
     */
    bool isNodeInMesh(Node *n,TmvInt mesh);

    /**
     * Verilen node'un connectionListteki kayıtlı bağlantılarını ve connection'ı siler
     */
    void clearNodeConnection(Node *n);

    /**
     * Pencerenin hangi durumda olduðunu kontrol ederek ayarlama yapar.
     */
    void stateCheck();

    /**
     * efekt yapmak için
     */
    void makeEffect(std::string type,std::string name, std::string ssound);

    /**
     * Üzerinden geçen lambaların parlaklıklarını hesapla ve yak
     */
    void turnonLamps();

    /**
     * Pilin değerini yönüyle birlikte hesapla
     */
    void calculateBatteryValue();

    /**
     * Gönderilen vector içinde o component var mı?
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
     * Bütün lambaları kapat
     */
     void turnoffAllLamp();

    /**
     * Tüm gui elemanlarını sil
     */
     void deleteWidgets();

    /**
     * Tüm vector ve map'leri boşalt
     */
     void trashMeshMem();

    /**
     * Her nod'un bağlantılı komşularını belirler
     */
    void findConnectedNodeId();

    /**
     * komşu nodeların listesini metin kutusuna yazar --debug
     */
    void yaz_ConnectedNodeId();

    /**
     * çevrimlerin listesini metin kutusuna yazar --debug
     */
    void showNodeLoop();

    /**
     * kesinleşmiş düğümlerin listesini metin kutusuna yazar --debug
     */
    void showMesh();

    /**
     * mvNodeLoop map'inin içindeki tekrar eden düğümleri temizler
     */
    void winnowMesh();

    /**
     * ilmek ihtimali olanları bulur arasından ilmek seçimi yapar :mvNodeLoop --> mvMesh
     */
    void addLoopToMesh();

    /**
     * kesinleşmiş düğümlerin listesin kirchoff denklemini yazar
     */
    void makeMatris();

    /**
     * id'si verilen node'un nodelist adresini döndürür
     */
    Node* findNode(int id);

    /**
     * Devrenin analizini yap ve gereken componentlerin status'unu değiştir
     */
    void devreAnaliz();

    /**
     * Kirchhoff için oluşturulan matrisi textbox içine yaz
     */
    void yaz_DirencCompMatris();

    /**
     * Matrisin satır sütun vectorunu bul köşegen haricindeki eleman listesini oluştur
     */
    TmvComponent matrisElemanBul(int satir);

    /**
     * Dirençlerin matrisini göster --
     */
    void yaz_DirencDegerMatris();

    /**
     * Pillerin matrisini göster --
     */
    void yaz_PilDegerMatris();

    /**
     * resistanceCompDeter map'inin içinden satır ve sütuna göre TmvComponent döndürecek
     */
     TmvComponent matrisComponentXY(int satir, int sutun);

    /**
     * mvDugum'in içinde verilen iki node sırayla var mı?
     * @satir :
     * @node1 :
     * @node2 :
     * -1 : ter sırada
     *  0 : yok
     *  1 : var
     */
     int elemanYonKontrol(int satir, int node1, int node2);

     /**
      * mvDugum map'ini yeniden numaralandır
      */
      void dugumListResort();

      /**
       * akımların yönüne göre dirençlerin yönünü belirle
       */
      void direncYonBelirle();

      /**
       * Nodun komşularını mvConnectedNodeId vectörün al
       * @hedef: komşuları bulunacak node
       * @geri : bu noda gelinmesini sağlayan önceki node
       * return: node id'i listesini geri döndürür
       */
      TmvInt getConnectedNodeId(int target, int prev);

     /**
      * Düğüm listesinde componentin her iki node'u da var mı?
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
