#ifndef SLAYTWINDOW_H
#define SLAYTWINDOW_H

#include <guichan/widgets/window.hpp>
#include <guichan/actionlistener.hpp>
#include <guichan/graphics.hpp>
#include <guichan/widgets/label.hpp>
#include <libxml/xmlwriter.h>

#include "simpleanimation.h"

#include "../resources/animation.h"
#include "../resources/image.h"
#include "../resources/resourcemanager.h"
#include "../resources/imageset.h"

#include "gui/gui.h"
#include "gui/widgets/window.h"
#include "gui/widgets/button.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/textbox.h"
#include "gui/widgets/radiobutton.h"
#include "gui/widgets/dropdown.h"

#include "utils/xml.h"
#include "utils/stringutils.h"
#include "utils/gettext.h"
#include "./typedef.h"
#include "./bitbutton.h"

enum SlideState {
    SLIDE_MESSAGE_STATE,
    SLIDE_NEW_STATE,
    SLIDE_CLOSE_STATE
};

class SlideListModel : public gcn::ListModel
{
public:
    std::vector<std::string> SLIDE_SIZE;

    virtual ~SlideListModel() { }

    virtual int getNumberOfElements()
    {
        return SLIDE_SIZE.size();
    }

    virtual std::string getElementAt(int i)
    {
        if (i >= getNumberOfElements())
            return _("???");

        return SLIDE_SIZE.at(i);
    }
    virtual void ekle(std::string slaytname)
    {
        SLIDE_SIZE.push_back(slaytname);
    }
    virtual void degistir(std::string slaytname, int yer)
    {
        SLIDE_SIZE.at(yer) = slaytname;
    }
    virtual void temizle()
    {
        SLIDE_SIZE.clear();
    }
};

class SlaytWindow: public Window, public gcn::ActionListener
{
    public:
        SlaytWindow();
        virtual ~SlaytWindow();
        void action(const gcn::ActionEvent &event);
        void draw(gcn::Graphics *graphics);
        void parseXML(std::string mDoc);
        void startCancelPosition();
        void slideStateControl();
        void nextPrevPosition();
        void clearOldSlide();
    protected:
    private:
        int mTotalSlides;
        int mCurrentSlide;

        int padX;
        int padY;
        SlideState slideState;
        SlideListModel *slideListModel;
        Button *mStart;
        Button *mCancel;
        Button *mNext;
        Button *mPrev;
        DropDown *mDropDown;

        xmlDocPtr mxmlDoc;

        TmvLabel mvLabel;
        TmiLabel miLabel;

        TmvImage mvImage;
        TmiImage miImage;

        TmvAnim mvAnim;
        TmiAnim miAnim;

        TmvScrollArea mvScrollArea;
        TmiScrollArea miScrollArea;

        TmvBrowserBox mvTextBox;
        TmiBrowserBox miTextBox;



};

#endif // SLAYTWINDOW_H
