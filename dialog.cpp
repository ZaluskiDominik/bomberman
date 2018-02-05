#include "dialog.h"
#include "ui_dialog.h"
#include <QPainter>
#include <QKeyEvent>

//carts in play menu
playerCart* players[4];
//game resolution
QSize screenSize;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("Bomberman");
    setMinimumSize(600, 700);
    create_main_manu();

    //background music
    menuMusic.setMedia(QMediaContent(QUrl("qrc:/sounds/sounds/Power Bots Loop.wav")));
    connect (&menuMusic, SIGNAL(positionChanged(qint64)), this, SLOT(onPosChanged()));
    menuMusic.play();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::erase_main_menu()
{
    //deleting buttons
    for (int i=0 ; i<menuButton::buttons ; i++)
        chooseButtons[i]->deleteLater();
    //deleting frame
    menuFrame->deleteLater();
    volumeButton->deleteLater();
}

void Dialog::create_main_manu()
{
    currentLocation="mainmenu";
    //setting frame
    menuFrame=new QFrame(this);
    menuFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    menuFrame->setLineWidth(3);

    //adding choose buttons
    QString text[menuButton::buttons]={ "Play", "Settings", "Exit" };
    for (int i=0 ; i<menuButton::buttons ; i++)
    {
        chooseButtons[i]=new menuButton(this, text[i]);
        chooseButtons[i]->show();
    }

    //volume
    volumeButton=new QPushButton(this);
    volumeButton->setGeometry(20, 20, 50, 50);
    volumeButton->setIcon(QIcon(":/images/img/volume.png"));
    volumeButton->setIconSize(QSize(50, 50));
    volumeButton->setFlat(true);

    //connecting slots
    connect (chooseButtons[0], SIGNAL(clicked(bool)), this, SLOT(onPlayClicked()));
    connect (chooseButtons[1], SIGNAL(clicked(bool)), this, SLOT(onSettingsClicked()));
    connect (chooseButtons[2], SIGNAL(clicked(bool)), this, SLOT(onExitClicked()));
}

void Dialog::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    if (currentLocation=="mainmenu")
    {
        //drawing main menu
        //drawing background
        QImage background(":/images/img/menuBackground.png");
        background=background.scaled(width(), height());
        p.drawImage(0, 0, background);

        //main text "bomberman"
        p.setFont(QFont("Ravie", 20));
        p.setPen(Qt::red);
        p.drawText(width()/2-150, 40, 300, 100, Qt::AlignCenter, "Bomberman");

        //resizing frame
        int frameW=0.5*width(), frameH=0.5*height();
        menuFrame->setGeometry(width()/2-(frameW/2), height()/2-(frameH/2), frameW, frameH);
        //button class take care of resing itself
        menuButton::frameSizeChanged(QSize(frameW, frameH));

        //resizing buttons
        int buttonH=chooseButtons[0]->sizeHint().height(), buttonW=chooseButtons[0]->sizeHint().width();
        for (int i=0 ; i<menuButton::buttons ; i++)
            chooseButtons[i]->setGeometry(menuFrame->pos().x()+15, menuFrame->pos().y()+15+(i*(buttonH+20)), buttonW, buttonH);
    }
    else if (currentLocation=="play")
    {
        //drawing play menu
        QImage choosingPlayersImg(":/images/img/choosing_players.jpg");
        choosingPlayersImg=choosingPlayersImg.scaled(width(), height());
        p.drawImage(0, 0, choosingPlayersImg);

        //drawing start button
        startButton->setGeometry(width()/2-100, height()-90, 200, 80);
        int i, cartH=(height()-25-90)/2, cartW=(width()-25)/2;
        for (i=0 ; i<playerCart::playersCount() ; i++)
            players[i]->setGeometry((i%2)*(cartW+5) + 10, (i/2)*(cartH+5) + 10, cartW, cartH); 
    }
}

//menu music
void Dialog::onPosChanged()
{
    if (menuMusic.position()>=45000)
    {
        menuMusic.setPosition(0);
        menuMusic.play();
    }
}

//beging game
void Dialog::onStartGameClicked()
{
    hide();
    gameWnd=new game(this, QSize(1000, 800));
    connect(gameWnd, SIGNAL(destroyed(QObject*)), this, SLOT(onGameEnded()));
    gameWnd->show();
}

void Dialog::onGameEnded()
{
    qDebug()<<"SIGNAL";
    show();
}

//MAIN MENU BUTTONS SLOTS

void Dialog::onPlayClicked()
{
    erase_main_menu();
    players[0]=new playerCart(this);
    players[0]->show();
    currentLocation="play";

    //setting start button
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

//ends appp
void Dialog::onExitClicked()
{
    QCoreApplication::exit(0);
}

//KEYBOARD

void Dialog::keyPressEvent(QKeyEvent *e)
{
    if (e->key()==Qt::Key_Escape)
    {
        if (currentLocation=="play")
        {
            //deleting widgets
            for (int i=0 ; i<playerCart::playersCount() ; i++)
                players[i]->deleteLater();
        }
        //returning to main menu
        create_main_manu();
        currentLocation="mainmenu";
        update();
    }
}
