#ifndef PLAYERCART_H
#define PLAYERCART_H

#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class playerCart : public QWidget
{
    Q_OBJECT
public:
    explicit playerCart(QWidget *parent = nullptr);

    ~playerCart()
    {
        //decrement the number of playerCarts
        playersCounter--;

        //if this slot was taken by a player
        if (playerAdded)
            addedPlayers--;
    }

    //return player's color
    QString playerColor()
    {
        return color;
    }

    //return player's name
    QString playerName()
    {
        return name;
    }

    //return the number of playerCarts
    static int playersCount()
    {
        return playersCounter;
    }

    //return the number of playerCarts that are not empty(slot was taken by a player)
    static int playersAddedCount()
    {
        return addedPlayers;
    }

protected:
    void paintEvent(QPaintEvent*);

private:
    static int playersCounter, addedPlayers;

    //player's data
    QString name, color;

    //whether addButton was clicked(slot is taken by a player)
    bool playerAdded;

    //frame around the cart
    QFrame* playerFrame;
    QVBoxLayout* frameLayout;

    //button for taking slot by a player
    QPushButton* addButton;
    //button for removing the player's slot
    QPushButton* closeCart;

    //label for player's image
    QLabel* playerImage;

    //labels for player's data
    QLabel *nameLabel, *colorLabel;

    //player can select his color from combo box
    QComboBox* colorBox;

    //line edit for typying player's name
    QLineEdit* nameEdit;

    //layout containing player's image, name, color
    QGridLayout* playerDataLayout;

    //***********************************************************************

    //create frame around the cart
    void create_frame();

    //create empty slots, player can be added after clicking addButton
    void create_emptySlot();

    void create_empty_label();

    //slot taken
    //*************************************************************************

    //create bottom part of the cart(the part where user can change his data)
    void create_data_section();

    //creates button for deleting player cart
    void create_closeButton();

    //set label containing player's image
    void create_playerImage_label();

    //change player's image based on selected color
    void change_player_image(QString color);

    //create name label and color label
    void create_playerData_labels();

    void create_nameEdit();

    //create combo box with colors
    void create_color_box();

    //add colors to combo box that are available to choose
    void add_colors();

private slots:
    //addPlayer button has been clicked
    void onAddPlayer();

    //player changed color in combo box
    void onColorChanged();

    //closeCart button has been clicked
    void onCartClosed();

    //player changed name
    void onNameEntered();

};

#endif // PLAYERCART_H
