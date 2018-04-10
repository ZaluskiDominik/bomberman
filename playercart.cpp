#include "playercart.h"
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include "otherFunctions.h"

//slots for players
playerCart* players[4];

int playerCart::playersCounter=0, playerCart::addedPlayers=0;

playerCart::playerCart(QWidget *parent)
    :QWidget(parent)
{
    playersCounter++;
    //new slot cart is created as empty cart(player can be added)
    playerAdded=false;

    //create frame around player's slot cart
    create_frame();
    //create empty slot
    create_emptySlot();

    //connect slot which will react on clicking the button for adding player
    QObject::connect(addButton, SIGNAL(clicked(bool)), this, SLOT(onAddPlayer()));
}

void playerCart::create_frame()
{
    playerFrame=new QFrame(this);
    frameLayout=new QVBoxLayout;

    //set frame's look
    playerFrame->setLayout(frameLayout);
    playerFrame->setLineWidth(3);
    playerFrame->setMidLineWidth(3);
    playerFrame->setFrameStyle(QFrame::Box | QFrame::Raised);
    playerFrame->show();
}

void playerCart::create_emptySlot()
{
    create_empty_label();

    //button for adding player
    addButton=new QPushButton("Add player");
    addButton->setIcon(QIcon(":/images/img/add_player.png"));
    addButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    frameLayout->addWidget(addButton, 0, Qt::AlignCenter);
}

void playerCart::create_empty_label()
{
    //label with text: "empty slot"
    QLabel* emptySlot=new QLabel;
    QFont f;
    f.setBold(true);
    f.setPointSize(22);
    emptySlot->setFont(f);
    setStyleSheet("color: #991144");

    //apply drop shadow effect
    QGraphicsDropShadowEffect* effect=new QGraphicsDropShadowEffect(this);
    effect->setOffset(5, 5);
    effect->setBlurRadius(4);
    effect->setColor(Qt::blue);
    emptySlot->setGraphicsEffect(effect);

    emptySlot->setText("Empty slot");
    frameLayout->addWidget(emptySlot, 0, Qt::AlignCenter);
}

//slot taken
//*************************************************************************************

void playerCart::create_data_section()
{
    //create layout for player's data
    playerDataLayout=new QGridLayout;
    frameLayout->addLayout(playerDataLayout);

    //create combo box with colors
    create_color_box();

    change_player_image(colorBox->currentText());

    create_playerData_labels();

    create_nameEdit();
}

void playerCart::create_closeButton()
{
    closeCart=new QPushButton;
    closeCart->setIcon(QIcon(":/images/img/close.png"));
    //set fixed size 16x16
    closeCart->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    closeCart->setFixedSize(16, 16);
    closeCart->setIconSize(QSize(16, 16));
    //add button to frame layout
    frameLayout->addWidget(closeCart, 0, Qt::AlignRight);
    QObject::connect(closeCart, SIGNAL(clicked(bool)), this, SLOT(onCartClosed()));
}

void playerCart::create_playerImage_label()
{
    playerImage=new QLabel;
    playerImage->setScaledContents(true);
    frameLayout->addWidget(playerImage);
}

void playerCart::create_playerData_labels()
{
    //name label
    nameLabel=new QLabel;
    nameLabel->setText("Name:");
    playerDataLayout->addWidget(nameLabel, 0, 0);

    //color label
    colorLabel=new QLabel;
    colorLabel->setText("Color: ");
    playerDataLayout->addWidget(colorLabel, 1, 0);
}

void playerCart::create_nameEdit()
{
    nameEdit=new QLineEdit;
    //set max name's length
    nameEdit->setMaxLength(12);
    playerDataLayout->addWidget(nameEdit, 0, 1);
    //receive signal after user end typying
    QObject::connect(nameEdit, SIGNAL(editingFinished()), this, SLOT(onNameEntered()));
}

void playerCart::change_player_image(QString color)
{
    //color player's helmet
    QPixmap p=color_player(string_to_playerColor(color), ":/images/img/players/white/Front/Bman_F_f00.png");
    //scale pixmap
    playerImage->setPixmap(p.scaled(playerImage->width(), playerImage->height()));
}

void playerCart::create_color_box()
{
    colorBox=new QComboBox;

    //add to frame layout
    playerDataLayout->addWidget(colorBox, 1, 1);
    //add colors to combo box
    add_colors();

    //set player's color as first available from combo box
    color=colorBox->currentText();

    //connect slot reacting on change of color
    connect (colorBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onColorChanged()));
}

//add colors to combo box
void playerCart::add_colors()
{
    QString colors[4]={"white", "blue", "green", "yellow"};
    int i, j;

    //loop through all colors
    for (i=0 ; i<4 ; i++)
    {
        for (j=0 ; j<playersCounter && players[j]->colorBox->currentText()!=colors[i] ; j++);
        //if previous players didn't selected that color, add it to combo box
        if (j==playersCounter)
            colorBox->addItem(colors[i]);
    }
}

void playerCart::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    //draw background gradient
    QLinearGradient grad(0, 0, 0, height());
    grad.setColorAt(0.0, QColor(211, 211, 211));
    grad.setColorAt(0.3, QColor("#AAAAAA"));
    grad.setColorAt(0.7, QColor("#999999"));
    grad.setColorAt(1.0, QColor("#777777"));
    p.fillRect(0, 0, width(), height(), grad);

    //resize frame
    playerFrame->resize(width(), height());
}

//SLOTS
//*******************************************************************************************************

void playerCart::onAddPlayer()
{
    //delete previous layout of the empty cart
    frameLayout->deleteLater();
    playerFrame->deleteLater();
    //create new frame
    create_frame();

    //add closeButton at top right corner
    create_closeButton();
    create_playerImage_label();

    create_data_section();

    //update combo boxes of others players(other player shouldn't be able to choose the color of this player)
    for (int i=0 ; i<playersCounter-1 ; i++)
        players[i]->colorBox->removeItem(players[i]->colorBox->findText(color));

    //mark this slot as taken by the player
    playerAdded=true;
    addedPlayers++;

    //if the number of player carts is less than 4 create new emptySlot cart
    if (playersCounter<4)
    {
        //right side will be calculated first, so the variable playersCounter will be incremented
        //it's why array's index equals playersCounter-1
        players[playersCounter-1]=new playerCart(parentWidget());
        players[playersCounter-1]->show();
    }
}

void playerCart::onColorChanged()
{
    int i, j;
    QComboBox* senderBox=qobject_cast<QComboBox*>(sender());
    for (i=0 ; players[i]->colorBox!=senderBox ; i++);

    //check if removing player's cart triggered index change
    if (senderBox->currentText()==players[i]->color)
        return;

    for (j=0 ; j<addedPlayers ; j++)
    {
        if (i!=j)
        {
            //delete from other players' combo boxes the color which senderPlayer selected
            int index=players[j]->colorBox->findText(senderBox->currentText());
            players[j]->colorBox->removeItem(index);

            //add old color of senderPlayer to other players' combo boxes
            players[j]->colorBox->addItem(players[i]->color);
        }
    }

    //set new player's color
    players[i]->color=senderBox->currentText();
    //change player's image
    players[i]->change_player_image(players[i]->color);
}

void playerCart::onCartClosed()
{
    QPushButton *sigSender=qobject_cast<QPushButton*>(sender());
    playerCart* cartToDelete;
    int j, i;

    //find array's index of player's cart which will be deleted
    for (i=0 ; players[i]->closeCart!=sigSender ; i++);
    //save cart to futher delete
    cartToDelete=players[i];

    //move all carts with indexes greater than the index of cart that will be deleted one position left in array
    for (j=i ; j+1<playersCounter ; j++)
        players[j]=players[j+1];

    //if all players' slots were taken
    if (addedPlayers==4)
    {
        //add new empty cart
        players[3]=new playerCart(parentWidget());
        players[3]->show();
    }

    //add deleted player's color to other players combo boxes
    for (i=0 ; i<addedPlayers-1 ; i++)
        players[i]->colorBox->addItem(cartToDelete->colorBox->currentText());

    //delete the player's cart
    cartToDelete->deleteLater();
}

void playerCart::onNameEntered()
{
    int i, j;
    //who is the sender of this signal
    QLineEdit* sigSender=qobject_cast<QLineEdit*>(sender());
    //find his index in players array
    for (j=0 ; players[j]->nameEdit!=sigSender ; j++);

    //check if other player didn't already seleted that name
    for (i=0 ; i<addedPlayers; i++)
    {
        if ( (players[i]->name==sigSender->text() && i!=j) || sigSender->text()=="")
        {
            //that name is already taken, restore previous name
            sigSender->setText(players[j]->name);
            return;
        }
    }

    //change this player name
    players[j]->name=sigSender->text();
}
