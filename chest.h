#ifndef CHEST_H
#define CHEST_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <memory>

struct obstacle : public QGraphicsPixmapItem
{
    obstacle(const QPixmap& p) :QGraphicsPixmapItem(p) {}
};

class chest : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    chest();
    void explode();

    //whether bomb is currently exploding
    bool in_process_of_destruction();

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

#endif
