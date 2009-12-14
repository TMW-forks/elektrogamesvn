#ifndef _EG_MISSION_WINDOW_H
#define _EG_MISSION_WINDOW_H

#include <iosfwd>

#include <guichan/actionlistener.hpp>
#include <guichan/widgets/label.hpp>

#include "gui/widgets/window.h"
#include "gui/widgets/browserbox.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/textbox.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/progressbar.h"
#include "../resources/image.h"

#include "../guichanfwd.h"
#include "bitbutton.h"
#include "typedef.h"

#include "utils/xml.h"
#include "utils/stringutils.h"
#include "utils/gettext.h"

#include "imagewidget.h"
/**
 * The mission window.
 *
 * \ingroup Interface
 */
struct SmSubMission{
    std::string mainName;
    TextBox *oneTarget;
    ImageWidget *oneImage;
    BrowserBox *oneExplain;
    int oneStatus;
    bool oneVisible;
};

typedef std::vector<SmSubMission*> TSubMissions;
typedef std::vector<SmSubMission*>::iterator TSubMissionsIter;

struct SmMainMission{
    Button *mainButton;
    TSubMissions subMissions;
};

typedef std::map<std::string, SmMainMission*> TMainMissions;
typedef std::map<std::string, SmMainMission*>::iterator TMainMissionsIter;

class MissionWindow : public Window, public gcn::ActionListener
{
    public:
        /**
         * Constructor.
         */
        MissionWindow();

        /**
         * Logic (updates components' size and infos)
         */
        void logic();

        /**
         * Performs action.
         */

        /**
         * Draws the button.
         */
        virtual void draw(gcn::Graphics* graphics);

        /**
         * Olay yönetici
         */
        void action(const gcn::ActionEvent &event);

        /**
         * Bütün görevleri temizle
         */
        void clearMissions();

        /**
         * Ana görevin altýna yeni bir görev ekle
         */
        void addSubMission(std::string mainname, SmSubMission *one);
        /**
         * Ana görev ekle
         */
        void addMainMission(std::string one);

        /**
         * Bir görevi görünür hale getir
         */
        void viewOneMission(std::string one);

        /**
         * Mission düðmelerini yerleþtir
         */
        void putMissionButtons();

        /**
         * Alt görevleri yerleþtir
         */
        void putSubMission();

        /**
         * ana görev alt görevler arasýndaki çizgiler çizer
         */
        void drawTree();

        /**
         * Alt görevleri gizle
         */
        void hideSubMissions();

        /**
         * npchandler tarafýndan gelen verileri parse et
         */
        void parse(std::string mDoc);

        void mousePressed(gcn::MouseEvent &event);

    private:
        int sayfa ; // gösterilecek resmi seçmek için
        Image *mBackgroundPattern;
        Button *s1;
        Container *mContainerSub;
        Container *mContainerMain;
        Container *mContainerExp;
        ScrollArea *mScrollSub;
        ScrollArea *mScrollMain;
        ScrollArea *mScrollExp;
        ProgressBar  *susle;
        bool susleprogress;

        //Alt Görev
//        TSubMissions mSubMissions;
        TSubMissionsIter mSubMissionsIter;
        //Ana Görev
        TMainMissions mMainMission;
        TMainMissionsIter mMainMissionIter;
};
extern MissionWindow *missionWindow;
#endif
