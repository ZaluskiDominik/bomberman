#include "game.h"
#include <QFile>
#include <QMessageBox>
#include <QDebug>

const int w=20, h=17;
int fieldSize;

game::game(QWidget* parent, QSize resolution)
    :QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint)
{
    fieldSize=resolution.height()/h;
    setFixedSize(resolution);
    setWindowTitle("Bomberman");

    //setting graphicsView and scene
    scene=new QGraphicsScene(this);
    view=new QGraphicsView(scene, this);
    view->setGeometry(0, 0, width(), height());
    view->show();
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene->setSceneRect(0, 0, view->width(), view->height());


    //loading map
    load_field();
}

void game::load_field()
{
    QFile file("field.txt");
    if (file.open(QIODevice::ReadOnly))
    {
        QString temp(file.readAll());
        qDebug()<<temp;
        file.close();
    }
    else
    {
        //error file isnt open
        parentWidget()->show();
        QMessageBox::warning(parentWidget(), "Error", "Could not open file field.txt", QMessageBox::Ok);
        deleteLater();
    }
}

//KEY EVENTS
void game::keyPressEvent(QKeyEvent *e)
{

}
