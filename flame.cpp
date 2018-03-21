#include "flame.h"

extern int fieldSize;

flame::flame()
{
    timeCounter=animationStage=0;

    setup_pixmaps();

    setPixmap(pixmaps[0]);

    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer.start(timeoutPeriod);
}

void flame::setup_pixmaps()
{
    for (int i=0 ; i<numPixmaps ; i++)
    {
        pixmaps[i].load(":/images/img/Flame/Flame_F0" + QString::number(i) + ".png");
        pixmaps[i]=pixmaps[i].scaled(fieldSize, fieldSize);
    }
}

void flame::onTimeout()
{
    //next pixmap
    animationStage=(animationStage + 1) % numPixmaps;
    setPixmap(pixmaps[animationStage]);

    timeCounter+=timeoutPeriod;
    //if this flame's time of existance is up then delete this flame
    if (timeCounter>=existTime)
    {
        timer.stop();
        deleteLater();
    }
}
