#include "menubutton.h"
#include <QPainter>
#include <QGraphicsBlurEffect>
#include <QLinearGradient>

extern int volume;

menuButton::menuButton(QWidget *parent, QString text)
    :QAbstractButton(parent), str(text)
{
    //set button's background as transparent
    setStyleSheet("background: transparent");
    textColor=Qt::black;
    in=paintBackground=false;
    //set path of click sound
    clickPlayer.setMedia(QMediaContent(QUrl("qrc:/sounds/sounds/button_click.mp3")));
    clickPlayer.setVolume(volume);

    setup_font();
}

void menuButton::set_text_color(const QColor &color)
{
    textColor=color;
}

void menuButton::set_background_color(const QColor &color)
{
    paintBackground=true;
    backColor=color;
}

void menuButton::change_volume(int vol)
{
    clickPlayer.setVolume(vol);
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
    //mouse cursor left the geometry of the button
    in=false;
}

void menuButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    if (in)
    {
        //mouse hover over
        p.fillRect(QRect(0, 0, width(), height()), create_gradient(QColor(255,165,0)));
    }
    else if (paintBackground)
        p.fillRect(QRect(0, 0, width(), height()), create_gradient(backColor));

    //draw button's frame
    QPen pen(Qt::green);
    pen.setWidth(8);
    p.setPen(pen);
    p.drawRect(0, 0, width(), height());

    //change font size
    font.setPixelSize(width()*0.13);
    p.setFont(font);

    //draw button's text
    pen.setColor(textColor);
    p.setPen(pen);
    p.drawText(0, 0, width(), height(), Qt::AlignCenter, str);
}

void menuButton::setup_font()
{
    font.setBold(true);
    font.setFamily("Ravie");
    font.setLetterSpacing(QFont::AbsoluteSpacing, 6);
    //apply blur effect
    QGraphicsBlurEffect* effect=new QGraphicsBlurEffect(this);
    effect->setBlurRadius(3);
    setGraphicsEffect(effect);
}

QLinearGradient menuButton::create_gradient(QColor color)
{
    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, color.lighter(130));
    gradient.setColorAt(1, color.lighter(80));
    return gradient;
}
