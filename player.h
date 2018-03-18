#ifndef PLAYER_H
#define PLAYER_H
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QPixmap>
#include <QGraphicsScene>
#include <QTimer>
#include <QList>
#include "otherFunctions.h"
#include "bomb.h"

//class representing a player in the game

class player : public QObject, public QGraphicsPixmapItem
{
    friend class bomb;
    Q_OBJECT

public:
    player(const playerData& data, QGraphicsScene* scene);

    //add new direction if it's one of this player's keys
    void key_pressed(int key);

    //release key if it's one of this player's keys
    void key_released(int key);

private:
    //player's keys selected in settings menu
    keys_t keys;
    QString name;
    playerColor::color color;
    int lifes;
    int maxNumBombs;
    int explosionRange;

    //player's number of bombs currently placed at the scene
    int bombsPlaced;

    //after collecting an item player can push bomb
    //time in which bomb move one unit of distance
    int bombPushInterv;

    //list with keys indicating player's moving directory
    QList<int> currDir;

    //time between previous and next stage of moving animation
    int movingTime;

    //distance which player moves each time
    static const int movingDist=3;

    //timer for measuring time between moves
    QTimer moveTimer;

    //player images representing stage of moving
    const int numPixmaps;
    QPixmap playerBack[8];
    QPixmap playerFront[8];
    QPixmap playerSide[8];

    //which pixmap should be applied as current player's image
    int moveStage;

    //**********************************************************************

    //set player's position and pixmap
    void setup_player(QGraphicsScene *scene);

    //load player's pixmaps to memory
    void setup_pixmaps();

    //change player's pixmap based on direction
    void set_player_pixmap(int dir);

    //move player in given direction and distance
    void change_player_pos(int dir, int dist);

    void place_bomb();

    //remove colliding players or flame from collide list
    void remove_colliding_players_or_flame(QList<QGraphicsItem*>& collide);

    //handle pushing bombs, leaving bomb's rect
    void handle_bombs(QList<QGraphicsItem*>& collide);

    //check if player left bomb's rect
    void left_bomb_rect();

    //check for collision with obstacles, wooden chests, bricks or bombs
    bool collision(QList<QGraphicsItem*>& collide);

private slots:
    void onMoveTimeout();

    void onBombExploded();

signals:
    //signal sended to game class when bomb explodes
    void drawFlamesRequest(bomb*);

};

#endif // PLAYER_H
