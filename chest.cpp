#include "chest.h"
#include <QGraphicsScene>
#include <QImage>

extern int fieldSize;
std::unique_ptr<QPixmap[]> chest::explodePixmaps;

chest::chest()
{
    setZValue(1);
    inDestruction=false;
    explodeStage=0;
    setPixmap(QPixmap(":/images/img/fields/chest.png").scaled(fieldSize, fieldSize));
    connect(&timer, SIGNAL(timeout()), this, SLOT(onExplodeTimeout()));
}

void chest::load_pixmaps()
{
    QImage img(":/img/img/chest_explosion.png");
    explodePixmaps=std::make_unique<QPixmap[]>(numPixmaps);
    for (int i=0 ; i<numPixmaps ; i++)
    {
        explodePixmaps[i]=QPixmap::fromImage(img.copy( (i%8)*512, (i/8 + 2) * 512, 512, 512 ).copy(120, 120, 512-240, 512-240));
        explodePixmaps[i]=explodePixmaps[i].scaled(fieldSize, fieldSize);
    }
}

void chest::free_pixmaps()
{
    delete explodePixmaps.release();
}

void chest::explode()
{
    inDestruction=true;
    //create new chest that will be placed under explosion
    prevChest=new QGraphicsPixmapItem(pixmap());
    prevChest->setPos(pos());
    scene()->addItem(prevChest);

    setPixmap(explodePixmaps[explodeStage]);
    timer.start(40);
}

bool chest::in_process_of_destruction()
{
    return inDestruction;
}

void chest::onExplodeTimeout()
{
    if (++explodeStage < numPixmaps)
    {
        //next pixmap
        setPixmap(explodePixmaps[explodeStage]);

        //delete chest lying under explosion
        if (explodeStage==int(numPixmaps*0.7))
        {
            scene()->removeItem(prevChest);
            delete prevChest;
        }
    }
    else
    {
        timer.stop();
        deleteLater();
    }
}
