#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <guichan/widgets/window.hpp>
#include "../resources/imageset.h"

class ImageWidget : public gcn::Window
{
    public:
        ImageWidget(std::string imagename);
        virtual ~ImageWidget();
        int getMode(){return mMode;}
        void setMode(int m){mMode = m;}
    protected:
    private:

        ImageSet *mImageSet;
        int mMode;

        void draw(gcn::Graphics* graphics);

};

#endif // IMAGEWIDGET_H
