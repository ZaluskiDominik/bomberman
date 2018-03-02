#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFrame>
#include <QMediaPlayer>
#include <QPushButton>
#include "menubutton.h"
#include "playercart.h"
#include "game.h"
#include "otherFunctions.h"

//class representing the window with the game's main menu

class menu : public QDialog
{
    Q_OBJECT

public:
    explicit menu(QWidget *parent = nullptr);

protected:
    //handle drawing and resizing
    void paintEvent(QPaintEvent*);
    //handle pressing keyboard's keys
    void keyPressEvent(QKeyEvent *e);

private:
    //number of menus
    const int numMenu;
    //menu buttons(play, settings, quit)
    menuButton* menuButtons[3];

    //background music player
    QMediaPlayer menuMusic;

    //button, which clicked, shows slider for changing the level of music's volume
    QPushButton* volumeButton;

    //keep track in which menu user is currently in
    QString currentLocation;

    //button for starting the game in lobby location
    QPushButton* startButton;

    //game's window with the graphics scene
    game* gameWnd;

    //************************************************************

    //create main menu with buttons
    void create_main_manu();

    //draw main menu
    void draw_main_menu(QPainter &p);

    //draw menu buttons
    void draw_menuButtons();

    //create music's volume button
    void setup_volumeButton();

    //clear main menu's window
    void erase_main_menu();

    //draw lobby with slots(carts) for players to choose their names and colors
    void draw_lobby(QPainter &p);

    //clear lobby's window layout
    void erase_lobby();

    //export player data obtained in playerCart class to game class
    void export_playersData(playerData *playersData);

private slots:
    //go to lobby
    void onPlayClicked();

    //got to settings menu
    void onSettingsClicked();

    //exit from the app
    void onExitClicked();

    //activated while advancing in background music
    void onPosChanged();

    //start game
    void onStartGameClicked();

    //game has come to the end
    void onGameEnded();

};

#endif // DIALOG_H
