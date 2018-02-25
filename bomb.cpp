#include "bomb.h"
#include <QList>
#include <QGraphicsScene>

int bomb::bombSize;

bomb::bomb()
{
    explodeStage=0;
    set_bomb_pixmap();
    QObject::connect(&explodeTimer, SIGNAL(timeout()), this, SLOT(explode()));
}

void bomb::set_bomb_pixmap()
{
    setPixmap(QPixmap(":/images/img/bomb/bomb.png").scaled(bombSize, bombSize));
}

void bomb::explode()
{
    scene()->removeItem(this);
    this->deleteLater();
}
