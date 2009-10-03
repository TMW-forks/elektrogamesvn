#ifndef WIRE_H
#define WIRE_H

#include "component.h"

#include "../resources/image.h"
#include "../resources/resourcemanager.h"
#include "../resources/imageset.h"
#include "../graphics.h"


class Wire : public Component
{
    public:
        Wire(gcn::ActionListener *listener);
        virtual ~Wire();
        void draw(gcn::Graphics *graphics);
        void nodesCalc();
    protected:
    private:
};

#endif // WIRE_H
