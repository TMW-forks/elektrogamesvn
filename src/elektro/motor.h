#ifndef MOTOR_H
#define MOTOR_H

#include "component.h"

#include "../resources/image.h"
#include "../resources/resourcemanager.h"
#include "../resources/imageset.h"
#include "../graphics.h"


class Motor : public Component
{
    public:
        Motor(gcn::ActionListener *listener, Node *n1, Node *n2);
        virtual ~Motor();
        void draw(gcn::Graphics *graphics);
        void logic();
    protected:
    private:
        int picture;
};

#endif // MOTOR_H
