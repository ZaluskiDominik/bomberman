#include "player.h"

extern QList<QGraphicsPixmapItem*> obstacles;
extern QList<QGraphicsPixmapItem*> chests;
extern QList<bomb*> bombs;
extern int fieldSize;

extern player::playerColor string_to_playerColor(QString str);

// CHANGING PLAYER HELMET'S COLOR

QPixmap player::color_player(playerColor color, QString imgPath)
{
    QImage img(imgPath);
    if (color==Silver)
        color_player_helper(img, 32, 40, QColor(0, 76, 153));
    else if (color==Green)
        color_player_helper(img, 32, 40, QColor(51, 255, 51));
    else if(color==Yellow)
        color_player_helper(img, 32, 40, Qt::yellow);
    return QPixmap::fromImage(img);
}

void player::color_player_helper(QImage &img, int x, int y, QColor color)
{
    if (((x-32)*(x-32)) + ((y-59)*(y-59)) > 25*25 || stop_coloring(img.pixelColor(x, y), color))
        return;

    img.setPixelColor(x, y, color);

    color_player_helper(img, x, y+1, color);
    color_player_helper(img, x, y-1, color);
    color_player_helper(img, x+1, y, color);
    color_player_helper(img, x-1, y, color);
}

bool player::stop_coloring(const QColor& color, const QColor& aimedColor)
{
    //yellow face
    if (color.red()==255 && color.green()==218 && color.blue()==48)
        return true;
    //brown something on helmet
    if (color.red()==203 && color.green()==99 && color.blue()==43)
        return true;
    //player's back
    if (color.red()==228 && color.green()==224 && color.blue()==195)
        return true;
    //aimed color --> this pixel already has been visited
    if (color.red()==aimedColor.red() && color.green()==aimedColor.green() && color.blue()==aimedColor.blue())
        return true;
    return false;
}

//************************************************************************************

player::player(const playerData& data, QGraphicsScene *scene)
    :numPixmaps(8)
{
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
    if (color==White)
    {
        setPos(fieldSize, fieldSize);         //top left
        set_player_pixmap(keys.down);
    }
    else if (color==Silver)
    {
        setPos(scene->width() - (2*fieldSize), fieldSize);     //top right
        set_player_pixmap(keys.down);
    }
    else if (color==Green)
    {
        setPos(fieldSize, scene->height() - (2*fieldSize));    //bottom left
        set_player_pixmap(keys.up);
    }
    else //yellow
    {
        setPos(scene->width() - (2*fieldSize), scene->height() - (2*fieldSize));    //bottom right
        set_player_pixmap(keys.up);
    }
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
    if ( (y()<fieldSize) || (y()>scene()->width() - (2*fieldSize)) || (x()<fieldSize) || (x()>scene()->height() - (2*fieldSize)) || (collide.size()) )
        return true;

    //there wasn't any collision
    return false;
}
