#ifndef FLAME_H
#define FLAME_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QPixmap>

class flame : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    flame();

private:
    static const int existTime=1500;
    static const int timeoutPeriod=existTime/20;
    //cout the time until the flame exists
    int timeCounter;
    int animationStage;

    static const int numPixmaps=5;
    QPixmap pixmaps[numPixmaps];

    QTimer timer;

    //load pixmaps to memory
    void setup_pixmaps();

private slots:
    void onTimeout();

};

#endif // FLAME_H
