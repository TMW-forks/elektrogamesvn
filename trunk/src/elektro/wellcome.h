#ifndef _EG_WELLCOME_H
#define _EG_WELLCOME_H

#include <iosfwd>

#include <guichan/actionlistener.hpp>
#include <guichan/widgets/label.hpp>

#include "gui/widgets/window.h"
#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "../resources/image.h"

#include "../guichanfwd.h"
#include "bitbutton.h"

#include "utils/stringutils.h"
#include "utils/gettext.h"

#include "imagewidget.h"
/**
 * The mission window.
 *
 * \ingroup Interface
 */

class Wellcome : public Window, public gcn::ActionListener
{
    public:
        /**
         * Constructor.
         */
        Wellcome();

        /**
         * Draws the button.
         */
        virtual void draw(gcn::Graphics* graphics);

        /**
         * Olay yönetici
         */
        void action(const gcn::ActionEvent &event);

        void mouseExited(gcn::MouseEvent &event);

        void mouseMoved(gcn::MouseEvent &event);

        void mousePressed(gcn::MouseEvent &event);
    private:
        Image *mSlide;
        Button *mNext;
        Button *mPrev;
        Button *mFinish;
        CheckBox *mAgain;

        int mCurrentSlide;

        std::vector<std::string>mList;

        void load();
};
#endif
