#ifndef MENUBUTTON_H
#define MENUBUTTON_H
#include <QObject>
#include <QAbstractButton>
#include <QMediaPlayer>

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
    const QString str;  //button title
    bool in;        //cursor hover over

    //music players
    QMediaPlayer clickPlayer;

};

#endif // MENUBUTTON_H
