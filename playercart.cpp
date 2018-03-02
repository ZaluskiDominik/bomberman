#include "playercart.h"
#include <QPainter>
#include "otherFunctions.h"

//slots for players
playerCart* players[4];

int playerCart::playersCounter=0, playerCart::addedPlayers=0;

playerCart::playerCart(QWidget *parent)
    :QWidget(parent)
{
    playersCounter++;

    //new slot cart is empty first(player can be added)
    playerAdded=false;

    //create frame around player's slot cart
    set_frame();

    //create empty slots's widgets inside the frame
    set_emptySlot();

    //connect slot which will react on clicking the button for adding player
    connect (addButton, SIGNAL(clicked(bool)), this, SLOT(onAddPlayer()));
}

void playerCart::set_frame()
{
    //allocate
    playerFrame=new QFrame(this);
    frameLayout=new QVBoxLayout;

    //set frame's look
    playerFrame->setLayout(frameLayout);
    playerFrame->setLineWidth(3);
    playerFrame->setMidLineWidth(3);
    playerFrame->setFrameStyle(QFrame::Box | QFrame::Raised);
    playerFrame->show();
}

void playerCart::set_emptySlot()
{
    //emptySlot label, add player button
    QLabel* emptySlot=new QLabel;

    //set emptySlot label's font
    QFont f;
    f.setBold(true);
    f.setPointSize(18);
    emptySlot->setFont(f);
    emptySlot->setText("Empty slot");

    addButton=new QPushButton("Add player");
    addButton->setIcon(QIcon(":/images/img/add_player.png"));
    addButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    //add button for adding players and emptySlot label to frame layout
    frameLayout->addWidget(emptySlot, 0, Qt::AlignCenter);
    frameLayout->addWidget(addButton, 0, Qt::AlignCenter);
}

void playerCart::set_closeButton()
{
    closeCart=new QPushButton;
    //set icon
    closeCart->setIcon(QIcon(":/images/img/close.png"));

    //set fixed size 16x16
    closeCart->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    closeCart->setFixedSize(16, 16);
    closeCart->setIconSize(QSize(16, 16));

    connect (closeCart, SIGNAL(clicked(bool)), this, SLOT(onCartClosed()));

    //add button to frame layout
    frameLayout->addWidget(closeCart, 0, Qt::AlignRight);
}

void playerCart::set_playerImage_label()
{
    playerImage=new QLabel;
    playerImage->setScaledContents(true);
    frameLayout->addWidget(playerImage);
}

void playerCart::set_playerData_labels()
{
    //name label
    nameLabel=new QLabel;
    nameLabel->setText("Name:");
    playerData->addWidget(nameLabel, 0, 0);

    //color label
    colorLabel=new QLabel;
    colorLabel->setText("Color: ");
    playerData->addWidget(colorLabel, 1, 0);
}

//changes player's image
void playerCart::change_player_image(QString color)
{
    QPixmap p=color_player(string_to_playerColor(color), ":/images/img/players/white/Front/Bman_F_f00.png");
    playerImage->setPixmap(p.copy(0, 30, p.width(), p.height()).scaled(playerImage->width(), playerImage->height()));
}

//initialize combo box
void playerCart::set_color_box()
{
    colorBox=new QComboBox;

    //add to frame layout
    playerData->addWidget(colorBox, 1, 1);
    //fill with colors
    add_colors();

    //set player's color as first available from combo box
    color=colorBox->currentText();

    //connect slot reacting on change of color
    connect (colorBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onColorChanged()));
}

//ad colors to combo box
void playerCart::add_colors()
{
    QString colors[4]={"white", "silver", "green", "yellow"};
    int i, j;

    //loop through all colors
    for (i=0 ; i<4 ; i++)
    {
        for (j=0 ; j<playersCounter && players[j]->colorBox->currentText()!=colors[i] ; j++);
        //if previous players didn't choose that color add it to comco box
        if (j==playersCounter)
            colorBox->addItem(colors[i]);
    }
}

void playerCart::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.fillRect(0, 0, width(), height(), QColor(211, 211, 211));
    //resizing frame
    playerFrame->resize(width(), height());
}

//SLOTS
//*******************************************************************************************************

void playerCart::onAddPlayer()
{
    //delete emptySlot layout
    frameLayout->deleteLater();
    playerFrame->deleteLater();
    set_frame();

    //close button
    set_closeButton();
    //player image
    set_playerImage_label();

    //add layout for widgets with player data
    playerData=new QGridLayout;
    frameLayout->addLayout(playerData);

    //combo box with colors
    set_color_box();

    //change player image based on selected color
    change_player_image(colorBox->currentText());

    //label for player's data
    set_playerData_labels();

    //line edit for typing player's name
    nameEdit=new QLineEdit;
    playerData->addWidget(nameEdit, 0, 1);
    //receive signal after user end typying
    connect (nameEdit, SIGNAL(editingFinished()), this, SLOT(onNameEntered()));

    //updating combo boxes of others players(other player can't choose the color of this player)
    for (int i=0 ; i<playersCounter-1 ; i++)
        players[i]->colorBox->removeItem(players[i]->colorBox->findText(color));

    //mark this cart as taken by player
    playerAdded=true;
    addedPlayers++;

    //if all players carts are less than 4 create new emptySlot cart
    if (playersCounter<4)
    {
        //right side will be calculated first, so the variable playersCounter will be incremented
        //it's why left side array's index is playersCounter-1
        players[playersCounter-1]=new playerCart(parentWidget());
        players[playersCounter-1]->show();
    }
}

//combo box index changed
void playerCart::onColorChanged()
{
    int i, j;
    for (i=0 ; players[i]->colorBox!=qobject_cast<QComboBox*>(sender()) ; i++);

    //check if removing player's cart didnt trigger index change
    if (players[i]->colorBox->currentText()==players[i]->color)
        return;

    for (j=0 ; j<playersCounter ; j++)
    {
        if (j!=i && players[j]->playerAdded)
        {
            //delete from other players' combo boxes the color which senderPlayer selected
            int index=players[j]->colorBox->findText(players[i]->colorBox->currentText());
            players[j]->colorBox->removeItem(index);

            //add old color of senderPlayer to other players' combo boxes
            players[j]->colorBox->addItem(players[i]->color);
        }
    }

    //set new player's color
    players[i]->color=players[i]->colorBox->currentText();
    //change player's image
    players[i]->change_player_image(players[i]->color);
}

//close button clicked
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
    if (playersCounter==4 && players[3]->playerAdded)
    {
        //add new empty cart
        players[3]=new playerCart(parentWidget());
        players[3]->show();
    }

    //add deleted player's color to other players combo boxes
    for (i=0 ; i<playersCounter-2 ; i++)
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

    //check if other player didn't already chose that name
    for (i=0 ; i<playersCounter && players[i]->playerAdded; i++)
    {
        if (players[i]->name==players[j]->nameEdit->text() && i!=j )
        {
            //that name is already taken, restore previous name
            sigSender->setText(players[j]->name);
            return;
        }
    }
    //change this player name
    players[j]->name=sigSender->text();
}
