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
        void setAgirlik(int agirlik);
        void setID(int id);

        int getID();
        bool getSelected ();
        int getAgirlik();

        void mousePressed(gcn::MouseEvent &event);
        void mouseDragged(gcn::MouseEvent &event);
        void mouseReleased(gcn::MouseEvent &event);
    protected:
    private:
        gcn::ActionListener *mListener;
        int tempX,tempY;
        int mID;
        bool mSelected;
        int mAgirlik;
};

#endif // KUTLE_H
