#include "game.h"
#include <QFile>
#include <QMessageBox>
#include <algorithm>
#include <QDebug>

//every field is a square
//field size
int fieldSize;

//list with undestroyable obstacles
QList<QGraphicsPixmapItem*> obstacles;

//list witch desroyable wooded chests
QList<QGraphicsPixmapItem*> chests;

//players in game
std::vector<player*> gamePlayers;

game::game(QWidget* parent, QSize resolution, const playerData *playersData, int numPlayers)
    :QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint)
{
    //handle keybord events
    grabKeyboard();

    //set window resolution and title
    setFixedSize(resolution);
    setWindowTitle("Bomberman");

    //set pixmaps paths
    setup_pixmaps();

    if (!load_map())
        return;

    //draw graphicsView's frame as brick
    draw_bricks();

    //draw players
    gamePlayers.resize(numPlayers);
    spawn_players(playersData);
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

void game::setup_pixmaps()
{
    //fields
    fieldPixmaps[0].load(":/images/img/fields/obstacle.png");
    fieldPixmaps[1].load(":/images/img/fields/brick.jpg");
    fieldPixmaps[2].load(":/images/img/fields/chest.png");
}

QPixmap game::getFieldPixmap(fieldType name, int size)
{
    QPixmap pixmap;
    //choose pixmap based on name
    if (name==Obstacle)
        pixmap=fieldPixmaps[0];
    else if (name==Brick)
        pixmap=fieldPixmaps[1];
    else if (name==Chest)
        pixmap=fieldPixmaps[2];

    //resize
    pixmap=pixmap.scaled(size, size);
    return pixmap;
}

bool game::load_map()
{
    QFile file("map.txt");
    QString line;

    if (!file.open(QIODevice::ReadOnly))
    {
        //error file isn't open
        parentWidget()->show();
        QMessageBox::warning(parentWidget(), "Error", "Could not open file field.txt", QMessageBox::Ok);
        deleteLater();
        return false;
    }

    //read dimensions
    line=file.readLine();
    //two additional rows and columns for brick
    w=line.left(line.size()-2).toInt() + 2;
    line=file.readLine();
    h=line.left(line.size()-2).toInt() + 2;

    //calculate map size
    fieldSize=std::min(height()/h, int(width() * 0.8 / w));

    //create graphics scene
    setup_graphics();

    //read fields and draw them
    draw_fields(&file);

    //exit without errors
    return true;
}

void game::draw_fields(QFile* file)
{
    QString line;
    //number determinating which field this is
    int field;
    for (int i=1 ; i<h-1 ; i++)
    {
        line=file->readLine();
        for (int j=1 ; j<w-1 ; j++)
        {
            field=line[j-1].toLatin1() - '0';
            if (field==Obstacle)
            {
                obstacles.append(new QGraphicsPixmapItem(getFieldPixmap(Obstacle, fieldSize)));
                obstacles.back()->setPos(j * fieldSize, i * fieldSize);
                scene->addItem(obstacles.back());
            }
            else if (field==Chest)
            {
                chests.append(new QGraphicsPixmapItem(getFieldPixmap(Chest, fieldSize)));
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
        QGraphicsPixmapItem* item[2]={new QGraphicsPixmapItem(getFieldPixmap(Brick, fieldSize)), new QGraphicsPixmapItem(getFieldPixmap(Brick, fieldSize))};
        item[0]->setPos(0, i * fieldSize);
        scene->addItem(item[0]);
        item[1]->setPos(view->width() - fieldSize, i * fieldSize);
        scene->addItem(item[1]);
    }

    //horizontal
    for (int i=1 ; i<w-1 ; i++)
    {
        QGraphicsPixmapItem* item[2]={new QGraphicsPixmapItem(getFieldPixmap(Brick, fieldSize)), new QGraphicsPixmapItem(getFieldPixmap(Brick, fieldSize))};
        item[0]->setPos(i * fieldSize, 0);
        scene->addItem(item[0]);
        item[1]->setPos(i * fieldSize, view->height() - fieldSize);
        scene->addItem(item[1]);
    }
}

void game::spawn_players(const playerData* data)
{
    //set players' width and height
    player::playerSize=fieldSize * 0.9;

    //add players to scene
    for (unsigned int i=0 ; i<gamePlayers.size() ; i++)
    {
        gamePlayers[i]=new player(data[i], scene);
        scene->addItem(gamePlayers[i]);
    }
}

//KEY EVENTS
void game::keyPressEvent(QKeyEvent *e)
{
    if (e->isAutoRepeat())
        return;

    //let each player handle pressing keys
    for (unsigned int i=0 ; i<gamePlayers.size() ; i++)
        gamePlayers[i]->move_player(e->key());
}

void game::keyReleaseEvent(QKeyEvent *e)
{
    if (e->isAutoRepeat())
        return;

    //let each player handle releasing keys
    for (unsigned int i=0 ; i<gamePlayers.size() ; i++)
        gamePlayers[i]->reset_direction(e->key());
}
