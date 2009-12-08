#include "elektrowidget.h"
#include "./gui/widgets/browserbox.h"
#include "log.h"

ElektroWidget::ElektroWidget()
{
    //ctor
}

ElektroWidget::~ElektroWidget()
{
    //dtor
}

gcn::Label*
ElektroWidget::addLabel(xmlNodePtr node)
{
    xmlNodePtr subnode  = node;
    gcn::Label *templabel=new gcn::Label("");
    templabel->setCaption(XML::getProperty(subnode, "text", "label"));
    templabel->setX(XML::getProperty(subnode, "x", 0)+padX);
    templabel->setY(XML::getProperty(subnode, "y", 0)+padY);
    int w=XML::getProperty(subnode, "width", 0);
    int h=XML::getProperty(subnode, "height", 0);
    if (w==0||h==0)
        templabel->adjustSize();
      else
        templabel->setSize(w,h);

    int r = XML::getProperty(subnode, "fcolorr", 0);
    int g = XML::getProperty(subnode, "fcolorg", 0);
    int b = XML::getProperty(subnode, "fcolorb", 0);
    if (r!=0 && g!=0 && b!=0) templabel->setForegroundColor(gcn::Color(r,g,b));

    r = XML::getProperty(subnode, "bcolorr", 0);
    g = XML::getProperty(subnode, "bcolorg", 0);
    b = XML::getProperty(subnode, "bcolorb", 0);
    if (r!=0 && g!=0 && b!=0) templabel->setBackgroundColor(gcn::Color(r,g,b));

    std::string font = XML::getProperty(subnode, "font", "boldFont");
    if (font=="boldFont") templabel->setFont(boldFont);
     else if (font=="bas_1_20") templabel->setFont(font_bas_1_20);
     else if (font=="bas_1_18") templabel->setFont(font_bas_1_18);
     else if (font=="bas_1_16") templabel->setFont(font_bas_1_16);
     else if (font=="bas_1_14") templabel->setFont(font_bas_1_14);
     else if (font=="bas_2_20") templabel->setFont(font_bas_2_20);
     else if (font=="bas_2_18") templabel->setFont(font_bas_2_18);
     else if (font=="bas_2_16") templabel->setFont(font_bas_2_16);
     else if (font=="bas_2_14") templabel->setFont(font_bas_2_14);
     else if (font=="bas_3_18") templabel->setFont(font_bas_3_18);
     else if (font=="bas_3_16") templabel->setFont(font_bas_3_16);
     else if (font=="bas_3_14") templabel->setFont(font_bas_3_14);
     else if (font=="bas_3_12") templabel->setFont(font_bas_3_12);
     else if (font=="bas_4_16") templabel->setFont(font_bas_4_16);
     else if (font=="bas_4_14") templabel->setFont(font_bas_4_14);
     else if (font=="bas_4_12") templabel->setFont(font_bas_4_12);
     else if (font=="bas_4_10") templabel->setFont(font_bas_4_10);
     else if (font=="bas_4_8") templabel->setFont(font_bas_4_8);
     else if (font=="bas_5") templabel->setFont(font_bas_5);
     else if (font=="bas_b_1_20") templabel->setFont(font_bas_b_1_20);
     else if (font=="bas_b_1_18") templabel->setFont(font_bas_b_1_18);
     else if (font=="bas_b_1_16") templabel->setFont(font_bas_b_1_16);
     else if (font=="bas_b_1_14") templabel->setFont(font_bas_b_1_14);
     else if (font=="bas_b_2") templabel->setFont(font_bas_b_2);
     else if (font=="el_1") templabel->setFont(font_el_1);
     else if (font=="el_2") templabel->setFont(font_el_2);
     else if (font=="el_3") templabel->setFont(font_el_3);
     else if (font=="el_b_1") templabel->setFont(font_el_b_1);
     else if (font=="el_b_2") templabel->setFont(font_el_b_2);
     else if (font=="txt_1_14") templabel->setFont(font_txt_1_14);
     else if (font=="txt_1_12") templabel->setFont(font_txt_1_12);
     else if (font=="txt_1_10") templabel->setFont(font_txt_1_10);
     else if (font=="txt_1_8") templabel->setFont(font_txt_1_8);
     else if (font=="txt_2") templabel->setFont(font_txt_2);
     else if (font=="txt_3") templabel->setFont(font_txt_3);
     else if (font=="txt_4") templabel->setFont(font_txt_4);
     else if (font=="txt_5") templabel->setFont(font_txt_5);
     else if (font=="txt_6") templabel->setFont(font_txt_6);
     else if (font=="txt_b_1") templabel->setFont(font_txt_b_1);
     else if (font=="txt_b_2") templabel->setFont(font_txt_b_2);
     else if (font=="txt_b_3") templabel->setFont(font_txt_b_3);
     else if (font=="txt_cal") templabel->setFont(font_calibri);
     else if (font=="txt_cal_i") templabel->setFont(font_i_calibri);
     else if (font=="txt_cal_b") templabel->setFont(font_b_calibri);
     else if (font=="txt_cal_bi") templabel->setFont(font_b_i_calibri);
     else templabel->setFont(boldFont);

    templabel->setFrameSize(XML::getProperty(subnode, "bordersize", 0));
    templabel->setVisible(true);
    templabel->adjustSize();
    return templabel;
}

SmImage
ElektroWidget::addImage(xmlNodePtr node)
{
    ResourceManager *resman = ResourceManager::getInstance();
    SmImage temp;
    temp.img = resman->getImage(XML::getProperty(node, "src", ""));
    temp.x   =  XML::getProperty(node, "x", 0)+padX;
    temp.y   =  XML::getProperty(node, "y", 0)+padY;
    temp.visible = true;
    return temp;
}

SmAnim
ElektroWidget::addAnim(xmlNodePtr node)
{
    ResourceManager *resman = ResourceManager::getInstance();
    SmAnim temp;
    ImageSet *mImageSet = resman->getImageSet(XML::getProperty(node, "src",""),
                                    XML::getProperty(node, "width", 0),
                                    XML::getProperty(node, "height", 0));

    Animation *mAnimation = new Animation();

    for (unsigned int i = 0; i < mImageSet->size(); ++i)
    {
        mAnimation->addFrame(mImageSet->get(i), 75, 0, 0);
    }
    temp.anim = new SimpleAnimation(mAnimation);
    temp.x   =  XML::getProperty(node, "x", 0)+padX;
    temp.y   =  XML::getProperty(node, "y", 0)+padY;
    temp.v   =  XML::getProperty(node, "v", 0);
    temp.visible = true;
    return temp;
}

SmTextBox
ElektroWidget::addTextBox(gcn::Window *w, xmlNodePtr node)
{
        SmTextBox temp;
        BrowserBox *mBrowserBox = new BrowserBox();
        int h =  XML::getProperty(node, "height", 275);
        int l =  XML::getProperty(node, "x", (800-w->getWidth())/2)+padX;
        int t =  XML::getProperty(node, "y", (600-w->getHeight())/2)+padY;
        int ww =  XML::getProperty(node, "width", 350);

        ScrollArea *mScrollArea = new ScrollArea(mBrowserBox);
        mScrollArea->setDimension(gcn::Rectangle(l, t, ww, h ));

        //gcn::ScrollArea::ScrollPolicy ile sayıyı enum tipine döndürdük
        mScrollArea->setVerticalScrollPolicy(gcn::ScrollArea::ScrollPolicy(XML::getProperty(node, "scrollv",2)));
        mScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::ScrollPolicy(XML::getProperty(node, "scrollh",2)));

        mBrowserBox->setOpaque(XML::getProperty(node, "opaque", false));
        for_each_xml_child_node(subnode, node)
        {
            if (xmlStrEqual(subnode->name, BAD_CAST "addrow"))
            {
                mBrowserBox->addRow(XML::getProperty(subnode, "text", ""));
            }
        }
        mScrollArea->setVerticalScrollAmount(0);
        mScrollArea->setHorizontalScrollAmount(0);
        temp.browserbox = mBrowserBox;
        temp.scrollarea = mScrollArea;
    return temp;
}

SmRadio
ElektroWidget::addRadio(gcn::ActionListener *w, xmlNodePtr node, int selected)
{
    SmRadio temp;
    std::string text = XML::getProperty(node, "text", "");
    int x = XML::getProperty(node, "x", 0)+padX;
    int y = XML::getProperty(node, "y", 0)+padY;

    gcn::RadioButton *mRadio = new RadioButton(text,"1",false);
    mRadio->setPosition(x, y);
    mRadio->setVisible(false);
    mRadio->addActionListener(w);
    const std::string &olayId = "radiocheck";
    mRadio->setActionEventId(olayId);
    temp.value = XML::getProperty(node, "value", 0);
    if (temp.value == selected) mRadio->setSelected(true);
    temp.radio = mRadio;
    return temp;
}

SmCheck
ElektroWidget::addCheck(gcn::ActionListener *w, xmlNodePtr node)
{
    SmCheck temp;
    std::string text = XML::getProperty(node, "text", "");
    int x = XML::getProperty(node, "x", 0)+padX;
    int y = XML::getProperty(node, "y", 0)+padY;

    CheckBox *mCheck = new CheckBox(text,false);
    mCheck->setPosition(x, y);
    mCheck->setVisible(false);

    temp.value = XML::getProperty(node, "value", "2");
    int sayi = XML::getProperty(node, "checked", -1);
    if (sayi==-1)
        mCheck->setSelected(false);
    else
        mCheck->setSelected(true);
    mCheck->addActionListener(w);
    const std::string &olayId = "checkboxcheck"+ temp.value;
    mCheck->setActionEventId(olayId);
    temp.check = mCheck;
    return temp;
}



