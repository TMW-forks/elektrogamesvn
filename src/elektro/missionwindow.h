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
         * Olay y�netici
         */
        void action(const gcn::ActionEvent &event);

        /**
         * B�t�n g�revleri temizle
         */
        void clearMissions();

        /**
         * Ana g�revin alt�na yeni bir g�rev ekle
         */
        void addSubMission(std::string mainname, SmSubMission *one);
        /**
         * Ana g�rev ekle
         */
        void addMainMission(std::string one);

        /**
         * Bir g�revi g�r�n�r hale getir
         */
        void viewOneMission(std::string one);

        /**
         * Mission d��melerini yerle�tir
         */
        void putMissionButtons();

        /**
         * Alt g�revleri yerle�tir
         */
        void putSubMission();

        /**
         * ana g�rev alt g�revler aras�ndaki �izgiler �izer
         */
        void drawTree();

        /**
         * Alt g�revleri gizle
         */
        void hideSubMissions();

    private:
        int sayfa ; // g�sterilecek resmi se�mek i�in
        Image *sayfaImg;
        Image *mBackgroundPattern;
        BitButton *s1;
        TmvButton missionButtons;
        TmiButton missionButtonsIter;

        //Alt G�rev
//        TSubMissions mSubMissions;
        TSubMissionsIter mSubMissionsIter;
        //Ana G�rev
        TMainMissions mMainMission;
        TMainMissionsIter mMainMissionIter;
};
extern MissionWindow *missionWindow;
#endif
