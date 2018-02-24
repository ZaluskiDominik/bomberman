#include "player.h"
#include <QDebug>

extern QList<QGraphicsPixmapItem*> obstacles;
extern QList<QGraphicsPixmapItem*> chests;
extern int fieldSize;

int player::playerSize;

player::player(const playerData& data, QGraphicsScene *scene)
    :numPixmaps(8)
{
    //import data about player
    keys=data.keys;
    name=data.name;
    color=data.color;

    movingTime=20;
    moveStage=0;

    //load player pixmaps
    setup_pixmaps();

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
    set_player_pixmap(keys.down);

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

void player::setup_pixmaps()
{
    //front of the player
    for (int i=0 ; i<numPixmaps ; i++)
    {
        playerFront[i].load(":/images/img/players/white/Front/Bman_F_f0" + QString::number(i) + ".png");
        playerFront[i]=playerFront[i].scaled(playerSize, playerSize);
    }

    //back of the player
    for (int i=0 ; i<numPixmaps ; i++)
    {
        playerBack[i].load(":/images/img/players/white/Back/Bman_B_f0" + QString::number(i) + ".png");
        playerBack[i]=playerBack[i].scaled(playerSize, playerSize);
    }

    //side of the player(player look in right direction)
    for (int i=0 ; i<numPixmaps ; i++)
    {
        playerSide[i].load(":/images/img/players/white/Side/Bman_F_f0" + QString::number(i) + ".png");
        playerSide[i]=playerSide[i].scaled(playerSize, playerSize);
    }
}

void player::set_player_pixmap(int dir)
{
    setTransform(QTransform());
    if (dir==keys.up)
        setPixmap(playerBack[moveStage]);
    else if (dir==keys.down)
        setPixmap(playerFront[moveStage]);
    else
    {
        setPixmap(playerSide[moveStage]);
        if (dir==keys.left)
            //rotate pixmap, player will be looking in left direction
            setTransform(QTransform().scale(-1, 1).translate(-playerSize, 0));
    }
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
            collide=collidingItems(Qt::IntersectsItemBoundingRect);
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

    dirIter++;
    if (distance!=-1)
    {
        //no collision
        moveStage=(moveStage + 1) % numPixmaps;
        set_player_pixmap(currDir[dirIter]);
    }
    else
    {
        //collision
        moveStage=0;
    }
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
