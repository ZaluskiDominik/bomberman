#include "player.h"

extern QList<QGraphicsPixmapItem*> obstacles;
extern QList<QGraphicsPixmapItem*> chests;
extern QList<bomb*> bombs;
extern int fieldSize;

int player::playerSize;

player::player(const playerData& data, QGraphicsScene *scene)
    :numPixmaps(8)
{
    //import data about player, keys setting
    keys=data.keys;
    name=data.name;
    color=data.color;
    lifes=2;
    maxNumBombs=1;
    bombsPlaced=0;
    bombPushInterv=20;

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
        //start timer if this was a first key
        if (currDir.size()==1)
            moveTimer.start(movingTime);

    }
    else if (key==keys.bomb)
        //create bomb
        new bomb(pos().toPoint(), scene());
}

void player::reset_direction(int key)
{
    //if currDir is empty after removing released key direction
    if (currDir.removeOne(key))
        if (currDir.empty())
        {
            //stop moving, reset moving stage
            moveTimer.stop();
            moveStage=0;
            set_player_pixmap(key);
        }
}

void player::setup_player(QGraphicsScene* scene)
{
    //set player's position on the map
    int margin=(fieldSize - playerSize)/2 + fieldSize;
    if (color=="red")
    {
        setPos(margin, margin);         //top left
        set_player_pixmap(keys.down);
    }
    else if (color=="blue")
    {
        setPos(scene->width() - playerSize - margin, margin);     //top right
        set_player_pixmap(keys.down);
    }
    else if (color=="green")
    {
        setPos(margin, scene->height() - playerSize - margin);    //bottom left
        set_player_pixmap(keys.up);
    }
    else if (color=="yellow")
    {
        setPos(scene->width() - playerSize - margin, scene->height() - playerSize - margin);    //bottom right
        set_player_pixmap(keys.up);
    }
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
        setPixmap(playerBack[moveStage]);       //player's back
    else if (dir==keys.down)
        setPixmap(playerFront[moveStage]);      //player's front
    else
    {
        setPixmap(playerSide[moveStage]);       //player's side
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

            //remove players from colliding items list
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
        //advance in move animation
        moveStage=(moveStage + 1) % numPixmaps;
        set_player_pixmap(currDir[dirIter]);

        //check if plaer didn't leave bomb's shape
        left_bomb_shape();
    }
    else
        //collision
        moveStage=0;
}

void player::remove_colliding_players(QList<QGraphicsItem *> &collide)
{
    for (auto i=collide.begin() ; i!=collide.end() ; i++)
        if (typeid(**i)==typeid(player))
            collide.removeOne(*i);
}

void player::bomb_collision(QList<QGraphicsItem *> &collide)
{
    QList<bomb*>::iterator collidingBomb;
    for (auto i=collide.begin() ; i!=collide.end() ; i++)
    {
        //it's a bomb
        if (typeid(**i)==typeid(bomb))
        {
            //find that bomb in bombs list
            for (collidingBomb=bombs.begin() ; (*collidingBomb)!=(*i) ; collidingBomb++);

            //if playersInsideShape list is not empty
            if ((*collidingBomb)->playersInsideShape.empty()==false)
            {
                //bomb isn't pushable
                QList<QGraphicsItem*>::iterator j;
                for (j=(*collidingBomb)->playersInsideShape.begin() ; j!=(*collidingBomb)->playersInsideShape.end() && (*j)!=this ; j++);
                if (j != (*collidingBomb)->playersInsideShape.end())
                {
                    //bomb is not collideable for this player
                    //remove that bomb from collide list
                    collide.removeOne(*i);
                }
            }
            else
            {

            }
        }
    }
}

void player::left_bomb_shape()
{
    auto collide=collidingItems(Qt::IntersectsItemBoundingRect);
    for (auto i=bombs.begin() ; i!=bombs.end() ; i++)
        if (!collide.contains(*i))  //player isn't colliding with that bomb
            //remove if player exists in playerInsideList
            (*i)->playersInsideShape.removeOne(this);
}

bool player::blocks_collision(QList<QGraphicsItem *>& collide)
{
    //if player entered brick frame or collide list isn't empty --> there was collision with a block
    if ( (y()<fieldSize) || (y()>scene()->width() - fieldSize -playerSize) || (x()<fieldSize) || (x()>scene()->height() - fieldSize - playerSize) || (collide.size()) )
        return true;

    //there wasn't any collision
    return false;
}
