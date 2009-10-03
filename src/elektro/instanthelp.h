#ifndef _TMW_INSTANTHELP_H
#define _TMW_INSTANTHELP_H

#include <iosfwd>

#include <guichan/actionlistener.hpp>

#include "gui/widgets/window.h"
#include "../resources/image.h"

#include "../guichanfwd.h"
#include "bitbutton.h"
/**
 * The chat window.
 *
 * \ingroup Interface
 */
class InstantHelp : public Window, public gcn::ActionListener
{
    public:
        /**
         * Constructor.
         */
        InstantHelp();

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

        void action(const gcn::ActionEvent &event);

    private:
        int sayfa ; // gösterilecek resmi seçmek için
        Image *sayfaImg;
        BitButton *s1;
        BitButton *s2;
        BitButton *s3;
        BitButton *s4;
        BitButton *s5;
};

#endif
