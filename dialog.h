#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFrame>
#include <QMediaPlayer>
#include <QPushButton>
#include "menubutton.h"
#include "playercart.h"
#include "game.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

protected:
    void paintEvent(QPaintEvent*);
    void keyPressEvent(QKeyEvent *e);

private:
    Ui::Dialog *ui;
    //menu buttons
    menuButton* chooseButtons[menuButton::buttons];
    //box for buttons
    QFrame* menuFrame;
    //background music
    QMediaPlayer menuMusic;
    QPushButton* volumeButton;
    QString currentLocation;    //which menu
    QPushButton* startButton;
    game* gameWnd;

    //display main menu
    void create_main_manu();
    void erase_main_menu();

private slots:
    //main menu buttons slots
    void onPlayClicked();
    void onSettingsClicked();
    void onExitClicked();

    //music loop
    void onPosChanged();

    void onStartGameClicked();
    void onGameEnded();

};

#endif // DIALOG_H
