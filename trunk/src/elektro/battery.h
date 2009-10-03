#ifndef BATTERY_H
#define BATTERY_H

#include "component.h"

#include "../resources/image.h"
#include "../resources/resourcemanager.h"
#include "../resources/imageset.h"
#include "../graphics.h"


class Battery : public Component
{
    public:
        Battery(gcn::ActionListener *listener, Node *n1, Node *n2);
        virtual ~Battery();
        void draw(gcn::Graphics *graphics);
    protected:
    private:
};

#endif // BATTERY_H
