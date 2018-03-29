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
#include "powerup.h"
#include "chest.h"

//class representing a player in the game

class player : public QObject, public QGraphicsPixmapItem
{
    friend class game;
    Q_OBJECT

public:
    player(const playerData& data, QGraphicsScene* scene);

private:
    //player's keys selected in settings menu
    keys_t keys;
    QString name;
    playerColor::color color;
    int lifes;
    int maxNumBombs;
    int explosionRange;

    //after explosion hit player is immortal few moments
    bool immortal;
    int immortalTimeCounter;
    static const int immortalityTime=3000;
    QTimer immortalTimer;

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
    static int movingDist;

    //timer for measuring time between moves
    QTimer moveTimer;

    //player images representing stage of moving
    const int numPixmaps;
    QPixmap playerBack[8];
    QPixmap playerFront[8];
    QPixmap playerSide[8];

    //which pixmap should be applied as current player's image
    int moveStage;

    bool playerDead;

    //**********************************************************************

    //add new direction if it's one of this player's keys
    void key_pressed(int key);

    //release key if it's one of this player's keys
    void key_released(int key);

    //set player's position and pixmap
    void setup_player(QGraphicsScene *scene);

    //load player's pixmaps to memory
    void setup_pixmaps();

    //change player's pixmap based on direction
    void set_player_pixmap(int dir);

    //move player in given direction and distance
    void change_player_pos(int dir, int dist);

    void place_bomb();

    //player got hit by flames
    void explosion_hit();

    //COLLISIONS*******************************

    //remove colliding players or flame from collide list
    void remove_colliding_players(QList<QGraphicsItem*>& collide);

    void handle_bombs(QList<QGraphicsItem*>& collide);

    void handle_flames(QList<QGraphicsItem*>& collide);

    void handle_powerups(QList<QGraphicsItem*>& collide);

    //check for collision with obstacles, wooden chests, bricks or bombs
    bool collision(QList<QGraphicsItem*>& collide);

    //check if player left bomb's rect
    void left_bomb_rect();

    void try_bypassing_obstacle(QList<QGraphicsItem*>& collide, const int dir, const QPoint origin);

private slots:
    void onMoveTimeout();

    void onBombExploded();

    void onImmortalTimeout();

    void onPlayerDead();

signals:
    //signal sended to game class when bomb explodes
    void drawFlamesRequest(bomb*);

    void dead();

};

#endif // PLAYER_H
