#ifndef PLAYER_H
#define PLAYER_H
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QPixmap>
#include <QGraphicsScene>
#include <QTimer>
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

    void reset_direction(int key);

private:
    //player's keys selected in settings menu
    keys_t keys;
    QString color, name;

    int currDir;
    int movingTime;

    QTimer moveTimer;

    //draw players
    void setup_player(QGraphicsScene *scene);

private slots:
    void onMoveTimeout();
};

#endif // PLAYER_H
