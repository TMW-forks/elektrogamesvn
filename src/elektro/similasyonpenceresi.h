#ifndef SIMILASYONPENCERESI_H
#define SIMILASYONPENCERESI_H

#include <guichan/widgets/window.hpp>
#include <guichan/actionlistener.hpp>
#include <guichan/graphics.hpp>
#include <guichan/widgets/label.hpp>
#include <libxml/xmlwriter.h>
#include <vector>



#include "../resources/animation.h"
#include "../resources/image.h"
#include "../resources/resourcemanager.h"
#include "../resources/imageset.h"
#include "../resources/iteminfo.h"

#include "gui/gui.h"
#include "gui/widgets/window.h"
#include "gui/widgets/button.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/textbox.h"
#include "gui/widgets/textfield.h"
#include "gui/widgets/radiobutton.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/browserbox.h"

#include "utils/xml.h"
#include "utils/stringutils.h"
#include "utils/gettext.h"
#include "./typedef.h"
#include "./bitbutton.h"

#include "beskilogram.h"
#include "simpleanimation.h"
#include "kaldirac.h"
#include "item.h"
#include <map>



class SimilasyonPenceresi : public Window, public gcn::ActionListener
{
    public:
        SimilasyonPenceresi();
        virtual ~SimilasyonPenceresi();
        void action(const gcn::ActionEvent &event);
        void draw(gcn::Graphics *graphics);
        void nesneleriAyarla();
        void nesneyiAl(Item *it);
        void parseXML(std::string mDoc);
        bool getPencereDurum();
        void clearComponent();
        void kontrolEt();
        int findEmptyID();
        void autoWrap(ScrollArea *textArea,BrowserBox *browserBox,std::string text);
        int farkBul(std::string word,std::string harf1,std::string harf2);

        int kefe1,kefe2,kefe3;
        std::map<int,std::vector<int> > idKefe;
        std::map<int,std::vector<int> >::iterator idKefeIt;
    protected:
    private:
        bool startCancelDurum;
        int toplam;

        Button *mCancel;
        Button *mClose;
        Button *mStart;
        Button *mControl;

        BrowserBox *mSoru;
        ScrollArea *mSoruArea;
        Kutle *nesne;

        std::string mText;
        std::vector<Kutle*> mvKutle;
        std::vector<Kutle*>::iterator miKutle;
        std::vector<Kaldirac*> mvKaldirac;
        std::vector<Kaldirac*>::iterator miKaldirac;
        std::vector<std::string>mvRow;
        std::vector<std::string>::iterator miRow;
        std::map<std::string,std::string> mmTurkce;
        std::map<std::string,std::string>::iterator miTurkce;

        TmvAnim mvAnim;
        TmiAnim miAnim;

        xmlDocPtr mxmlDoc;
        xmlNodePtr rootNode;

        bool nesneDurum;
        bool pencereDurum;
};
#endif // SIMILASYONPENCERESI_H
