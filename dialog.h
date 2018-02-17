#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFrame>
#include <QMediaPlayer>
#include <QPushButton>
#include "menubutton.h"
#include "playercart.h"
#include "game.h"
#include "structures.h"

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent*);
    void keyPressEvent(QKeyEvent *e);

private:
    const int numMenu;
    //menu buttons(play, settings, quit)
    menuButton* menuButtons[3];

    //frame for menu buttons
    QFrame* menuFrame;

    //background music player
    QMediaPlayer menuMusic;

    //button to show slider which changes the level of music's volume
    QPushButton* volumeButton;

    //keep track in which menu user is currently in
    QString currentLocation;

    //lobby button for starting the game
    QPushButton* startButton;

    //game's window with the graphics scene
    game* gameWnd;

    //********FUNCTIONS********

    //create main menu with buttons
    void create_main_manu();
    //draw main menu
    void draw_main_menu(QPainter &p);
    //draw menu buttons
    void draw_menuButtons();

    //set up music's volume button
    void setup_volumeButton();

    //clear main menu's window layout
    void erase_main_menu();

    //draw lobby with slots for players
    void draw_lobby(QPainter &p);

    //clear lobby's window layout
    void erase_lobby();

    //export player data to game
    void export_playersData(playerData *playersData);

private slots:
    //main menu buttons slots
    void onPlayClicked();
    void onSettingsClicked();
    void onExitClicked();

    //music loop
    void onPosChanged();

    //start game
    void onStartGameClicked();

    //game has come to end
    void onGameEnded();

};

#endif // DIALOG_H
