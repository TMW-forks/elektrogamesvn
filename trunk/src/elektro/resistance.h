#ifndef RESISTANCE_H
#define RESISTANCE_H

#include "component.h"

#include "../resources/image.h"
#include "../resources/resourcemanager.h"
#include "../resources/imageset.h"
#include "../graphics.h"


class Resistance : public Component
{
    public:
        Resistance(gcn::ActionListener *listener, Node *n1, Node *n2);
        virtual ~Resistance();
        void draw(gcn::Graphics *graphics);
        void nodesCalc();
    protected:
    private:
};

#endif // RESISTANCE_H
