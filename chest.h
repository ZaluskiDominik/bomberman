#ifndef CHEST_H
#define CHEST_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <memory>

class chest : public QObject, public QGraphicsPixmapItem
{
    friend class game;
    Q_OBJECT
public:
    chest();
    void explode();

    static void load_pixmaps();

    static void free_pixmaps();

private:
    static const int numPixmaps=32;
    static std::unique_ptr<QPixmap[]> explodePixmaps;
    int explodeStage;
    bool inDestruction;
    QTimer timer;
    QGraphicsPixmapItem* prevChest;

private slots:
    void onExplodeTimeout();

};

class obstacle : public QGraphicsPixmapItem
{

};

#endif
