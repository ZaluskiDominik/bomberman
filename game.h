#ifndef GAME_H
#define GAME_H

#include <QDialog>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QList>
#include "otherFunctions.h"
#include "player.h"
#include "chest.h"

class game : public QDialog
{
    Q_OBJECT
public:
    explicit game(QWidget *parent, const playerData* playersData, int numPlayers);
    ~game();

private:
    //enum type for identyfing game's fields
    //obstacle - undestroyable, road - background, Brick - frame, Chest - destroyable
    enum fieldType{Road=0, Obstacle, Chest, Brick};

    const int numPowerups;

    //field pixmaps
    QPixmap fields[3];

    //graphics scene
    QGraphicsView* view;
    QGraphicsScene* scene;

    //width and height in number of fields
    int w, h;

    //**************************************************

    //create graphicsView and graphicsScene
    void setup_graphics();

    //load fields pixmaps to memory
    void load_pixmaps();

    //return scaled pixmap of a field
    QPixmap get_field_pixmap(fieldType name, int size);

    //load map from disk
    bool load_map();

    //from loaded map add blocks to the scene
    void draw_fields(std::ifstream &file);

    //draw frame made of bricks around graphicsView
    void draw_bricks();

    //set all powerups under random chests
    void create_powerups();

    //create players and set their positions
    void spawn_players(const playerData *data, int numPlayers);

    void create_flame_line(QPoint direction, const bomb &b);

    bool is_obstacle(const QGraphicsItem* const item);

    bool is_chest(const QGraphicsItem* const item);

protected:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

private slots:
    //handle creating bomb explosion's flames
    void onDrawFlameRequest(bomb* b);

};

#endif // GAME_H
