#include "player.h"

extern QList<QGraphicsPixmapItem*> obstacles;
extern QList<QGraphicsPixmapItem*> chests;
extern QList<bomb*> bombs;
extern int fieldSize;

//************************************************************************************

player::player(const playerData& data, QGraphicsScene *scene)
    :numPixmaps(8)
{
    scene->addItem(this);

    //import data about player, keys setting
    keys=data.keys;
    name=data.name;
    color=string_to_playerColor(data.color);
    lifes=2;
    maxNumBombs=1;
    bombsPlaced=0;

    //in the beginning of the game player can't push bombs
    bombPushInterv=-1;

    movingTime=20;
    moveStage=0;

    //load player pixmaps
    setup_pixmaps();

    //add player to scene
    setup_player(scene);

    QObject::connect(&moveTimer, SIGNAL(timeout()), this, SLOT(onMoveTimeout()));
}

void player::key_pressed(int key)
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

void player::key_released(int key)
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
    if (color==playerColor::White)
        setPos(fieldSize, fieldSize);         //top left
    else if (color==playerColor::Silver)
        setPos(scene->width() - (2*fieldSize), fieldSize);     //top right
    else if (color==playerColor::Green)
        setPos(fieldSize, scene->height() - (2*fieldSize));    //bottom left
    else //yellow
        setPos(scene->width() - (2*fieldSize), scene->height() - (2*fieldSize));    //bottom right

    //set player's pixmap
    set_player_pixmap(keys.down);
}

void player::setup_pixmaps()
{
    //front of the player
    for (int i=0 ; i<numPixmaps ; i++)
    {
        playerFront[i]=color_player(color ,":/images/img/players/white/Front/Bman_F_f0" + QString::number(i) + ".png");
        playerFront[i]=playerFront[i].copy(0, 30, playerFront[i].width(), playerFront[i].height()).scaled(fieldSize, fieldSize);
    }

    //back of the player
    for (int i=0 ; i<numPixmaps ; i++)
    {
        playerBack[i]=color_player(color, ":/images/img/players/white/Back/Bman_B_f0" + QString::number(i) + ".png");
        playerBack[i]=playerBack[i].copy(0, 30, playerBack[i].width(), playerBack[i].height()).scaled(fieldSize, fieldSize);
    }

    //side of the player(player look in right direction)
    for (int i=0 ; i<numPixmaps ; i++)
    {
        playerSide[i]=color_player(color, ":/images/img/players/white/Side/Bman_F_f0" + QString::number(i) + ".png");
        playerSide[i]=playerSide[i].copy(0, 30, playerSide[i].width(), playerSide[i].height()).scaled(fieldSize, fieldSize);
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
            setTransform(QTransform().scale(-1, 1).translate(-fieldSize, 0));
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

        //check if player didn't leave bomb's shape
        left_bomb_shape();
    }
    else
        //collision
        moveStage=0;

    set_player_pixmap(currDir[dirIter]);
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

    for (int i=0 ; i<collide.size() ; i++)
    {
        //it's a bomb
        if (typeid(*collide[i])==typeid(bomb))
        {
            //find that bomb in bombs list
            for (collidingBomb=bombs.begin() ; (*collidingBomb)!=collide[i] ; collidingBomb++);

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
                    collide.removeOne(collide[i--]);
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
    if ( (y()<fieldSize) || (y()>scene()->width() - (2*fieldSize)) || (x()<fieldSize) || (x()>scene()->height() - (2*fieldSize)) || (collide.size()) )
        return true;

    //there wasn't any collision
    return false;
}
