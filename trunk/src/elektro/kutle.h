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


class Kutle: public Window, public gcn::ActionListener
{
    public:
        Kutle(gcn::ActionListener *listener);
        virtual ~Kutle();

        void action(const gcn::ActionEvent &event);
        void draw(gcn::Graphics *graphics);
        bool getSelected ();
        void setKoordinat(int x,int y);
        void setW(int w);
        void setH(int h);

        int getW();
        int getH();
        void getKoordinat(int &x,int &y);

        void mousePressed(gcn::MouseEvent &event);
        void mouseDragged(gcn::MouseEvent &event);
        void mouseReleased(gcn::MouseEvent &event);
        void mouseEntered(gcn::MouseEvent &event);
        void mouseExited(gcn::MouseEvent &event);
    protected:
    private:
        gcn::ActionListener *mListener;
        SimpleAnimation *mSelectedAnime;
        int mX;
        int mY;
        int mW;
        int mH;
        static int fareResize;

        bool mSelected;
};

#endif // KUTLE_H
