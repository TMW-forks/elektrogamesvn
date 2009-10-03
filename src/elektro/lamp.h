#ifndef LAMP_H
#define LAMP_H

#include "component.h"


#include "../resources/image.h"
#include "../resources/resourcemanager.h"
#include "../resources/imageset.h"
#include "../graphics.h"

class Lamp : public Component
{
    public:
        Lamp(gcn::ActionListener *listener, Node *n1, Node *n2);
        virtual ~Lamp();
        void draw(gcn::Graphics *graphics);
    protected:
    private:
};

#endif // LAMP_H
