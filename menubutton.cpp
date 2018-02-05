#include "menubutton.h"
#include <QPainter>

int menuButton::parentHeight=100, menuButton::parentWidth=50, menuButton::spacing=20;

menuButton::menuButton(QWidget *parent, QString text)
    :QAbstractButton(parent), str(text)
{
    setStyleSheet("background: transparent");
    in=false;
    clickPlayer.setMedia(QMediaContent(QUrl("qrc:/sounds/sounds/button click version sound effect  13.mp3")));
}

QSize menuButton::sizeHint() const
{
    int h=(parentHeight-(spacing*(buttons-1))-30)/buttons;
    return QSize(parentWidth-30, h);
}

void menuButton::frameSizeChanged(QSize size)
{
    parentWidth=size.width();
    parentHeight=size.height();
}

void menuButton::enterEvent(QEvent *)
{
    in=true;
    //click sound
    clickPlayer.setPosition(0);
    clickPlayer.play();
}

void menuButton::leaveEvent(QEvent *)
{
    in=false;
}

//paintig button
void menuButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    if (in)
    {
        //mouse focus
        p.fillRect(QRect(0, 0, width(), height()), QBrush(QColor(255, 215, 0)));
    }

    QFont f;
    f.setBold(true);
    f.setPixelSize(height()*0.5);
    p.setFont(f);
    QPen pen(Qt::green);
    pen.setWidth(8);
    p.setPen(pen);
    p.drawRect(0, 0, width(), height());

    pen.setColor(Qt::black);
    p.setPen(pen);
    p.drawText(0, 0, width(), height(), Qt::AlignCenter, str);
}
