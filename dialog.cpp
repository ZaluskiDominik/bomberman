#include "dialog.h"
#include <QPainter>
#include <QKeyEvent>
#include <QCoreApplication>

//player slot carts in lobby
extern playerCart* players[4];

Dialog::Dialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), numMenu(3)
{
    //set window title and resolution
    setWindowTitle("Bomberman");
    setMinimumSize(600, 700);

    //display main menu
    create_main_manu();

    //set background music's path
    menuMusic.setMedia(QMediaContent(QUrl("qrc:/sounds/sounds/menu_music.wav")));

    //connect slot keeping track of music players's position
    connect (&menuMusic, SIGNAL(positionChanged(qint64)), this, SLOT(onPosChanged()));
    menuMusic.play();
}

void Dialog::erase_main_menu()
{
    //delete buttons
    for (int i=0 ; i<numMenu ; i++)
        menuButtons[i]->deleteLater();

    //delete menu's frame and volume button
    menuFrame->deleteLater();
    volumeButton->deleteLater();
}

void Dialog::create_main_manu()
{
    currentLocation="mainmenu";
    //create menu's frame
    menuFrame=new QFrame(this);
    menuFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    menuFrame->setLineWidth(3);

    //create menu buttons
    QString text[numMenu]={ "Play", "Settings", "Exit" };
    for (int i=0 ; i<numMenu ; i++)
    {
        menuButtons[i]=new menuButton(this, text[i]);
        menuButtons[i]->show();
    }

    setup_volumeButton();

    //connect slots which will react on clicking menu buttons
    connect (menuButtons[0], SIGNAL(clicked(bool)), this, SLOT(onPlayClicked()));
    connect (menuButtons[1], SIGNAL(clicked(bool)), this, SLOT(onSettingsClicked()));
    connect (menuButtons[2], SIGNAL(clicked(bool)), this, SLOT(onExitClicked()));

    update();
}

void Dialog::setup_volumeButton()
{
    //create volume button
    volumeButton=new QPushButton(this);
    volumeButton->setGeometry(20, 20, 50, 50);
    volumeButton->setIcon(QIcon(":/images/img/volume.png"));
    volumeButton->setIconSize(QSize(50, 50));
    //set its look as flat
    volumeButton->setFlat(true);
}

//*************************** PAINT EVENT ***************************

void Dialog::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    if (currentLocation=="mainmenu")
        draw_main_menu(p);
    else if (currentLocation=="lobby")
        draw_lobby(p);
}

void Dialog::draw_menuButtons()
{
    //resize frame containing menu buttons
    int frameW=0.5 * width(), frameH=0.5 * height();
    menuFrame->setGeometry(width()/2 - (frameW / 2), height()/2 - (frameH / 2), frameW, frameH);

    //resize menu buttons
    //calculate their size
    int spaceBetween=0.05 * menuFrame->height(), verticalMargin=0.1 * menuFrame->height();
    int h=(menuFrame->height() - ((numMenu - 1) * spaceBetween) - (2 * verticalMargin)) / numMenu, w=menuFrame->width() - 30;
    for (int i=0 ; i<numMenu ; i++)
        menuButtons[i]->setGeometry(menuFrame->pos().x() + 15, menuFrame->pos().y() + verticalMargin + (i * (h + spaceBetween)), w, h);
}

void Dialog::draw_main_menu(QPainter& p)
{
    //draw background
    QImage background(":/images/img/menuBackground.png");
    background=background.scaled(width(), height());
    p.drawImage(0, 0, background);

    //set title "bomberman"
    p.setFont(QFont("Ravie", 20));
    p.setPen(Qt::red);
    p.drawText(width()/2 - 150, 40, 300, 100, Qt::AlignCenter, "Bomberman");

    //draw menu buttons
    draw_menuButtons();
}

void Dialog::draw_lobby(QPainter& p)
{
    //draw lobby's background
    QImage lobbyBackImg(":/images/img/choosing_players.jpg");
    lobbyBackImg=lobbyBackImg.scaled(width(), height());
    p.drawImage(0, 0, lobbyBackImg);

    //resize startGame button
    startButton->setGeometry(width()/2 - 100, height() - 90, 200, 80);

    //resize slots for players
    int i, cartH=(height() - 25 - 90)/2, cartW=(width() - 25)/2;
    for (i=0 ; i<playerCart::playersCount() ; i++)
        players[i]->setGeometry((i%2) * (cartW+5) + 10, (i/2) * (cartH+5) + 10, cartW, cartH);
}

//******************************************************************

//background music
void Dialog::onPosChanged()
{
    //loop background music
    if (menuMusic.position()>=45000)
    {
        menuMusic.setPosition(0);
        menuMusic.play();
    }
}

//MAIN MENU BUTTONS SLOTS
//***********************************************************

//go to lobby
void Dialog::onPlayClicked()
{
    currentLocation="lobby";
    //clear screen
    erase_main_menu();

    //create first slot for player
    players[0]=new playerCart(this);
    players[0]->show();

    //set startGame button
    startButton=new QPushButton(this);
    startButton->setText("Start Game");
    startButton->setStyleSheet("background-color: blue; font-size: 17pt; font-weight: bold; color: red;");
    startButton->show();
    connect (startButton, SIGNAL(clicked(bool)), this, SLOT(onStartGameClicked()));

    update();
}

void Dialog::onSettingsClicked()
{

}

//end appp
void Dialog::onExitClicked()
{
    QCoreApplication::exit(0);
}
//***********************************************************

//KEYBOARD
//***********************************************************

void Dialog::keyPressEvent(QKeyEvent *e)
{
    //ESC key for returning to main menu
    if (e->key()==Qt::Key_Escape)
    {
        if (currentLocation=="lobby")
            erase_lobby();

        //return to main menu
        create_main_manu();
    }
}

void Dialog::erase_lobby()
{
    //delete players's slot carts
    for (int i=0 ; i<playerCart::playersCount() ; i++)
        players[i]->deleteLater();

    //delete startGame button
    startButton->deleteLater();
}

void Dialog::export_playersData(playerData* playersData)
{
    for (int i=0 ; i<playerCart::playersAddedCount() ; i++)
    {
        playersData[i].color=players[i]->playerColor();
        playersData[i].name=players[i]->playerName();

        //temporary
        if (playersData[i].color!="red")
        {
            playersData[i].keys.up=Qt::Key_W;
            playersData[i].keys.down=Qt::Key_S;
            playersData[i].keys.left=Qt::Key_A;
            playersData[i].keys.right=Qt::Key_D;
            playersData[i].keys.bomb=Qt::Key_Tab;
        }
    }
}

//***********************************************************

//begin game
void Dialog::onStartGameClicked()
{
    //if number of players is less than 1 return
    if (playerCart::playersAddedCount()<1)
        return;

    //hide main menu's window
    hide();

    //stop music
    menuMusic.stop();

    playerData playersData[playerCart::playersAddedCount()];
    export_playersData(playersData);

    //shown game's window
    gameWnd=new game(this, QSize(1000, 600), playersData, playerCart::playersAddedCount());
    gameWnd->show();

    //connect slot witch will react on end of game
    connect(gameWnd, SIGNAL(destroyed(QObject*)), this, SLOT(onGameEnded()));
}

void Dialog::onGameEnded()
{
    //reappear main menu's window
    show();
}
