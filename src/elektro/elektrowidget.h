#ifndef ELEKTROWIDGET_H
#define ELEKTROWIDGET_H
#include <guichan/widgets/window.hpp>
#include <guichan/actionlistener.hpp>
#include <guichan/graphics.hpp>
#include <guichan/widgets/label.hpp>

#include "gui/gui.h"
#include "gui/widgets/window.h"
#include "gui/widgets/button.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/textbox.h"
#include "gui/widgets/radiobutton.h"
#include "gui/widgets/dropdown.h"

#include "utils/xml.h"

#include "typedef.h"

class ElektroWidget
{
    public:
        ElektroWidget();
        virtual ~ElektroWidget();
        gcn::Label* addLabel(xmlNodePtr node);
        SmImage addImage(xmlNodePtr node);
        SmAnim addAnim(xmlNodePtr node);
        SmTextBox addTextBox(gcn::Window *w, xmlNodePtr node);
        SmRadio addRadio(gcn::ActionListener *w, xmlNodePtr node, int selected);
        SmCheck addCheck(gcn::ActionListener *w, xmlNodePtr node);
    protected:
    private:
};

#endif // ELEKTROWIDGET_H
