#include "bomb.h"
#include <QList>
#include <QGraphicsScene>

extern int fieldSize;

//lists with bombs placed currently at the scene
QList<bomb*> bombs;

bomb::bomb(QPoint pos, QGraphicsScene* scene)
{
    calculate_bomb_pos(pos);
    explodeStage=0;

    //check if a bomb is already placed on that position, return and don't create a new one
    for (auto i=bombs.begin() ; i!=bombs.end() ; i++)
        if ((*i)->x()==this->x() && (*i)->y()==this->y())
        {
            deleteLater();
            return;
        }

    set_bomb_pixmap();

    //add this bomb to list
    bombs.append(this);

    //add the bomb to the scene
    scene->addItem(this);

    mark_players_inside();

    //start counting time
    explodeTimer.start(timeToExplode/numBombPixmaps);
    QObject::connect(&explodeTimer, SIGNAL(timeout()), this, SLOT(advance_explode()));
}

bomb::~bomb()
{
    //remove this bomb from list
    bombs.removeOne(this);
}

void bomb::mark_players_inside()
{
    playersInsideShape=collidingItems(Qt::IntersectsItemBoundingRect);
}

void bomb::set_bomb_pixmap()
{
    setPixmap(QPixmap(":/images/img/bomb/bomb.png").scaled(fieldSize, fieldSize));
}

void bomb::calculate_bomb_pos(QPoint pos)
{
    int temp=pos.y()/fieldSize;

    //set y position
    if (pos.y() - (fieldSize * temp)<=fieldSize/2)
        setY(temp * fieldSize);
    else
        setY((temp + 1) * fieldSize);

    //set x position
    temp=pos.x()/fieldSize;
    if (pos.x() - (fieldSize * temp)<=fieldSize/2)
        setX(temp * fieldSize);
    else
        setX((temp + 1) * fieldSize);
}

void bomb::explode()
{
    //create flame in bomb's row


    //remove the bomb
    scene()->removeItem(this);
    this->deleteLater();
}

void bomb::advance_explode()
{
    //change bomb's pixmap on next
    explodeStage++;
    set_bomb_pixmap();
    if (explodeStage==numBombPixmaps)
        //explosion
        explode();
}
