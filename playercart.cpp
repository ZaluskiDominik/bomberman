#include "playercart.h"
#include <QPainter>

//slots for players
playerCart* players[4];
int playerCart::playersCounter=0;

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

//changes player's image
void playerCart::change_player_image(const QString playerColor)
{
    if (playerColor=="red")
        playerImage->setPixmap(QPixmap(":/images/img/red.png"));
    else if (playerColor=="blue")
        playerImage->setPixmap(QPixmap(":/images/img/blue.png"));
    else if (playerColor=="green")
        playerImage->setPixmap(QPixmap(":/images/img/red.png"));
    else if (playerColor=="yellow")
        playerImage->setPixmap(QPixmap(":/images/img/blue.png"));
}

//initialize combo box
void playerCart::set_color_box()
{
    colorBox=new QComboBox;

    //set player's color as first available from combo box
    color=colorBox->currentText();

    //add to frame layout
    playerData->addWidget(colorBox, 1, 1);
    //fill with colors
    add_colors();

    //connect slot reacting on change of color
    connect (colorBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onColorChanged()));
}

//ad colors to combo box
void playerCart::add_colors()
{
    QString colors[4]={"red", "blue", "green", "yellow"};
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

    nameLabel=new QLabel;
    colorLabel=new QLabel;

    nameEdit=new QLineEdit;

    playerData->addWidget(nameLabel, 0, 0);
    playerData->addWidget(nameEdit, 0, 1);
    playerData->addWidget(colorLabel, 1, 0);


    nameLabel->setText("Name:");
    colorLabel->setText("Color: ");



    //updating combo boxes of others players
    for (int i=0 ; i<playersCounter-1 ; i++)
        players[i]->colorBox->removeItem(players[i]->colorBox->findText(this->colorBox->currentText()));

    //connecting line edit with name to slot
    connect (nameEdit, SIGNAL(editingFinished()), this, SLOT(onNameEntered()));

    playerAdded=true;
    if (playersCounter<4)
    {
        //firs cumputing is right side
        players[playersCounter-1]=new playerCart(parentWidget());
        players[playersCounter-1]->show();
    }
}

//combo box index changed
void playerCart::onColorChanged()
{
    int i;
    for (i=0 ; players[i]->colorBox!=qobject_cast<QComboBox*>(sender()) ; i++);
    //checking if remove didnt triggered index change
    if (players[i]->colorBox->currentText()==players[i]->color)
        return;

    for (int j=0 ; j<playersCounter ; j++)
        if (j!=i && players[j]->playerAdded)
        {
            //deleting sender's new color color from combo boxes
            int index=players[j]->colorBox->findText(players[i]->colorBox->currentText());
            players[j]->colorBox->removeItem(index);
            //adding new color - old color of sender
            players[j]->colorBox->addItem(players[i]->color);
        }
    players[i]->color=players[i]->colorBox->currentText();
    //change image
    players[i]->change_player_image(players[i]->color);
}

//close button clicked
void playerCart::onCartClosed()
{
    QPushButton *sigSender=qobject_cast<QPushButton*>(sender());
    playerCart* cart;
    int j, i;
    //which cart will be deleted
    for (i=0 ; players[i]->closeCart!=sigSender ; i++);
    cart=players[i];

    for (j=i ; j+1<playersCounter ; j++)
        players[j]=players[j+1];

    if (playersCounter==4 && players[3]->playerAdded)
    {
        players[3]=new playerCart(parentWidget());
        players[3]->show();
    }
    //adding deleted color to combo boxes
    for (i=0 ; i<playersCounter-2 ; i++)
        players[i]->colorBox->addItem(cart->colorBox->currentText());

    cart->deleteLater();
}

void playerCart::onNameEntered()
{
    int i, j;
    QLineEdit* sigSender=qobject_cast<QLineEdit*>(sender());
    for (j=0 ; players[j]->nameEdit!=sigSender ; j++);

    for (i=0 ; i<playersCounter-1 && players[j]->name!=players[i]->nameEdit->text(); i++);
    if (i==playersCounter-1)
    {
        //other player already has this name
        sigSender->setText(players[j]->name);
    }
}
