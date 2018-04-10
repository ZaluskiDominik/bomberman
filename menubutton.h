#ifndef MENUBUTTON_H
#define MENUBUTTON_H
#include <QObject>
#include <QAbstractButton>
#include <QMediaPlayer>

//class representing menu button

class menuButton : public QAbstractButton
{
    Q_OBJECT
public:
    menuButton(QWidget *parent, QString text);

    void set_text_color(const QColor& color);

    void set_background_color(const QColor& color);

    void change_volume(int vol);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);

private:
    //button's title
    const QString str;
    QColor textColor, backColor;
    bool paintBackground;
    QFont font;

    //whether mouse cursor is over the button
    bool in;

    //music player
    QMediaPlayer clickPlayer;

    //button's text font
    void setup_font();

    QLinearGradient create_gradient(QColor color);

};

#endif // MENUBUTTON_H
