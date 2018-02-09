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
        playersCounter--;
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

    static int playersCount()
    {
        return playersCounter;
    }

protected:
    void paintEvent(QPaintEvent*);

private:
    //current number of slots
    static int playersCounter;

    //player's data
    QString name, color;

    //whether or not add player was clicked
    bool playerAdded;

    //frame around player's slot cart
    QFrame* playerFrame;
    QVBoxLayout* frameLayout;

    //add player button
    QPushButton* addButton;
    //close cart button --> remove player's slot
    QPushButton* closeCart;

    //label for player's image
    QLabel* playerImage;

    //labels for player's data
    QLabel *nameLabel, *colorLabel;

    //player can select his color from combo box
    QComboBox* colorBox;
    //line edit for typying player's name
    QLineEdit* nameEdit;

    //contains player's image, name, color
    QGridLayout* playerData;

    //FUNCTIONS*******************************************************
    //creates combo box with colors
    void set_color_box();

    //add colors to combo box that are available to choose
    void add_colors();

    void set_frame();
    //create empty slots's widgets inside the frame
    void set_emptySlot();

    //creates button for deleting player cart
    void set_closeButton();

    //set label containing player's image
    void set_playerImage_label();
    //change player's image based on selected color
    void change_player_image(const QString playerColor);

    void set_playerData_labels();

private slots:
    //addPlayer button clicked
    void onAddPlayer();

    //player change color in combo box
    void onColorChanged();

    //closeCart button has been clicked
    void onCartClosed();

    //player changed name
    void onNameEntered();

};

#endif // PLAYERCART_H
