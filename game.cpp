#include "game.h"
#include <fstream>
#include <QMessageBox>
#include <algorithm>

//every field is a square
//length of the side of the square
int fieldSize;

//list with undestroyable obstacles
QList<QGraphicsPixmapItem*> obstacles;

//list witch desroyable wooded chests
QList<QGraphicsPixmapItem*> chests;

//list with players in game
QList<player*> gamePlayers;

game::game(QWidget* parent, QSize resolution, const playerData *playersData, int numPlayers)
    :QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint)
{
    //handle keybord events
    grabKeyboard();

    //set window resolution and title
    setFixedSize(resolution);
    setWindowTitle("Bomberman");

    load_pixmaps();

    //load the map to memory
    if (!load_map())
    {
        //an error occured
        parentWidget()->show();
        QMessageBox::warning(parentWidget(), "Error", "Could not open file field.txt", QMessageBox::Ok);
        deleteLater();
        return;
    }

    //draw graphicsView's frame made of brick
    draw_bricks();

    //draw players
    spawn_players(playersData, numPlayers);
}

void game::setup_graphics()
{
    //create graphics scene
    scene=new QGraphicsScene(this);
    view=new QGraphicsView(scene, this);

    //calculate geometry of graphics view and scene
    int xOffset=(0.8 * width() - (w * fieldSize))/2 + (0.2 * width()), yOffset=(height() - (h * fieldSize))/2;
    view->setGeometry(xOffset, yOffset, w * fieldSize, h * fieldSize);
    scene->setSceneRect(0, 0, view->width(), view->height());
    view->show();

    //set view's scroll bars off
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //set background image
    scene->setBackgroundBrush(QImage(":/images/img/fields/road.jpg"));
}

void game::load_pixmaps()
{
    fields[0].load(":/images/img/fields/obstacle.png");
    fields[1].load(":/images/img/fields/brick.jpg");
    fields[2].load(":/images/img/fields/chest.png");
}

QPixmap game::get_field_pixmap(fieldType name, int size)
{
    QPixmap pixmap;
    //choose pixmap based on name
    if (name==Obstacle)
        pixmap=fields[0];
    else if (name==Brick)
        pixmap=fields[1];
    else if (name==Chest)
        pixmap=fields[2];

    //scale pixmap
    return pixmap.scaled(size, size);
}

bool game::load_map()
{
    std::ifstream file("map.txt");

    if (!file.is_open())
        //error file isn't open
        return false;

    //read width and height(in number of fields)
    file>>w>>h;
    //two additional rows and columns for brick
    w+=2;
    h+=2;

    //calculate the map size
    fieldSize=std::min(height()/h, int(width() * 0.8 / w));

    //create graphics scene
    setup_graphics();

    //read fields and draw them
    draw_fields(file);

    //exit without errors
    return true;
}

void game::draw_fields(std::ifstream& file)
{
    //number determinating which field this is
    int field;
    for (int i=1 ; i<h-1 ; i++)
    {
        for (int j=1 ; j<w-1 ; j++)
        {
            file>>field;
            if (field==Obstacle)
            {
                obstacles.append(new QGraphicsPixmapItem(get_field_pixmap(Obstacle, fieldSize)));
                obstacles.back()->setPos(j * fieldSize, i * fieldSize);
                scene->addItem(obstacles.back());
            }
            else if (field==Chest)
            {
                chests.append(new QGraphicsPixmapItem(get_field_pixmap(Chest, fieldSize)));
                chests.back()->setPos(j * fieldSize, i * fieldSize);
                scene->addItem(chests.back());
            }
        }
    }
}

void game::draw_bricks()
{
    //vertical
    for (int i=0 ; i<h ; i++)
    {
        QGraphicsPixmapItem* item[2]={new QGraphicsPixmapItem(get_field_pixmap(Brick, fieldSize)), new QGraphicsPixmapItem(get_field_pixmap(Brick, fieldSize))};
        item[0]->setPos(0, i * fieldSize);
        scene->addItem(item[0]);
        item[1]->setPos(view->width() - fieldSize, i * fieldSize);
        scene->addItem(item[1]);
    }

    //horizontal
    for (int i=1 ; i<w-1 ; i++)
    {
        QGraphicsPixmapItem* item[2]={new QGraphicsPixmapItem(get_field_pixmap(Brick, fieldSize)), new QGraphicsPixmapItem(get_field_pixmap(Brick, fieldSize))};
        item[0]->setPos(i * fieldSize, 0);
        scene->addItem(item[0]);
        item[1]->setPos(i * fieldSize, view->height() - fieldSize);
        scene->addItem(item[1]);
    }
}

void game::spawn_players(const playerData* data, int numPlayers)
{
    for (int i=0 ; i<numPlayers ; i++)
        gamePlayers.append(new player(data[i], scene));
}

//KEY EVENTS
void game::keyPressEvent(QKeyEvent *e)
{
    //return if event for that key was repeated
    if (e->isAutoRepeat())
        return;

    //let each player handle pressing keys
    for (int i=0 ; i<gamePlayers.size() ; i++)
        gamePlayers[i]->key_pressed(e->key());
}

void game::keyReleaseEvent(QKeyEvent *e)
{
    if (e->isAutoRepeat())
        return;

    //let each player handle releasing keys
    for (int i=0 ; i<gamePlayers.size() ; i++)
        gamePlayers[i]->key_released(e->key());
}
