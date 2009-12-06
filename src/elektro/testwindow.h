/*
 * ElektroGame Online
 * En Öðrenceli OYUN!
 *
 * author :Mehmet Emin KORKUSUZ
 * email  :ekorkusuz@gmail.com
 * web    :www.korkusuz.gen.tr
 */

#ifndef _TMW_GUI_TESTWINDOW_H
#define _TMW_GUI_TESTWINDOW_H

#include <iosfwd>
#include <vector>
#include <map>
#include <list>
#include <string>

#include "../resources/animation.h"
#include "../resources/imageset.h"

#include "../simpleanimation.h"
#include "../guichanfwd.h"

#include <guichan/actionlistener.hpp>
#include <guichan/listmodel.hpp>
#include <guichan/widgets/label.hpp>
#include "gui/widgets/button.h"
#include "gui/widgets/textbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/browserbox.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/window.h"


//#include "window_iki.h"

#include "utils/xml.h"
#include <libxml/xmlwriter.h>
#include "gui/gui.h"
#include "typedef.h"

/**
 * The test dialog.
 *
 * \ingroup Interface
 */

enum TestState {
    MESSAGE_STATE,
    ONEQ_START_STATE,
    MANYQ_START_SATATE,
    MANYQ_CONTINUE_STATE,
    ONE_RADIO_STATE,
    ONE_CEHCK_STATE,
    TEST_FINISHED_STATE,
    SHOW_RESULT_STATE
};
class TestDialog : public Window, public gcn::ActionListener
{
    public:
        /**
         * Constructor.
         *
         * @see Window::Window
         */
        TestDialog();
//        ~TestDialog();

        /**
         * draw all widgets
         */
        void
        draw(gcn::Graphics *graphics);

        /**
         * Called when receiving actions from the widgets.
         */
        void
        action(const gcn::ActionEvent &event);

        /**
         * Returns the number of items in the choices list.
         */
        int
        getNumberOfElements();

        /**
         * Returns the name of item number i of the choices list.
         */
        std::string
        getElementAt(int i);

        /**
         * Fills the options list for an NPC dialog.
         *
         * @param itemString A string with the options separated with colons.
         */
        void
        parseItems(const std::string &itemString);

        /**
         * Resets the list by removing all items.
         */
        void
        reset();

        /**
         * set documant.
         */
        void
        setDoc(std::string xmlDoc);

        /**
         * xml parse not items !!!
         */
        void
        parse();

        /**
         * loop
         */
        void
        logic();

        /**
         * start test
         */
        void
        start();

        /**
         * make an effect
         */
        void
        makeEffect(std::string type,std::string name,std::string sound);

        TestState testState;  //test ne durumda

    private:
        // Teste ait genel bilgiler
        int mTotalQuestion;
        int mTotalTime;
        int mSuccessLimit;
        int mPunish;
        int mAward;

        // Mesaj ekraný göstermek için
        bool mMesajMod;  // tüm modlar için ayrý bir enum tanýmla
        BrowserBox *mMessageText;
        ScrollArea *mMessageArea;
        Button *mMessageOk;

        //Baþlangýç ekraný çok sorulu testler için
        Button *mStartOk;
        Button *mStartCancel;
        ScrollArea *mStartScroll;
        BrowserBox *mStartBox;

        //Sonuç ekraný
        ScrollArea *mFinishScroll;
        BrowserBox *mFinishText;
        Button *mFinishClose;

        int mStartQ;
        int mQuestionNumber;
        int mSelected;
        //
        std::string mType;
        static int gecenZaman;
        bool mBasla;
        bool mBitti;

        Image *testinfo;
        Image *mBackgroundPattern;

        std::string mDoc;
        xmlDocPtr mxmlDoc;

        Button *cancelButton;
//        Button *mEvaluate;
        BitButton *mEvaluate;

        gcn::Label *lblAward;
        gcn::Label *lblPunish;
        gcn::Label *lblTotalTime;
        gcn::Label *lblRemainingTime;
        gcn::Label *lblTotalQuestion;
        gcn::Label *lblCurrentQuestion;
        gcn::Label *lblSuccessLimit;


        TmvLabel mvLabel;
        TmiLabel miLabel;

        TmvImage mvImage;
        TmiImage miImage;

        TmvAnim mvAnim;
        TmiAnim miAnim;

        TmvRadio mvRadio;
        TmiRadio miRadio;

        TmvCheck mvCheck;
        TmiCheck miCheck;

        TmvButton mvButton;
        TmiButton miButton;

        TmvScrollArea mvScrollArea;
        TmiScrollArea miScrollArea;

        TmvBrowserBox mvTextBox;
        TmiBrowserBox miTextBox;



        /**
         * Soru numarasý tutan buttonlarý sil
         */
         void deleteQuestionButtons();

        /**
         * Bütün widget yazý vs. gizle
         */
        void hideEverything();

        /**
         * Test ekraný ilk mesaja ait þeyleri gizle
         */
         void hideMessage();

        /**
         * Test ekraný ilk mesaja ait þeyleri göster
         */
         void showMessage();

        /**
         * Test ekraný ilk mesaja ait þeyleri gizle
         */
         void hideStart();

        /**
         * Rapor ekranýna ait þeyleri gizle
         */
         void hideFinish();

        /**
         * Show a question.
         */
        void showQuestion();

        /**
         * Show finish message.
         */
        void showFinishMessage();

        /**
         * Soru düðmelerini yerleþtir.
         */
        void putPlaceButtons();

        /**
         * Teste ait bilgileri gösteren labelleri gizle.
         */
        void hideInfoLabels();
        /**
         * Teste ait bilgileri gösteren labelleri göster.
         */
        void showInfoLabels();

        /**
         * Test bilgi labellerinin yerini ayarla
         */
        void setupPositionInfoLabels();


};

#endif
