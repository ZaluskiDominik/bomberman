#ifndef BOMB_H
#define BOMB_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QTimer>

class bomb : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    bomb(QPoint pos, QGraphicsScene* scene);
    ~bomb();

    static int bombSize;

    //players that were inside the bomb's shape during its placing at the scene
    //needed to decide whether the bomb should be collideable with them
    QList<QGraphicsItem*> playersInsideShape;

private:
    //time in miliseconds till bomb explode
    static const int timeToExplode=2800;

    //number of bomb's pixmaps which make animation
    static const int numBombPixmaps=3;
    int explodeStage;

    //counts time till bomb explode
    QTimer explodeTimer;

    //********************************************

    //add players to playerInsideShape list if they are colliding with the bomb during its creating
    inline void mark_players_inside();

    //change bomb's pixmap based on its stage of explosion
    void set_bomb_pixmap();

    //compute point where bomb will be placed
    void calculate_bomb_pos(QPoint pos);

    void explode();

private slots:
    //detonate the bomb
    void advance_explode();

};

#endif // BOMB_H
