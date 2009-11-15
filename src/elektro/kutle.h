#ifndef KUTLE_H
#define KUTLE_H

#include <guichan/actionlistener.hpp>
#include "gui/widgets/window.h"
#include <guichan/graphics.hpp>

#include "../simpleanimation.h"

#include "../resources/animation.h"
#include "../resources/image.h"
#include "../resources/resourcemanager.h"
#include "../resources/imageset.h"


class Kutle : public gcn::Window, public gcn::ActionListener
{
    public:
        Kutle(gcn::ActionListener *listener);
        virtual ~Kutle();

        void action(const gcn::ActionEvent &event);
        void draw(gcn::Graphics *graphics);
        void setSelected (bool durum);
        bool getSelected ();

        void mousePressed(gcn::MouseEvent &event);
        void mouseDragged(gcn::MouseEvent &event);
        void mouseReleased(gcn::MouseEvent &event);
        void mouseEntered(gcn::MouseEvent &event);
        void mouseExited(gcn::MouseEvent &event);
    protected:
    private:
        gcn::ActionListener *mListener;
        int tempX;
        int tempY;
        int mX;
        int mY;
        int mW;
        int mH;

        bool mSelected;
};

#endif // KUTLE_H
