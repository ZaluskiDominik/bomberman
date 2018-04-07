#include "menu.h"
#include <QPainter>
#include <QKeyEvent>
#include <QCoreApplication>
#include <QGraphicsDropShadowEffect>

int fieldSize;

//player slots in lobby(max 4 players)
extern playerCart* players[4];

menu::menu(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint), numMenu(3)
{
    //set the window's title and the window's resolution
    setWindowTitle("Bomberman");
    setMinimumSize(600, 700);
    setMaximumSize(900, 1000);

    //display main menu
    create_main_menu();

    fieldSize=60;
}

void menu::create_main_menu()
{
    //change current location on mainmenu
    currentLocation="mainmenu";

    create_title_label();

    //create menu buttons
    QString text[numMenu]={ "Play", "Settings", "Exit" };
    for (int i=0 ; i<numMenu ; i++)
    {
        menuButtons[i]=new menuButton(this, text[i]);
        menuButtons[i]->show();
    }

    //connect slots which will react on clicking menu buttons
    connect (menuButtons[0], SIGNAL(clicked(bool)), this, SLOT(onPlayClicked()));
    connect (menuButtons[1], SIGNAL(clicked(bool)), this, SLOT(onSettingsClicked()));
    connect (menuButtons[2], SIGNAL(clicked(bool)), this, SLOT(onExitClicked()));

    //update screen
    update();
}

void menu::erase_main_menu()
{
    //delete menu buttons
    for (int i=0 ; i<numMenu ; i++)
        menuButtons[i]->deleteLater();

    titleLabel->deleteLater();
}

void menu::create_title_label()
{
    titleLabel=new QLabel("Bomberman", this);
    titleLabel->show();
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #00FF00;");
    //set font
    QFont f;
    f.setBold(true);
    f.setFamily("Ravie");
    f.setLetterSpacing(QFont::AbsoluteSpacing, 6);
    titleLabel->setFont(f);
    //apply drop shadow effect
    QGraphicsDropShadowEffect* effect=new QGraphicsDropShadowEffect(titleLabel);
    effect->setColor(QColor("#BBBBBB"));
    effect->setBlurRadius(0);
    effect->setOffset(6, 6);
    titleLabel->setGraphicsEffect(effect);
}

void menu::draw_title_label()
{
    int h=height()*0.25;
    titleLabel->setGeometry(0, 0.07*h, width(), h);
    //change font size
    QFont f=titleLabel->font();
    f.setPixelSize(titleLabel->width()*0.11);
    titleLabel->setFont(f);
}

void menu::draw_menuButtons()
{
    //calculate a new size for menu buttons
    int spaceBetween=0.025 * height();  //space between the buttons
    int h=( height()*0.6 - ((numMenu - 1) * spaceBetween) )/numMenu, w=width()*0.65;
    for (int i=0 ; i<numMenu ; i++)
        menuButtons[i]->setGeometry((width() - w)/2, height()*0.3 + (i * (h + spaceBetween)), w, h);
}

void menu::draw_main_menu(QPainter& p)
{
    //draw the background
    p.fillRect(0, 0, width(), height(), QBrush(QColor(0, 0, 200)));

    //draw menu buttons
    draw_menuButtons();

    draw_title_label();
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

void menu::erase_lobby()
{
    //delete all player's slots carts
    for (int i=0 ; i<playerCart::playersCount() ; i++)
        players[i]->deleteLater();

    //delete startGame button
    startButton->deleteLater();
}

void menu::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    if (currentLocation=="mainmenu")    //draw main menu
        draw_main_menu(p);
    else if (currentLocation=="lobby")  //draw lobby menu
        draw_lobby(p);
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

//keyboard handler
void menu::keyPressEvent(QKeyEvent *e)
{
    //ESC key for returning to main menu
    if (e->key()==Qt::Key_Escape)
    {
        if (currentLocation=="lobby")
            erase_lobby();

        //return to main menu
        create_main_menu();
    }
}

//TO DO after adding settings menu
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

//begin game
void menu::onStartGameClicked()
{
    //if number of players is less than 1 return
    if (playerCart::playersAddedCount()<1)
        return;

    //hide main menu's window
    hide();

    //prepare structure for exporting players' data
    playerData playersData[playerCart::playersAddedCount()];
    export_playersData(playersData);

    //show game's window
    gameWnd=new game(this, playersData, playerCart::playersAddedCount());
    gameWnd->show();

    //connect slot witch will react on the end of the game
    connect(gameWnd, SIGNAL(rejected()), this, SLOT(onGameEnded()));
}

void menu::onGameEnded()
{
    //reappear main menu's window
    gameWnd->deleteLater();
    erase_lobby();
    create_main_menu();
    show();

    //take over handling the keyboard events
    grabKeyboard();
}
