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

    //graphics scene
    QGraphicsView* view;
    QGraphicsScene* scene;

    //playing field dimensions size(width, height)
    int w, h;

    //pixmaps representing fields
    QPixmap fieldPixmaps[3];

    //FUNCTIONS
    //**************************************************

    //create graphicsView and graphicsScene
    void setup_graphics();

    //create pixmap objects which are needed to fields as images
    void setup_pixmaps();

    //return scaled pixmap of a field
    QPixmap getFieldPixmap(fieldType name, int size);

    //load map from disk
    bool load_map();

    //read fields from file and draw them
    void draw_fields(QFile *file);

    //draw fram of bricks around graphicsView
    void draw_bricks();

    //create players and set their positions
    void spawn_players(const playerData *data);

protected:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

};

#endif // GAME_H
