#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <guichan/widgets/window.hpp>
#include "../resources/imageset.h"

class ImageWidget : public gcn::Window
{
    public:
        ImageWidget(std::string imagename,bool sets = true);
        virtual ~ImageWidget();
        int getMode(){return mMode;}
        void setMode(int m){mMode = m;}
        void mouseDragged(gcn::MouseEvent &event){}
        void mousePressed(gcn::MouseEvent &event){}

    protected:
    private:

        ImageSet *mImageSet;
        int mMode;
        bool mSet;

        void draw(gcn::Graphics* graphics);

};

#endif // IMAGEWIDGET_H
