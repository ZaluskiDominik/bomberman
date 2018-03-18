#ifndef BOMB_H
#define BOMB_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QVector2D>

class bomb : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    friend class game;

public:
    bomb(QPoint pos, const int explosionRange, const QGraphicsItem* const owner, QGraphicsScene* scene);
    ~bomb();

    //players that were inside the bomb's rect during its placing at the scene
    //needed to decide whether the bomb should be collideable with them
    QList<QGraphicsItem*> playersInside;

    //player who placed the bomb
    const QGraphicsItem* const whoseBomb;

    //compute point where bomb should be placed
    static QPoint calculate_bomb_pos(QPoint pos);

private:
    //time in miliseconds till bomb explode
    static const int timeToExplode=2800;

    //number of bomb's pixmaps which make animation
    static const int numBombPixmaps=3;
    int explodeStage;

    //range of flames after explosion
    const int range;

    //counts time till bomb explode
    QTimer explodeTimer;

    //********************************************

    //add players to playerInside list if they are colliding with the bomb during its creating
    inline void mark_players_inside();

    //change bomb's pixmap based on its stage of explosion
    void set_bomb_pixmap();

    //detonate the bomb
    void explode();

private slots:
    //next bomb's pixmap
    void advanceExplode();

signals:
    void bombExploded();

};

#endif // BOMB_H
