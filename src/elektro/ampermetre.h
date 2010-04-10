#ifndef AMPERMETRE_H
#define AMPERMETRE_H

#include "component.h"

#include "../resources/image.h"
#include "../resources/resourcemanager.h"
#include "../resources/imageset.h"
#include "../graphics.h"


class Ampermetre : public Component
{
    public:
        Ampermetre(gcn::ActionListener *listener, Node *n1, Node *n2);
        virtual ~Ampermetre();
        void draw(gcn::Graphics *graphics);
        void setCurrent(double s);
    protected:
    private:
        gcn::Label *mLabel;

};

#endif // AMPERMETRE_H
