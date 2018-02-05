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

    QString playerColor()
    {
        return color;
    }

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
    static int playersCounter;
    QString name, color;
    bool playerAdded;

    //player frame
    QFrame* playerFrame;
    QVBoxLayout* frameLayout;
    QPushButton* addButton;     //adds player
    QPushButton* closeCart;

    //new player
    QLabel* playerImage;
    QLabel *nameLabel, *colorLabel;
    QComboBox* colorBox;
    QLineEdit* nameEdit;
    QGridLayout* playerData;


    void setPlayerImage(const QString playerColor);
    void setColorBox();
    void setFrame();

private slots:
    void onAddPlayer();
    void onColorChanged();  //combo box slot
    void onCartClosed();
    //player changed name
    void onNameEntered();

};

#endif // PLAYERCART_H
