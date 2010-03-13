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

    mKaldiracImages = resman->getImageSet("graphics/elektrik/kaldirac.png",315,128);
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
    int index;
    if (similasyonPenceresi->getKontrolEtDurum())
    {
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
                        resimIndex--;
                        for (std::vector<Kutle*>::iterator it= similasyonPenceresi->mvKutle.begin();
                            it != similasyonPenceresi->mvKutle.end(); it++)
                        {
                            //logger->log("döngüde");
                            if ((*it)->getHareket())
                            {
                                index = (*it)->getResimIndex();
                                index--;
                                (*it)->setResimIndex(index);
                                (*it)->hesaplaY();
                            }
                        }
                    }
                    break;

                case UP:
                    if (resimIndex>=6)
                    {
                        similasyonPenceresi->setKontrolEtDurum(false);
                    }
                    else
                    {
                        resimIndex++;
                        for (std::vector<Kutle*>::iterator it = similasyonPenceresi->mvKutle.begin();
                            it != similasyonPenceresi->mvKutle.end(); it++)
                        {
                            if ((*it)->getHareket())
                            {
                                //logger->log("döngüde");
                                index = (*it)->getResimIndex();
                                index++;
                                (*it)->setResimIndex(index);
                                (*it)->hesaplaY();
                            }
                        }
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
