#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMediaPlayer>
#include <QGroupBox>
#include <QFormLayout>
#include "menubutton.h"
#include "playercart.h"
#include "keyssettingscart.h"
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
    void keyPressEvent(QKeyEvent* e);

private:
    //number of menus
    const int numMenu;
    //menu buttons(play, settings, quit)
    menuButton* menuButtons[3];

    //title "bomberman"
    QLabel* titleLabel;

    //keep track in which menu user is currently in
    QString currentLocation;

    //button for starting the game in lobby location
    menuButton* startButton;

    //game's window with the graphics scene
    game* gameWnd;

    //settings menu
    QGroupBox* settingsBox;
    menuButton* returnButton;
    menuButton* keySettingsButton;

    //keys settings menu
    keysSettingsCart* settingsCarts[4];
    menuButton* applyButton;

    //player's settings
    playerData data[4];

    //load players' key settings from file
    void load_players_settings();

    //if file with settings doesn't exist
    void load_default_settings();

    //main menu
    //************************************************************

    void draw_background(QPainter& p);

    //create main menu with buttons
    void create_main_menu();

    //clear main menu's window
    void erase_main_menu();

    void create_title_label();

    void draw_title_label();

    //draw main menu's buttons
    void draw_menuButtons();

    //draw all main menu's widgets
    void draw_main_menu(QPainter &p);

    //lobby
    //*******************************************************************

    void create_lobby();

    void create_startButton();

    //draw lobby with slots(carts) for players to choose their names and colors
    void draw_lobby(QPainter &p);

    //clear lobby's window layout
    void erase_lobby();

    void prepare_playersData_to_export();

    //settings menu
    //*******************************************************************
    void create_settings_menu();

    void erase_settings_menu();

    void create_settings_groupBox();

    QLabel* create_label_for_option(QString title);
    void create_volume_option(QFormLayout *settingsLayout);
    void create_resize_option(QFormLayout *settingsLayout);
    void create_lifes_option(QFormLayout* settigsLayout);

    void create_returnButton();

    void draw_settings_menu(QPainter& p);

    //keys settings menu
    //*******************************************************************
    void create_keys_settings_menu();
    void erase_keys_settings_menu();
    void draw_keys_settings_menu(QPainter& p);

    //buttons after clicking provide option for changing a key
    void create_change_key_carts();
    void create_apply_button();
    //resize apply and return buttons
    void resize_buttons_in_keys_setings();

    void save_players_settings(std::ofstream& file);

private slots:
    //go to lobby
    void onPlayClicked();

    //got to settings menu
    void onSettingsClicked();

    //exit from the app
    void onExitClicked();

    //start game
    void onStartGameClicked();

    //game has come to the end
    void onGameEnded();

    //user changed field size
    void onSpinBoxSizeValueChanged(int value);

    //number of lifes changed
    void onSpinBoxLifesValueChanged(int value);

    //user changed sound's volume
    void onSliderValueChanged(int value);

    //return to previous menu
    void onReturnClicked();

    //go to keys settings menu
    void onKeySettingsButtonClicked();

    //save keys
    void onApplyButtonClicked();

};

#endif // DIALOG_H
