#ifndef PLAYER_H
#define PLAYER_H
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QPixmap>
#include <QGraphicsScene>
#include <QTimer>
#include <QList>
#include "structures.h"

//class representing a player in the game

class player : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    player(const playerData& data, QGraphicsScene* scene);

    static int playerSize;

    //player makes a move if one of his keys has been pressed
    void move_player(int key);

    //release moving key
    void reset_direction(int key);

private:
    //player's keys selected in settings menu
    keys_t keys;
    QString color, name;

    //list with keys indicating player's moving directory
    QList<int> currDir;

    //period of time when player don't move
    int movingTime;

    //timer for measuring time between moves
    QTimer moveTimer;

    //draw players
    void setup_player(QGraphicsScene *scene);

private slots:
    void onMoveTimeout();
};

#endif // PLAYER_H
