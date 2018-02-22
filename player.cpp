#include "player.h"
#include <QDebug>

extern QList<QGraphicsPixmapItem*> obstacles;
extern QList<QGraphicsPixmapItem*> chests;
extern int fieldSize;

int player::playerSize;

player::player(const playerData& data, QGraphicsScene *scene)
    :QGraphicsPixmapItem()
{
    //import data about player
    keys=data.keys;
    name=data.name;
    color=data.color;

    movingTime=20;

    //add player to scene
    setup_player(scene);

    QObject::connect(&moveTimer, SIGNAL(timeout()), this, SLOT(onMoveTimeout()));
}

void player::move_player(int key)
{
    if (key==keys.up || key==keys.down || key==keys.left || key==keys.right)
    {
        currDir.append(key);
        //start timer if this was first key
        if (currDir.size()==1)
            moveTimer.start(movingTime);

    }
    else if (key==keys.bomb)
    {

    }
}

void player::reset_direction(int key)
{
    if (currDir.removeOne(key))
        if (currDir.empty())
            moveTimer.stop();
}

void player::setup_player(QGraphicsScene* scene)
{
    //resize pixmap
    QPixmap p(":/images/img/" + color + ".png");
    p=p.scaled(playerSize, playerSize);

    //set player's image
    setPixmap(p);

    //set player's position on the map
    int margin=(fieldSize - playerSize)/2 + fieldSize;
    if (color=="red")
        setPos(margin, margin);         //top left
    else if (color=="blue")
        setPos(scene->width() - playerSize - margin, margin);     //top right
    else if (color=="green")
        setPos(margin, scene->height() - playerSize - margin);    //bottom left
    else if (color=="yellow")
        setPos(scene->width() - playerSize - margin, scene->height() - playerSize - margin);    //bottom right
}

void player::change_player_pos(int dir, int dist)
{
    if (dir==keys.up)
        setY(y() - dist);
    else if (dir==keys.down)
        setY(y() + dist);
    else if (dir==keys.left)
        setX(x() - dist);
    else if (dir==keys.right)
        setX(x() + dist);
}

void player::onMoveTimeout()
{
    int distance, dirIter=currDir.size()-1;
    QPoint originPoint(x(), y());
    QList<QGraphicsItem*> collide;

    do
    {
        distance=movingDist;
        do
        {
            //reset player's position to origin
            setPos(originPoint);

            //move player
            change_player_pos(currDir.at(dirIter), distance);

            //remove player from colliding items list
            collide=collidingItems();
            remove_colliding_players(collide);

            //bomb collision
            bomb_collision(collide);

            distance--;
            //check for collisions with blocks
        }
        while (blocks_collision(collide));

        //change direction to next direction in currDir list
        dirIter--;
    }
    while (dirIter>=0 && distance==-1);
}

void player::remove_colliding_players(QList<QGraphicsItem *> &collide)
{
    for (auto i=collide.begin() ; i!=collide.end() ; i++)
        if (typeid(**i)==typeid(player))
            collide.removeOne(*i);
}

void player::bomb_collision(QList<QGraphicsItem *> &collide)
{

}

bool player::blocks_collision(QList<QGraphicsItem *>& collide)
{
    //if player entered brick frame or collide list isn't empty --> there was collision with a block
    if ( (y()<fieldSize) || (y()>scene()->width() - fieldSize -playerSize) || (x()<fieldSize) || (x()>scene()->height() - fieldSize - playerSize) || (collide.size()) )
        return true;

    //there wasn't any collision
    return false;
}
