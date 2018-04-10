#include "menu.h"
#include <QPainter>
#include <QKeyEvent>
#include <QCoreApplication>
#include <QGraphicsDropShadowEffect>
#include <QSpinBox>
#include <QSlider>

int fieldSize=60;
int volume=50;

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
}

void menu::draw_background(QPainter &p)
{
    QImage img(":/img/img/background.png");
    img=img.scaled(width(), height());
    p.drawImage(0, 0, img);
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

    //set buttons' color
    menuButtons[0]->set_text_color(QColor(128, 32, 0));
    menuButtons[1]->set_text_color(QColor(179, 89, 0));
    menuButtons[2]->set_text_color(QColor(89, 51, 204));

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
    draw_background(p);
    draw_menuButtons();
    draw_title_label();
}

//lobby
//**********************************************************

void menu::create_lobby()
{
    //create a first empty slot for player
    players[0]=new playerCart(this);
    players[0]->show();

    //create start and return buttons
    create_startButton();
    create_returnButton();
    returnButton->set_background_color(QColor("#99ff99"));

    update();
}

void menu::create_startButton()
{
    startButton=new menuButton(this, "Start");
    startButton->set_text_color(QColor("#ff0000"));
    startButton->set_background_color(QColor("#ffdb4d"));
    startButton->show();
    QObject::connect(startButton, SIGNAL(clicked(bool)), this, SLOT(onStartGameClicked()));
}

void menu::draw_lobby(QPainter& p)
{
    //draw lobby's background
    draw_background(p);

    //resize startGame and return buttons
    const int w=width()/2*0.8, h=80;
    returnButton->setGeometry((width()/2 - w)/2, height() - 90, w, h);
    startButton->setGeometry(width()/4*3 - (w/2), height() - 90, w, h);

    //resize slots for players
    const int cartH=(height() - 25 - 90)/2, cartW=(width() - 25)/2;
    for (int i=0 ; i<playerCart::playersCount() ; i++)
        players[i]->setGeometry((i%2) * (cartW+5) + 10, (i/2) * (cartH+5) + 10, cartW, cartH);
}

void menu::erase_lobby()
{
    //delete all player's slots carts
    for (int i=0 ; i<playerCart::playersCount() ; i++)
        players[i]->deleteLater();

    startButton->deleteLater();
    returnButton->deleteLater();
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

//**********************************************************

void menu::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    if (currentLocation=="mainmenu")    //draw main menu
        draw_main_menu(p);
    else if (currentLocation=="lobby")  //draw lobby menu
        draw_lobby(p);
    else if (currentLocation=="settings")
        draw_settings_menu(p);
    else if (currentLocation=="keysSettings")
        draw_keys_settings_menu(p);
}

//MAIN MENU BUTTONS SLOTS
//***********************************************************

void menu::onPlayClicked()
{
    //clear screen
    erase_main_menu();
    create_lobby();
    //change current location on lobby menu
    currentLocation="lobby";
}

void menu::onSettingsClicked()
{
    erase_main_menu();
    create_settings_menu();
}

void menu::onExitClicked()
{
    QCoreApplication::exit(0);
}

//settings menu
//**********************************************************

void menu::create_settings_menu()
{
    create_settings_groupBox();

    //button for keyboard settings menu
    keySettingsButton=new menuButton(this, "Keys");
    keySettingsButton->set_text_color(QColor("#994d00"));
    keySettingsButton->show();
    QObject::connect(keySettingsButton, SIGNAL(clicked(bool)), this, SLOT(onKeySettingsButtonClicked()));

    //button for returning to main menu
    create_returnButton();

    currentLocation="settings";
    update();
}

void menu::erase_settings_menu()
{
    settingsBox->deleteLater();
    keySettingsButton->deleteLater();
    returnButton->deleteLater();
}

void menu::create_settings_groupBox()
{
    QFormLayout* settingsLayout=new QFormLayout(this);
    settingsBox=new QGroupBox("Game settings", this);
    settingsBox->setStyleSheet("background: transparent");
    settingsBox->setLayout(settingsLayout);
    settingsLayout->setSpacing(20);
    settingsBox->show();

    create_resize_option(settingsLayout);
    settingsLayout->setSpacing(20);
    create_volume_option(settingsLayout);
}

void menu::create_volume_option(QFormLayout* settingsLayout)
{
    //changing level of sound's volume option
    QLabel* volumeLabel=new QLabel("Sound's volume");
    QSlider* slider=new QSlider(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setValue(volume);
    QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
    settingsLayout->addRow(volumeLabel, slider);
}

void menu::create_resize_option(QFormLayout *settingsLayout)
{
    //changing field size option
    QSpinBox* spinBox=new QSpinBox;
    spinBox->setValue(fieldSize);
    spinBox->setRange(40, 100);
    spinBox->setSingleStep(10);
    QObject::connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxValueChanged(int)));
    QLabel* sizeLabel=new QLabel("Field size");
    settingsLayout->addRow(sizeLabel, spinBox);
}

void menu::create_returnButton()
{
    returnButton=new menuButton(this, "Return");
    returnButton->set_text_color(QColor("#004d00"));
    returnButton->show();
    QObject::connect(returnButton, SIGNAL(clicked(bool)), this, SLOT(onReturnClicked()));
}

void menu::draw_settings_menu(QPainter &p)
{
    draw_background(p);
    int w=width()*0.65, h=height()*0.15;
    //resize settings group box
    settingsBox->setGeometry((width() - w)/2, height()*0.1, w, h);

    h=height()*0.2;
    keySettingsButton->setGeometry((width() - w)/2, settingsBox->y() + settingsBox->height() + (height()*0.1), w, h);
    returnButton->setGeometry((width() - w)/2, keySettingsButton->y() + h + (height()*0.1), w, h);
}

//keys settings menu
//*************************************************************************

void menu::create_keys_settings_menu()
{
    create_change_key_carts();
    create_returnButton();

    currentLocation="keysSettings";
    update();
}

void menu::erase_keys_settings_menu()
{
    for (int i=0 ; i<4 ; i++)
         settingsCarts[i]->deleteLater();
    returnButton->deleteLater();
}

void menu::draw_keys_settings_menu(QPainter &p)
{
    draw_background(p);

    //horizontall space between carts
    int hSpacing=10;
    int w=(width() - (hSpacing * 5))/4, h=height()*0.8;
    //resize settingsCarts
    for (int i=0 ; i<4 ; i++)
         settingsCarts[i]->setGeometry(hSpacing + ( i*(w + hSpacing) ), 10, w, h);

    returnButton->setGeometry(width()/2 - 150, height() - 110, 300, 100);
}

void menu::create_change_key_carts()
{
    playerColor::color colors[4]={playerColor::color::White, playerColor::color::Blue, playerColor::color::Green, playerColor::color::Yellow};
    for (int i=0 ; i<4 ; i++)
    {
        settingsCarts[i]=new keysSettingsCart(this, colors[i]);
        settingsCarts[i]->show();
    }
}

//*************************************************************************

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

void menu::onSpinBoxValueChanged(int value)
{
    fieldSize=value;
}

void menu::onSliderValueChanged(int value)
{
    volume=value;
    keySettingsButton->change_volume(volume);
    returnButton->change_volume(volume);
}

void menu::onReturnClicked()
{
    if (currentLocation=="settings")
    {
        erase_settings_menu();
        create_main_menu();
    }
    else if (currentLocation=="lobby")
    {
        erase_lobby();
        create_main_menu();
    }
    else if (currentLocation=="keysSettings")
    {
        erase_keys_settings_menu();
        create_settings_menu();
    }
}

void menu::onKeySettingsButtonClicked()
{
    erase_settings_menu();
    create_keys_settings_menu();
}
