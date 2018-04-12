#include "menu.h"
#include <QPainter>
#include <QKeyEvent>
#include <QCoreApplication>
#include <QGraphicsDropShadowEffect>
#include <QSpinBox>
#include <QSlider>
#include <fstream>
#include <QMessageBox>

int fieldSize=60;
int volume=50;

//player slots in lobby(max 4 players)
extern playerCart* players[4];

menu::menu(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint), numMenu(3)
{
    //set the window's title and the window's resolution
    setWindowTitle("Bomberman");
    setMinimumSize(800, 800);
    setMaximumSize(1100, 1020);
    load_players_settings();

    //display main menu
    create_main_menu();
}

void menu::load_players_settings()
{
    std::ifstream file("keys.txt");
    std::string color;

    if (file.is_open())
    {
        for (int i=0 ; i<4 ; i++)
        {
            //player's color
            file>>color;
            data[i].color=QString::fromStdString(color);
            //player's keys
            file>>data[i].keys.up;
            file>>data[i].keys.down;
            file>>data[i].keys.left;
            file>>data[i].keys.right;
            file>>data[i].keys.bomb;
        }
        file.close();
    }
    else
        load_default_settings();

    //set initial number of lifes
    for (int i=0 ; i<4 ; i++)
        data[i].lifes=2;
}

void menu::load_default_settings()
{
    //players' colors
    data[0].color="white";
    data[1].color="blue";
    data[2].color="green";
    data[3].color="yellow";

    //keys
    data[0].keys={Qt::Key_Up, Qt::Key_Down, Qt::Key_Left, Qt::Key_Right, Qt::Key_Return};
    data[1].keys={Qt::Key_W, Qt::Key_S, Qt::Key_A, Qt::Key_D, Qt::Key_CapsLock};
    data[2].keys={Qt::Key_5, Qt::Key_2, Qt::Key_1, Qt::Key_3, Qt::Key_9};
    data[3].keys={Qt::Key_K, ',', Qt::Key_M, '.', Qt::Key_Space};
}

void menu::draw_background(QPainter &p)
{
    QImage img(":/img/img/background.png");
    img=img.scaled(width(), height());
    p.drawImage(0, 0, img);
}

//main menu
//*********************************************************************************************

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
    for (int i=0 ; i<playerCart::carts_count() ; i++)
        players[i]->setGeometry((i%2) * (cartW+5) + 10, (i/2) * (cartH+5) + 10, cartW, cartH);
}

void menu::erase_lobby()
{
    //delete all player's slots carts
    for (int i=0 ; i<playerCart::carts_count() ; i++)
        players[i]->deleteLater();

    startButton->deleteLater();
    returnButton->deleteLater();
}

void menu::prepare_playersData_to_export()
{
    for (int i=0 ; i<4 ; i++)
        data[i].inGame=false;

    for (int i=0, j ; i<playerCart::added_carts_count() ; i++)
    {
        for (j=0 ; j<4 && data[j].color!=players[i]->get_player_color() ; j++);
        data[j].name=players[i]->get_player_name();
        //player with this data will be in game
        data[j].inGame=true;
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

void menu::keyPressEvent(QKeyEvent *e)
{
    if (keysSettingsCart::changedKey_selected())
    {
        keysSettingsCart::change_key(e->key());
        keysSettingsCart::select_changedKey(false);
    }
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
    QFormLayout* settingsLayout=new QFormLayout;
    settingsBox=new QGroupBox("Game settings", this);
    settingsBox->setStyleSheet("background: transparent; color: white");
    settingsBox->setLayout(settingsLayout);
    QFont f;
    f.setPointSize(10);
    f.setBold(true);
    settingsBox->setFont(f);

    //resize settings group box
    const int w=width()*0.65, h=200;
    settingsBox->setGeometry((width() - w)/2, height()*0.1, w, h);
    settingsBox->show();

    settingsLayout->setContentsMargins(6, 25, 6, 6);
    //row for resizing fields size
    create_resize_option(settingsLayout);
    settingsLayout->setSpacing(20);
    //row for changing volume
    create_volume_option(settingsLayout);
    settingsLayout->setSpacing(20);
    //row for changing number of lifes
    create_lifes_option(settingsLayout);
}

QLabel *menu::create_label_for_option(QString title)
{
    QLabel* label=new QLabel(title);
    label->setFont(settingsBox->font());
    return label;
}

void menu::create_volume_option(QFormLayout* settingsLayout)
{
    //changing level of sound's volume option
    QSlider* slider=new QSlider(Qt::Horizontal);
    //set minimal and maximal values
    slider->setRange(0, 100);
    slider->setValue(volume);
    QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
    settingsLayout->addRow(create_label_for_option("Sound's volume: "), slider);
}

void menu::create_resize_option(QFormLayout *settingsLayout)
{
    //changing field size option
    QSpinBox* spinBox=new QSpinBox;
    spinBox->setStyleSheet("background-color: white; color: black");
    spinBox->setValue(fieldSize);
    //set minimal and maximal values
    spinBox->setRange(40, 100);
    spinBox->setSingleStep(10);
    QObject::connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxSizeValueChanged(int)));

    settingsLayout->addRow(create_label_for_option("Field size: "), spinBox);
}

void menu::create_lifes_option(QFormLayout *settigsLayout)
{
    QSpinBox* spinBox=new QSpinBox;
    spinBox->setStyleSheet("background-color: white; color: black");
    spinBox->setRange(1, 10);
    //set initial number of lifes
    spinBox->setValue(data[0].lifes);
    QObject::connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxLifesValueChanged(int)));
    settigsLayout->addRow(create_label_for_option("Lifes: "), spinBox);
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
    //resize bith buttons
    int h=height()*0.2, w=settingsBox->width();
    keySettingsButton->setGeometry((width() - w)/2, settingsBox->y() + settingsBox->height() + (height()*0.1), w, h);
    returnButton->setGeometry((width() - w)/2, keySettingsButton->y() + h + (height()*0.1), w, h);
}

//keys settings menu
//*************************************************************************

void menu::create_keys_settings_menu()
{
    create_change_key_carts();
    create_returnButton();
    create_apply_button();

    currentLocation="keysSettings";
    update();
}

void menu::erase_keys_settings_menu()
{
    //remove any highlighted key that could be selected for changing
    keysSettingsCart::select_changedKey(false);
    for (int i=0 ; i<4 ; i++)
         settingsCarts[i]->deleteLater();
    returnButton->deleteLater();
    applyButton->deleteLater();
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

    //resize return and apply buttons
    resize_buttons_in_keys_setings();
}

void menu::create_change_key_carts()
{
    playerColor::color colors[4]={playerColor::color::White, playerColor::color::Blue, playerColor::color::Green, playerColor::color::Yellow};
    for (int i=0 ; i<4 ; i++)
    {
        settingsCarts[i]=new keysSettingsCart(this, colors[i], data[i].keys);
        settingsCarts[i]->show();
    }
}

void menu::create_apply_button()
{
    applyButton=new menuButton(this, "Apply");
    applyButton->set_text_color(QColor("#ff0000"));
    applyButton->show();
    QObject::connect(applyButton, SIGNAL(clicked(bool)), this, SLOT(onApplyButtonClicked()));
}

void menu::resize_buttons_in_keys_setings()
{
    int w=width()*0.4, h=80;
    int yOffset=settingsCarts[0]->y() + settingsCarts[0]->height();
    returnButton->setGeometry((width()/2 - w)/2, yOffset + ((height() - yOffset - h)/2), w, h);
    applyButton->setGeometry(width()*3/4 - (w/2), yOffset + ((height() - yOffset - h)/2), w, h);
}

void menu::save_players_settings(std::ofstream& file)
{
    //get keys from settingsCarts
    for (int i=0 ; i<4 ; i++)
         data[i].keys=settingsCarts[i]->get_keys();
    //write to file
    for (int i=0 ; i<4 ; i++)
    {
        file<<data[i].color.toStdString()<<"\n";
        file<<data[i].keys.up<<"\n";
        file<<data[i].keys.down<<"\n";
        file<<data[i].keys.left<<"\n";
        file<<data[i].keys.right<<"\n";
        file<<data[i].keys.bomb<<"\n";
    }
}

//*************************************************************************

//begin game
void menu::onStartGameClicked()
{
    //if number of players is less than 1 return
    if (playerCart::added_carts_count()<1)
        return;

    //hide main menu's window
    hide();
    prepare_playersData_to_export();

    //show game's window
    gameWnd=new game(this, data);
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
}

void menu::onSpinBoxLifesValueChanged(int value)
{
    for (int i=0 ; i<4 ; i++)
        data[i].lifes=value;
}

void menu::onSpinBoxSizeValueChanged(int value)
{
    QSpinBox* spinBox=qobject_cast<QSpinBox*>(sender());
    if (value%10)
        spinBox->setValue(int(value/10)*10);
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

void menu::onApplyButtonClicked()
{
    std::ofstream file("keys.txt");
    if (!file.is_open())
    {
        //error
        QMessageBox::critical(this, "Error", "Could not save keys' settings! Please try again.", QMessageBox::Ok);
    }
    else
    {
        save_players_settings(file);
        QMessageBox::information(this, "Operation successful", "Keys' settings saved.", QMessageBox::Ok);
    }
}
