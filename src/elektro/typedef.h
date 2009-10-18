#ifndef _EG_TYPEDEF_H
#define _EG_TYPEDEF_H

#include "node.h"
#include "component.h"
#include "gui/widgets/radiobutton.h"
#include "gui/widgets/checkbox.h"


/**
    Tmv = Vector Type
    Tmi = iterator Type
    Sm   = Struct
 */


//XML'den gelen nesnelerin özelliklerini saklamak için
struct SmImage{
     Image *img;
     int x;
     int y;
     bool visible;
};

struct SmAnim{
     SimpleAnimation *anim;
     int x;
     int y;
     int v;
     bool visible;
};

struct SmRadio{
     gcn::RadioButton *radio;
     int value;
};

struct SmCheck{
     CheckBox *check;
     std::string value;
};

struct SmLabel{
     gcn::Label *label;
     std::string value;
};

//Devre penceresinde iki node arasýndaki baðlantýnýn özelliklerini saklamak için
struct ConnectList {
    Node *firstCon;
    Node *secondCon;
    bool active;
    bool draw;
};


    // Devre penceresinde soru labelleri için
    typedef std::vector< gcn::Label *> TmvLabel;
    typedef std::vector< gcn::Label *>::iterator TmiLabel;

    // Devre penceresinde soru Radiobuttonlar için
    typedef std::vector< SmRadio> TmvRadio;
    typedef std::vector< SmRadio>::iterator TmiRadio;

    // Devre penceresinde soru Radiobuttonlar için
    typedef std::vector< SmCheck> TmvCheck;
    typedef std::vector< SmCheck>::iterator TmiCheck;

    // Devre penceresinde soru resimleri için
    // TImage testwindow.h 'da tanýmlý struct
    typedef std::vector< SmImage> TmvImage;
    typedef std::vector< SmImage>::iterator TmiImage;

    // Devre penceresinde soru animasyonlar için
    // TAnim testwindow.h 'da tanýmlý struct
    typedef std::vector< SmAnim> TmvAnim;
    typedef std::vector< SmAnim>::iterator TmiAnim;

    // Devre penceresinde buttonlar için
    typedef std::vector< Button* > TmvButton;
    typedef std::vector< Button *>::iterator TmiButton;

    // Devre penceresindeki metin kutularý (renkli)
    typedef std::vector< BrowserBox *> TmvBrowserBox;
    typedef std::vector< BrowserBox *>::iterator TmiBrowserBox;

    // Devre penceresindeki metin kutularýnýn kaydýrma çubuklarý için
    typedef std::vector< ScrollArea *> TmvScrollArea;
    typedef std::vector< ScrollArea *>::iterator TmiScrollArea;

    //Componentler için
    typedef std::vector< Component* > TmvComponent;
    typedef std::vector< Component* >::iterator TmiComponent;

    //Baðlantý noktalarý için (node)
    typedef std::vector< Node* > TmvNode;
    typedef std::vector< Node* >::iterator TmiNode;

/****************************************************************************
    Devre Çözümü (Kirchoff) için gerekli tipler
*****************************************************************************/

    //int için vector.  node id'si vs...
    typedef std::vector< int > TmvInt;
    typedef std::vector< int >::iterator TmiInt;

    //hesap kitap için float vector ve iteratorü
    typedef std::vector< double > TmvFloat;
    typedef std::vector< double >::iterator TmiFloat;

    //iki boyutlu int matris
    typedef std::map< int,TmvInt > TmvIntMatris;
    typedef std::map< int,TmvInt >::iterator  TmiIntMatris;

    //iki boyutlu float matris
    typedef std::vector< TmvFloat > TmvFloatMatris;
    typedef std::vector< TmvFloat >::iterator TmiFloatMatris;

    //componentlerden oluþan 2 boyutlu matris (direnç,pil vs)
    typedef std::vector< TmvComponent > TmvComponentMatris;
    typedef std::vector< TmvComponent >::iterator TmiComponentMatris;

    //iki boyutlu matrisin map'i
    typedef std::map< int, TmvComponentMatris > Tmatris;
    typedef std::map< int, TmvComponentMatris >::iterator TmatrisIter;


#endif
