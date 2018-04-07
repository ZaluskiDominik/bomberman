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

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);

private:
    //button's title
    const QString str;
    QColor textColor;
    QFont font;

    //whether mouse cursor is over the button
    bool in;

    //music player
    QMediaPlayer clickPlayer;

    void setup_font();

};

#endif // MENUBUTTON_H
