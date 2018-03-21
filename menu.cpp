#include "menu.h"
#include <QPainter>
#include <QKeyEvent>
#include <QCoreApplication>

//player slots in lobby(max 4 players)
extern playerCart* players[4];

menu::menu(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint), numMenu(3)
{
    //set the window's title and the window's resolution
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

void menu::erase_main_menu()
{
    //delete menu buttons
    for (int i=0 ; i<numMenu ; i++)
        menuButtons[i]->deleteLater();

    //delete menu's volume button
    volumeButton->deleteLater();
}

void menu::create_main_manu()
{
    //change current location on mainmenu
    currentLocation="mainmenu";

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

    //update screen
    update();
}

void menu::setup_volumeButton()
{
    //create volume button
    volumeButton=new QPushButton(this);
    volumeButton->setGeometry(20, 20, 50, 50);

    //set the button's icon
    volumeButton->setIcon(QIcon(":/images/img/volume.png"));
    volumeButton->setIconSize(QSize(50, 50));
    //it will look as flat
    volumeButton->setFlat(true);
}

//*************************** PAINT EVENT ***************************

void menu::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    if (currentLocation=="mainmenu")    //draw main menu
        draw_main_menu(p);
    else if (currentLocation=="lobby")  //draw lobby menu
        draw_lobby(p);
}

void menu::draw_menuButtons()
{
    //calculate a new size for menu buttons
    int spaceBetween=0.025 * height();  //space between the buttons
    int h=( height()*0.6 - ((numMenu - 1) * spaceBetween) )/numMenu, w=width()/2;
    for (int i=0 ; i<numMenu ; i++)
        menuButtons[i]->setGeometry(w/2, height()*0.2 + (i * (h + spaceBetween)), w, h);
}

void menu::draw_main_menu(QPainter& p)
{
    //draw the background
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

void menu::draw_lobby(QPainter& p)
{
    //draw lobby's background
    QImage lobbyBackImg(":/images/img/choosing_players.jpg");
    lobbyBackImg=lobbyBackImg.scaled(width(), height());
    p.drawImage(0, 0, lobbyBackImg);

    //resize the startGame button
    startButton->setGeometry(width()/2 - 100, height() - 90, 200, 80);

    //resize slots for players
    int cartH=(height() - 25 - 90)/2, cartW=(width() - 25)/2;
    for (int i=0 ; i<playerCart::playersCount() ; i++)
        players[i]->setGeometry((i%2) * (cartW+5) + 10, (i/2) * (cartH+5) + 10, cartW, cartH);
}

//******************************************************************

//background music
void menu::onPosChanged()
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

void menu::onPlayClicked()
{
    //change current location on lobby menu
    currentLocation="lobby";
    //clear screen
    erase_main_menu();

    //create a first empty slot for player
    players[0]=new playerCart(this);
    players[0]->show();

    //create startGame button
    startButton=new QPushButton(this);
    startButton->setText("Start Game");
    startButton->setStyleSheet("background-color: blue; font-size: 17pt; font-weight: bold; color: red;");
    startButton->show();
    connect (startButton, SIGNAL(clicked(bool)), this, SLOT(onStartGameClicked()));

    update();
}

void menu::onSettingsClicked()
{

}

void menu::onExitClicked()
{
    QCoreApplication::exit(0);
}

//KEYBOARD
//***********************************************************

void menu::keyPressEvent(QKeyEvent *e)
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

void menu::erase_lobby()
{
    //delete all player's slots carts
    for (int i=0 ; i<playerCart::playersCount() ; i++)
        players[i]->deleteLater();

    //delete startGame button
    startButton->deleteLater();
}

//TO DO after adding setting menu
void menu::export_playersData(playerData* playersData)
{
    for (int i=0 ; i<playerCart::playersAddedCount() ; i++)
    {
        playersData[i].color=players[i]->playerColor();
        playersData[i].name=players[i]->playerName();

        //temporary
        if (playersData[i].color!="white")
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
void menu::onStartGameClicked()
{
    //if number of players is less than 1 return
    if (playerCart::playersAddedCount()<1)
        return;

    //hide main menu's window
    hide();

    //stop playing music
    menuMusic.stop();

    //prepare structure for exporting players' data
    playerData playersData[playerCart::playersAddedCount()];
    export_playersData(playersData);

    //show game's window
    gameWnd=new game(this, QSize(1000, 600), playersData, playerCart::playersAddedCount());
    gameWnd->show();

    //connect slot witch will react on the end of the game
    connect(gameWnd, SIGNAL(rejected()), this, SLOT(onGameEnded()));
}

void menu::onGameEnded()
{
    //reappear main menu's window
    gameWnd->deleteLater();
    erase_lobby();
    create_main_manu();
    menuMusic.play();
    show();

    //take over handling the keyboard events
    grabKeyboard();
}
