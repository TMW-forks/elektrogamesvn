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


//XML'den gelen nesnelerin �zelliklerini saklamak i�in
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

//Devre penceresinde iki node aras�ndaki ba�lant�n�n �zelliklerini saklamak i�in
struct ConnectList {
    Node *firstCon;
    Node *secondCon;
    bool active;
    bool draw;
};


    // Devre penceresinde soru labelleri i�in
    typedef std::vector< gcn::Label *> TmvLabel;
    typedef std::vector< gcn::Label *>::iterator TmiLabel;

    // Devre penceresinde soru Radiobuttonlar i�in
    typedef std::vector< SmRadio> TmvRadio;
    typedef std::vector< SmRadio>::iterator TmiRadio;

    // Devre penceresinde soru Radiobuttonlar i�in
    typedef std::vector< SmCheck> TmvCheck;
    typedef std::vector< SmCheck>::iterator TmiCheck;

    // Devre penceresinde soru resimleri i�in
    // TImage testwindow.h 'da tan�ml� struct
    typedef std::vector< SmImage> TmvImage;
    typedef std::vector< SmImage>::iterator TmiImage;

    // Devre penceresinde soru animasyonlar i�in
    // TAnim testwindow.h 'da tan�ml� struct
    typedef std::vector< SmAnim> TmvAnim;
    typedef std::vector< SmAnim>::iterator TmiAnim;

    // Devre penceresinde buttonlar i�in
    typedef std::vector< Button* > TmvButton;
    typedef std::vector< Button *>::iterator TmiButton;

    // Devre penceresindeki metin kutular� (renkli)
    typedef std::vector< BrowserBox *> TmvBrowserBox;
    typedef std::vector< BrowserBox *>::iterator TmiBrowserBox;

    // Devre penceresindeki metin kutular�n�n kayd�rma �ubuklar� i�in
    typedef std::vector< ScrollArea *> TmvScrollArea;
    typedef std::vector< ScrollArea *>::iterator TmiScrollArea;

    //Componentler i�in
    typedef std::vector< Component* > TmvComponent;
    typedef std::vector< Component* >::iterator TmiComponent;

    //Ba�lant� noktalar� i�in (node)
    typedef std::vector< Node* > TmvNode;
    typedef std::vector< Node* >::iterator TmiNode;

/****************************************************************************
    Devre ��z�m� (Kirchoff) i�in gerekli tipler
*****************************************************************************/

    //int i�in vector.  node id'si vs...
    typedef std::vector< int > TmvInt;
    typedef std::vector< int >::iterator TmiInt;

    //hesap kitap i�in float vector ve iterator�
    typedef std::vector< double > TmvFloat;
    typedef std::vector< double >::iterator TmiFloat;

    //iki boyutlu int matris
    typedef std::map< int,TmvInt > TmvIntMatris;
    typedef std::map< int,TmvInt >::iterator  TmiIntMatris;

    //iki boyutlu float matris
    typedef std::vector< TmvFloat > TmvFloatMatris;
    typedef std::vector< TmvFloat >::iterator TmiFloatMatris;

    //componentlerden olu�an 2 boyutlu matris (diren�,pil vs)
    typedef std::vector< TmvComponent > TmvComponentMatris;
    typedef std::vector< TmvComponent >::iterator TmiComponentMatris;

    //iki boyutlu matrisin map'i
    typedef std::map< int, TmvComponentMatris > Tmatris;
    typedef std::map< int, TmvComponentMatris >::iterator TmatrisIter;


#endif
