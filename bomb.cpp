#include "bomb.h"
#include <QList>
#include <QGraphicsScene>
#include "flame.h"
#include "powerup.h"

extern int volume;
extern int fieldSize;

extern QList<QGraphicsItem*> gamePlayers;

int bomb::movingDistance;

bomb::bomb(QPoint pos, const int explosionRange, const QGraphicsItem * const owner, QGraphicsScene* scene)
    :whoseBomb(owner), range(explosionRange)
{
    aboutToExplode=bombPushed=exploded=false;

    //set the bomb's position on the scene
    scene->addItem(this);
    setPos(pos);
    setPixmap(QPixmap(":/images/img/bomb/bomb.png").scaled(fieldSize, fieldSize));

    mark_players_inside();

    QObject::connect(&explodeTimer, SIGNAL(timeout()), this, SLOT(onExplodeTimeout()));
    QObject::connect(&pushTimer, SIGNAL(timeout()), this, SLOT(onPushTimeout()));
    QObject::connect(&explosionPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(onMusicStateChanged(QMediaPlayer::State)));

    explosionPlayer.setMedia(QMediaContent(QUrl("qrc:/sounds/sounds/explode.mp3")));
    explosionPlayer.setVolume(volume);

    //start counting time to explode
    explodeTimer.start(timeToExplode);
}

void bomb::mark_players_inside()
{
    playersInside=collidingItems(Qt::IntersectsItemBoundingRect);
}

void bomb::explode()
{
    if (!exploded)
    {
        exploded=true;
        emit bombExploded();
    }
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

void bomb::push_bomb(int moveTime, QPoint direction)
{
    //if the bomb isn't moving
    if (!bombPushed)
    {
        bombPushed=true;
        moveDir=direction;
        pushTimer.start(moveTime);

        //place all players in playersInside list so that the bomb won't collide with them
        playersInside=gamePlayers;
    }
}

void bomb::instant_explode()
{
    explodeTimer.stop();
    pushTimer.stop();
    explode();
}

void bomb::onExplodeTimeout()
{
    explodeTimer.stop();
    aboutToExplode=true;
    if (!bombPushed)
        explode();
}

void bomb::onPushTimeout()
{    
    //move the bomb
    setPos(pos() + moveDir);

    auto collide=collidingItems(Qt::IntersectsItemBoundingRect);
    //remove all players and powerups from collide list
    for (int i=0 ; i<collide.size() ; i++)
        if (gamePlayers.contains(collide[i]) || typeid(*collide[i])==typeid(powerup))
            collide.removeOne(collide[i--]);

    //if the bomb collide with an object then stop moving
    if (!collide.empty())
    {
        pushTimer.stop();
        setPos(pos() - moveDir);
        bombPushed=false;

        playersInside.clear();
        mark_players_inside();

        //flame collision
        if (typeid(*collide.first())==typeid(flame))
            explode();
    }

    //check if it's time for explode
    if (aboutToExplode && int(y())%fieldSize==0 && int(x())%fieldSize==0)
    {
        pushTimer.stop();
        explode();
    }
}

void bomb::onMusicStateChanged(QMediaPlayer::State state)
{
    //if explosion sound has ended then delete the bomb
    if (state==QMediaPlayer::State::StoppedState)
        deleteLater();
}
