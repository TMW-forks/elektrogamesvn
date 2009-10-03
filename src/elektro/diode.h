#ifndef DIODE_H
#define DIODE_H

#include "component.h"

#include "../resources/image.h"
#include "../resources/resourcemanager.h"
#include "../resources/imageset.h"
#include "../graphics.h"


class Diode : public Component
{
    public:
        Diode(gcn::ActionListener *listener, Node *n1, Node *n2);
        virtual ~Diode();
        void draw(gcn::Graphics *graphics);
    protected:
    private:
};

#endif // DIODE_H
