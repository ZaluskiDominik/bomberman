#ifndef BOMB_H
#define BOMB_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class bomb : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    bomb();
    static int bombSize;

private:
    static const int timeToExplode=2800;
    static const int numBombPixmaps=3;
    int explodeStage;

    //counts time till bomb explode
    QTimer explodeTimer;

    void set_bomb_pixmap();

    //detonate the bomb
    void explode();

};

#endif // BOMB_H
