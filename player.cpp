#include "player.h"
#include "flame.h"

extern QList<QGraphicsPixmapItem*> obstacles;
extern QList<QGraphicsPixmapItem*> chests;
extern QList<bomb*> bombs;
extern int fieldSize;

int player::movingDist;

//************************************************************************************

player::player(const playerData& data, QGraphicsScene *scene)
    :numPixmaps(8)
{
    scene->addItem(this);
    setZValue(2);

    //import data about player, keys setting
    keys=data.keys;
    name=data.name;
    color=string_to_playerColor(data.color);
    lifes=2;
    immortal=false;

    //in the beginning player can place only 1 bomb at any time
    maxNumBombs=1;
    bombsPlaced=0;
    explosionRange=1;
    //in the beginning of the game player can't push bombs
    bombPushInterv=-1;

    movingTime=40;
    moveStage=0;

    //load player's pixmaps
    setup_pixmaps();

    setup_player(scene);

    QObject::connect(&moveTimer, SIGNAL(timeout()), this, SLOT(onMoveTimeout()));
    QObject::connect(&immortalTimer, SIGNAL(timeout()), this, SLOT(onImmortalTimeout()));
}

void player::key_pressed(int key)
{
    //if players isn't on scene
    if (scene()==0)
        return;

    if (key==keys.up || key==keys.down || key==keys.left || key==keys.right)
    {
        currDir.append(key);
        //start timer if this was a first key
        if (currDir.size()==1)
            moveTimer.start(movingTime);

    }
    else if (key==keys.bomb)
    {
        //try to create a bomb
        if (bombsPlaced<maxNumBombs)
            place_bomb();
    }
}

void player::key_released(int key)
{
    //if currDir is empty after removing released key direction
    if (currDir.removeOne(key))
    {
        if (currDir.empty())
        {
            //stop moving, reset moving stage
            moveTimer.stop();
            moveStage=0;
            set_player_pixmap(key);
        }
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
            //mirror pixmap, player will be looking in left direction
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

void player::place_bomb()
{
    QPoint bombPos=bomb::calculate_bomb_pos(pos().toPoint());

    for (auto i=bombs.begin() ; i!=bombs.end() ; i++)
    {
        if ((*i)->x()==bombPos.x() && (*i)->y()==bombPos.y())
        {
            //bomb on that position already exists, so don't create another one
            return;
        }
    }

    //create a new bomb
    bombsPlaced++;
    bomb* newBomb=new bomb(bombPos, explosionRange, this, scene());
    QObject::connect(newBomb, SIGNAL(bombExploded()), this, SLOT(onBombExploded()));
}

void player::onMoveTimeout()
{
    int dirIter=currDir.size()-1;
    //save player position
    QPoint originPoint(x(), y());
    QList<QGraphicsItem*> collide;

    do
    {
        //reset player's position to origin
        setPos(originPoint);

        //move player
        change_player_pos(currDir[dirIter], movingDist);

        //get the list of items colliding with the player
        collide=collidingItems(Qt::IntersectsItemBoundingRect);
        //remove players from colliding items list
        remove_colliding_players(collide);

        handle_flames(collide);
        handle_powerups(collide);
        handle_bombs(collide);
    }
    while (--dirIter >= 0 && collision(collide));
    dirIter++;

    if (collision(collide))
    {
        //collision
        moveStage=0;
        setPos(originPoint);
    }
    else
    {
        //no collision
        //advance in move animation
        moveStage=(moveStage + 1) % numPixmaps;
        //check if player left bomb's rect
        left_bomb_rect();
    }

    set_player_pixmap(currDir[dirIter]);
}

void player::remove_colliding_players(QList<QGraphicsItem *> &collide)
{
    for (int i=0 ; i<collide.size() ; i++)
        if (typeid(*collide[i])==typeid(player))
            collide.removeOne(collide[i--]);
}

void player::handle_bombs(QList<QGraphicsItem *> &collide)
{
    for (int i=0 ; i<collide.size() ; i++)
    {
        if (typeid(*collide[i])==typeid(bomb))
        {
            bomb* collideBomb=static_cast<bomb*>(collide[i]);

            //if playersInside list is not empty
            if (!collideBomb->playersInside.empty())
            {
                //bomb isn't pushable
                QList<QGraphicsItem*>::iterator it;
                //check if this player is in playersInside list
                for (it=collideBomb->playersInside.begin() ; it!=collideBomb->playersInside.end() && this!=*it ; it++);

                if (it!=collideBomb->playersInside.end())
                {
                    //this player is inside bomb's rect, so this bomb shouldn't be collideable for this player
                    //remove that bomb from collide list
                    collide.removeOne(collide[i--]);
                }
            }
            else
            {
                //bomb can be pushed by players
            }
        }
    }
}

void player::handle_flames(QList<QGraphicsItem *> &collide)
{
    for (int i=0 ; i<collide.size() ; i++)
    {
        if (typeid(*collide[i])==typeid(flame))
        {
            collide.removeOne(collide[i--]);
            explosion_hit();
        }
    }
}

void player::handle_powerups(QList<QGraphicsItem *> &collide)
{
    for (auto i=collide.begin() ; i!=collide.end() ; i++)
    {
        if (typeid(**i)==typeid(powerup) && ( (*i)->x()==x() || (*i)->y()==y() ) )
        {
            powerup* item=static_cast<powerup*>(*i);
            //if powerup is under chest
            if (!item->is_pickable())
                return;

            switch(item->get_powerType())
            {
            case powerup::powerupType::NewBomb:
                maxNumBombs++;
                break;
            case powerup::powerupType::Boots:
                movingTime = (movingTime>15) ? movingTime*0.9 : movingTime;
                break;
            case powerup::powerupType::BiggerRange:
                explosionRange++;
                break;
            case powerup::powerupType::PushBombs:
                //bombPushInterv
                break;
            }
            collide.removeOne(item);
            delete item;
            break;
        }
    }
}

void player::left_bomb_rect()
{
    auto collide=collidingItems(Qt::IntersectsItemBoundingRect);
    for (int i=0 ; i<bombs.size() ; i++)
    {
        //if player isn't colliding with that bomb
        if (!collide.contains(bombs[i]))
        {
            //remove if player exists in playersInside list
            if (bombs[i]->playersInside.removeOne(this))
                i--;
        }
    }
}

bool player::collision(QList<QGraphicsItem *>& collide)
{
    //if player entered brick frame or collide list isn't empty
    if ( (y()<fieldSize) || (y()>scene()->width() - (2*fieldSize)) || (x()<fieldSize) || (x()>scene()->height() - (2*fieldSize)) || (collide.size()) )
        return true;

    //there wasn't any collision
    return false;
}

void player::explosion_hit()
{
    //if player was immortal hit don't affect him
    if (immortal)
        return;

    lifes--;
    if (lifes>0)
    {
        //still alive
        immortal=true;
        immortalTimeCounter=0;
        setOpacity(0.25);
        immortalTimer.start(immortalityTime/10);
    }
    else
    {
        //end of game for this player
        QObject::disconnect(&moveTimer, SIGNAL(timeout()), this, SLOT(onMoveTimeout()));
        QObject::connect(this, SIGNAL(dead()), this, SLOT(onPlayerDead()), Qt::QueuedConnection);
        emit dead();
    }
}

void player::onBombExploded()
{
    bomb* bombSender=qobject_cast<bomb*>(sender());
    //if it's the player's bomb then decrement the number of the player's used bombs
    if (bombSender->whoseBomb==this)
        bombsPlaced--;

    emit drawFlamesRequest(bombSender);
}

void player::onImmortalTimeout()
{
    immortalTimeCounter+=immortalityTime/10;
    if (immortalTimeCounter>=immortalityTime)
    {
        //end immortality
        immortalTimer.stop();
        immortal=false;
        setOpacity(1);
    }
    else
        setOpacity(opacity()==1 ? 0.25 : 1.0);
}

void player::onPlayerDead()
{
    scene()->removeItem(this);
}
