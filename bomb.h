#ifndef BOMB_H
#define BOMB_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QMediaPlayer>

class bomb : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    friend class game;

public:
    bomb(QPoint pos, const int explosionRange, const QGraphicsItem* const owner, QGraphicsScene* scene);

    //compute point where bomb should be placed
    static QPoint calculate_bomb_pos(QPoint pos);

    //list of players that aren't collideable for this bomb
    QList<QGraphicsItem*> playersInside;

    //player who placed the bomb
    const QGraphicsItem* const whoseBomb;

    static int movingDistance;

    void push_bomb(int moveTime, QPoint direction);

    //detonate the bomb
    void explode();

private:
    //time in miliseconds till bomb explode
    static const int timeToExplode=2500;

    //range of flames after explosion
    const int range;

    //counts time till bomb explode
    QTimer explodeTimer;

    //whether this bomb was pushed by a player
    bool bombPushed;
    QTimer pushTimer;
    //direction where the bomb is moving
    QPoint moveDir;

    bool aboutToExplode;
    bool exploded;

    QMediaPlayer explosionPlayer;

    //add players to playerInside list if they are colliding with the bomb during its creating
    inline void mark_players_inside();

private slots:
    void onExplodeTimeout();
    void onPushTimeout();

    //after sound comes to end delete this bomb from memory
    void onMusicStateChanged(QMediaPlayer::State state);

signals:
    //send signal to player and game classes about bomb detonation
    void bombExploded();

};

#endif // BOMB_H
