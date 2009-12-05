#ifndef SWITCH_H
#define SWITCH_H

#include "component.h"

#include "../resources/image.h"
#include "../resources/resourcemanager.h"
#include "../resources/imageset.h"
#include "../graphics.h"

class Switch : public Component
{
    public:
        Switch(gcn::ActionListener *listener, Node *n1, Node *n2);
        virtual ~Switch();
        void draw(gcn::Graphics *graphics);
        void mousePressed(gcn::MouseEvent &event);
    protected:
    private:
};

#endif // SWITCH_H
