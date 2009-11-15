#ifndef SIMILASYONPENCERESI_H
#define SIMILASYONPENCERESI_H

#include <guichan/widgets/window.hpp>
#include <guichan/actionlistener.hpp>
#include <guichan/graphics.hpp>
#include <guichan/widgets/label.hpp>
//#include <guichan/widgets/textfield.hpp>
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
    protected:
    private:
        bool startCancelDurum;
        Button *mCancel;
        Button *mClose;
        Button *mStart;
        BrowserBox *mSoru;
        ScrollArea *mSoruArea;
        Kutle *nesne;

        std::vector<Kutle*> mvKutle;
        std::vector<Kutle*>::iterator miKutle;

        xmlDocPtr mxmlDoc;
        xmlNodePtr rootNode;

        bool nesneDurum;
        bool pencereDurum;
};
#endif // SIMILASYONPENCERESI_H
