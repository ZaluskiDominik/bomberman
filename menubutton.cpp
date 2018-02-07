#include "menubutton.h"
#include <QPainter>

menuButton::menuButton(QWidget *parent, QString text)
    :QAbstractButton(parent), str(text)
{
    //set button's background as transparent
    setStyleSheet("background: transparent");
    in=false;
    //set path of click sound
    clickPlayer.setMedia(QMediaContent(QUrl("qrc:/sounds/sounds/button_click.mp3")));
}

void menuButton::enterEvent(QEvent *)
{
    //mouse cursor entered the geometry of the button
    in=true;
    //play click sound
    clickPlayer.setPosition(0);
    clickPlayer.play();
}

void menuButton::leaveEvent(QEvent *)
{
    //mouse cursor leaved the geometry of the button
    in=false;
}

//paintig button
void menuButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    if (in)
    {
        //mouse cursor is in the button's area
        //change color of the button
        p.fillRect(QRect(0, 0, width(), height()), QBrush(QColor(255, 215, 0)));
    }

    //set text's font
    QFont f;
    f.setBold(true);
    f.setPixelSize(height()*0.5);
    p.setFont(f);

    //draw button's frame
    QPen pen(Qt::green);
    pen.setWidth(8);
    p.setPen(pen);
    p.drawRect(0, 0, width(), height());

    //draw button's text
    pen.setColor(Qt::black);
    p.setPen(pen);
    p.drawText(0, 0, width(), height(), Qt::AlignCenter, str);
}
