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

class game : public QDialog
{
    Q_OBJECT
public:
    explicit game(QWidget *parent, QSize resolution, const playerData* playersData, int numPlayers);

private:
    //enum type for identyfing game's fields
    //obstacle - undestroyable, road - background, Brick - frame, Chest - destroyable
    enum fieldType{Road=0, Obstacle, Chest, Brick};

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

    //create players and set their positions
    void spawn_players(const playerData *data, int numPlayers);

protected:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

};

#endif // GAME_H
