#ifndef KALDIRAC_H
#define KALDIRAC_H

#include <guichan/actionlistener.hpp>
#include "gui/widgets/window.h"
#include <guichan/graphics.hpp>

#include "../simpleanimation.h"

#include "../resources/animation.h"
#include "../resources/image.h"
#include "../resources/resourcemanager.h"
#include "../resources/imageset.h"

enum HareketYonu
{
    SABIT,
    UP,
    DOWN
};

class Kaldirac : public gcn::Window, public gcn::ActionListener
{
    public:
        Kaldirac(gcn::ActionListener *listener);
        virtual ~Kaldirac();
        void action(const gcn::ActionEvent &event);
        void draw(gcn::Graphics *graphics);
        void logic();
        SimpleAnimation *mSelectedAnime;
        HareketYonu getHareketYonu();
        void setHareketYonu(HareketYonu yon);

    protected:

    private:
        gcn::ActionListener *mListener;
        Image *resim;
        ImageSet *mKaldiracImages;
        Animation *mKaldiracAnime;
        int sayac,resimIndex;
        HareketYonu hareketYonu;

};

#endif // KALDIRAC_H
