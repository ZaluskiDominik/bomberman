#include "bomb.h"
#include <QList>
#include <QGraphicsScene>

extern int fieldSize;

extern QList<bomb*> bombs;

bomb::bomb(QPoint pos, const int explosionRange, const QGraphicsItem * const owner, QGraphicsScene* scene)
    :whoseBomb(owner), range(explosionRange)
{
    //add this bomb to list
    bombs.append(this);
    //add the bomb to the scene
    scene->addItem(this);

    setPos(pos);

    explodeStage=0;
    set_bomb_pixmap();

    mark_players_inside();

    //start counting time
    explodeTimer.start(timeToExplode/numBombPixmaps);
    QObject::connect(&explodeTimer, SIGNAL(timeout()), this, SLOT(advanceExplode()));
}

bomb::~bomb()
{
    //remove this bomb from list
    bombs.removeOne(this);
}

void bomb::mark_players_inside()
{
    playersInside=collidingItems(Qt::IntersectsItemBoundingRect);
}

void bomb::set_bomb_pixmap()
{
    setPixmap(QPixmap(":/images/img/bomb/bomb.png").scaled(fieldSize, fieldSize));
}

QPoint bomb::calculate_bomb_pos(QPoint pos)
{
    QPoint resultPos;
    int temp=pos.y()/fieldSize;

    //set y position
    if (pos.y() - (fieldSize * temp)<=fieldSize/2)
        resultPos.setY(temp * fieldSize);
    else
        resultPos.setY((temp + 1) * fieldSize);

    //set x position
    temp=pos.x()/fieldSize;
    if (pos.x() - (fieldSize * temp)<=fieldSize/2)
        resultPos.setX(temp * fieldSize);
    else
        resultPos.setX((temp + 1) * fieldSize);

    return resultPos;
}

void bomb::explode()
{
    explodeTimer.stop();
    emit bombExploded();
}

void bomb::advanceExplode()
{
    explodeStage++;
    if (explodeStage==numBombPixmaps)
    {
        //explosion
        explode();
    }
    else
        //change bomb's pixmap on next
        set_bomb_pixmap();
}
