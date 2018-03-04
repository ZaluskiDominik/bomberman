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

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);

private:
    //button's title
    const QString str;

    //whether mouse cursor is over the button
    bool in;

    //music player
    QMediaPlayer clickPlayer;

};

#endif // MENUBUTTON_H
