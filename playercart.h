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
    void set_player_image(const QString playerColor);

    //creates combo box with available player's colors
    void set_color_box();

    void set_frame();
    //create empty slots's widgets inside the frame
    void set_emptySlot();

private slots:
    void onAddPlayer();
    void onColorChanged();  //combo box slot
    void onCartClosed();
    //player changed name
    void onNameEntered();

};

#endif // PLAYERCART_H
