#include "kaldirac.h"
#include "similasyonpenceresi.h"

extern SimilasyonPenceresi *similasyonPenceresi;

Kaldirac::Kaldirac(gcn::ActionListener *listener) : mListener(listener)
{
    if (mListener)
    {
        addActionListener(mListener);
    }
    setFrameSize(0);

    ResourceManager *resman = ResourceManager::getInstance();

    mKaldiracImages = resman->getImageSet("graphics/sprites/elektroadd/kaldirac.png",315,128);
    mKaldiracAnime = new Animation();

    for (unsigned int  i=0; i<mKaldiracImages->size(); i++)
        mKaldiracAnime->addFrame(mKaldiracImages->get(i),75,0,0);

    mSelectedAnime = new SimpleAnimation(mKaldiracAnime);

    setX(100);
    setY(225);
    setWidth(350);
    setHeight(200);
    sayac=0;
    resimIndex = 3;
}

Kaldirac::~Kaldirac()
{
    delete resim;
    delete mKaldiracImages;
    delete mKaldiracAnime;
}

void Kaldirac::draw(gcn::Graphics *graphics)
{
    ResourceManager *resman = ResourceManager::getInstance();
    Graphics *g = static_cast<Graphics*>(graphics);

    g->drawImage(mKaldiracImages->get(resimIndex),25, 25);

    drawChildren(graphics);
}

void
Kaldirac::logic()
{
    int a;
    if (similasyonPenceresi->getKontrolEtDurum())
    {
        a = similasyonPenceresi->mvKutle.at(0)->getResimIndex();
        sayac++;
        if(sayac >=50)
        {
            switch(hareketYonu)
            {
                case DOWN:
                    if (resimIndex<=0)
                    {
                        similasyonPenceresi->setKontrolEtDurum(false);
                    }
                    else
                    {
                        a--;
                        resimIndex--;
                        similasyonPenceresi->mvKutle.at(0)->setResimIndex(a);
                        similasyonPenceresi->mvKutle.at(0)->hesaplaY2();
                    }
                    break;

                case UP:
                    if (resimIndex==6)
                    {
                        similasyonPenceresi->setKontrolEtDurum(false);
                    }
                    else
                    {
                        a++;
                        resimIndex++;
                        similasyonPenceresi->mvKutle.at(0)->setResimIndex(a);
                        similasyonPenceresi->mvKutle.at(0)->hesaplaY();
                    }
                    break;

                case SABIT:
                    similasyonPenceresi->setKontrolEtDurum(false);
                    break;
            }

            sayac = 0;
        }
    }
}

void
Kaldirac::action(const gcn::ActionEvent &event)
{

}

HareketYonu
Kaldirac::getHareketYonu()
{
    return hareketYonu;
}

void
Kaldirac::setHareketYonu(HareketYonu yon)
{
    hareketYonu = yon;
}
