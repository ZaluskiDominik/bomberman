#ifndef MENUBUTTON_H
#define MENUBUTTON_H
#include <QObject>
#include <QAbstractButton>
#include <QMediaPlayer>

class menuButton : public QAbstractButton
{
    Q_OBJECT
public:
    const static int buttons=3;     //counter of buttons

    menuButton(QWidget *parent, QString text);
    QSize sizeHint() const;
    static void frameSizeChanged(QSize size);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);

private:
    const QString str;  //button title
    bool in;        //mouse focus
    static int parentWidth, parentHeight, spacing;     //adjusting geometry to frame

    //music players
    QMediaPlayer clickPlayer;

};

#endif // MENUBUTTON_H
