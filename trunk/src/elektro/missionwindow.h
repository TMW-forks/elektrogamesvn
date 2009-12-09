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
#include "../resources/image.h"

#include "../guichanfwd.h"
#include "bitbutton.h"
#include "typedef.h"

#include "utils/xml.h"
#include "utils/stringutils.h"
#include "utils/gettext.h"
/**
 * The mission window.
 *
 * \ingroup Interface
 */
struct SmSubMission{
    std::string mainName;
    TextBox *oneTarget;
    Image *oneImage;
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


class LeftScrollArea : public gcn::ListModel
{
public:
    std::vector<std::string> SLIDE_SIZE;

    virtual ~LeftScrollArea() { }

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

    private:
        int sayfa ; // gösterilecek resmi seçmek için
        Image *sayfaImg;
        Image *mBackgroundPattern;
        BitButton *s1;
        TmvButton missionButtons;
        TmiButton missionButtonsIter;

        //Alt Görev
//        TSubMissions mSubMissions;
        TSubMissionsIter mSubMissionsIter;
        //Ana Görev
        TMainMissions mMainMission;
        TMainMissionsIter mMainMissionIter;
};
extern MissionWindow *missionWindow;
#endif
