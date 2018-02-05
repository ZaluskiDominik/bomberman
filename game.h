#ifndef GAME_H
#define GAME_H

#include <QDialog>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QGraphicsScene>

class game : public QDialog
{
    Q_OBJECT
public:
    explicit game(QWidget *parent, QSize resolution);

private:
    enum{obstacle='0', road='1'};

    QGraphicsView* view;
    QGraphicsScene* scene;

    //load map from disk
    void load_field();

protected:
    void keyPressEvent(QKeyEvent* e);

};

#endif // GAME_H
