#include "player.h"

extern int fieldSize;
int player::playerSize;

player::player(const playerData& data, QGraphicsScene *scene)
    :QGraphicsPixmapItem()
{
    //import data about player
    keys=data.keys;
    name=data.name;
    color=data.color;

    movingTime=50;
    currDir=-1;

    setup_player(scene);

    QObject::connect(&moveTimer, SIGNAL(timeout()), this, SLOT(onMoveTimeout()));
}

void player::move_player(int key)
{
    if (key==keys.up || key==keys.down || key==keys.left || key==keys.right)
    {
        currDir=key;
        moveTimer.start(movingTime);
    }
    else if (key==keys.bomb)
    {

    }
}

void player::reset_direction(int key)
{
    if (key==currDir)
    {
        currDir=-1;
        moveTimer.stop();
    }
}

void player::setup_player(QGraphicsScene* scene)
{
    //resize pixmap
    QPixmap p(":/images/img/" + color + ".png");
    p=p.scaled(playerSize, playerSize);

    //set player's image
    setPixmap(p);

    //set player's position on the map
    int margin=(fieldSize - playerSize)/2 + fieldSize;
    if (color=="red")
        setPos(margin, margin);         //top left
    else if (color=="blue")
        setPos(scene->width() - playerSize - margin, margin);     //top right
    else if (color=="green")
        setPos(margin, scene->height() - playerSize - margin);    //bottom left
    else if (color=="yellow")
        setPos(scene->width() - playerSize - margin, scene->height() - playerSize - margin);    //bottom right
}

void player::onMoveTimeout()
{
    if (currDir==keys.up)
        setY(y() - 10);
    else if (currDir==keys.down)
        setY(y() + 10);
    else if (currDir==keys.left)
        setX(x() - 10);
    else if (currDir==keys.right)
        setX(x() + 10);
}
