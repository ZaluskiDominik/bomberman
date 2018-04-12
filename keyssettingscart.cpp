#include "keyssettingscart.h"

bool keyButton::receiveKeyEvents=true;
keyButton* keysSettingsCart::currChangedKey=nullptr;

keysSettingsCart::keysSettingsCart(QWidget *parent, playerColor::color color, const keys_t& keys)
    :QWidget(parent), playerColor(color)
{
    playerKeys=keys;
    create_playerLabel();
    create_keys_box();
}

void keysSettingsCart::paintEvent(QPaintEvent *)
{
    playerLabel->setGeometry(0, 0, width(), height()*0.5);
    keysBox->setGeometry(0, playerLabel->height() + 10, width(), height() - playerLabel->height() - 10);
}

keys_t keysSettingsCart::get_keys()
{
    return playerKeys;
}

bool keysSettingsCart::changedKey_selected()
{
    return (currChangedKey==nullptr) ? false : true;
}

void keysSettingsCart::select_changedKey(bool yesNo)
{
    if (yesNo)
    {
        keyButton::receiveKeyEvents=false;
        currChangedKey->setStyleSheet("background-color: yellow;");
    }
    else
    {
        if (currChangedKey!=nullptr)
        {
            keyButton::receiveKeyEvents=true;
            currChangedKey->set_defaul_style();
            currChangedKey=nullptr;
        }
    }
}

void keysSettingsCart::change_key(int key)
{
    QString strKey=from_key_number_to_string(key);
    if (strKey!="undef")
    {
        currChangedKey->setText(strKey);
        currChangedKey->keyValue=key;
    }
}

QString keysSettingsCart::from_key_number_to_string(int key)
{
    //ascii characters
    if (key>=33 && key<=126)
        return QString(QChar::fromLatin1(key));

    switch(key)
    {
    case Qt::Key_CapsLock:
        return "Caps Lock";
    case Qt::Key_Shift:
        return "Shift";
    case Qt::Key_Control:
        return "Ctrl";
    case Qt::Key_Alt:
        return "Alt";
    case Qt::Key_Space:
        return "Space";
    case Qt::Key_Return:
        return "Enter";
    case Qt::Key_Left:
        return "Key Left";
    case Qt::Key_Right:
        return "Key Right";
    case Qt::Key_Up:
        return "Key Up";
    case Qt::Key_Down:
        return "Key Down";
    case Qt::Key_Backspace:
        return "Backspace";
    }

    //key don't recognized
    return "undef";
}

void keysSettingsCart::create_keys_box()
{
    keysBox=new QGroupBox(" Keys ", this);
    QFont f;
    f.setBold(true);
    f.setPointSize(10);
    keysBox->setFont(f);
    keysLayout=new QGridLayout;
    keysBox->setLayout(keysLayout);
    create_all_keys();
}

void keysSettingsCart::create_key_row(const QPixmap &pixmap, int rotation, QString buttonText, int row)
{
    create_in_row_pixmapLabel(pixmap, rotation, row);
    create_in_row_keyButton(buttonText, row);
}

void keysSettingsCart::create_in_row_pixmapLabel(const QPixmap &pixmap, int rotation, int row)
{
    QLabel* pixmapLabel=new QLabel;
    pixmapLabel->setScaledContents(true);
    pixmapLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pixmapLabel->setPixmap(pixmap.scaled(40, 40).transformed(QTransform().rotate(rotation)));
    keysLayout->addWidget(pixmapLabel, row,0);
}

void keysSettingsCart::create_in_row_keyButton(QString buttonText, int row)
{
    keyButtons[row]=new keyButton(this, get_key_reference(row));
    keyButtons[row]->setText(buttonText);
    QFont f;
    f.setBold(true);
    f.setPointSize(9);
    keyButtons[row]->setFont(f);
    keysLayout->addWidget(keyButtons[row], row, 1);
    QObject::connect(keyButtons[row], SIGNAL(clicked(bool)), this, SLOT(onChangeKeyButtonClicked()));
}

void keysSettingsCart::create_all_keys()
{
    create_key_row(QPixmap(":/img/img/key_up.png"), 0, from_key_number_to_string(playerKeys.up), 0);
    create_key_row(QPixmap(":/img/img/key_up.png"), 180, from_key_number_to_string(playerKeys.down), 1);
    create_key_row(QPixmap(":/img/img/key_up.png"), 270, from_key_number_to_string(playerKeys.left), 2);
    create_key_row(QPixmap(":/img/img/key_up.png"), 90, from_key_number_to_string(playerKeys.right), 3);
    create_key_row(QPixmap(":/images/img/bomb/bomb.png"), 0, from_key_number_to_string(playerKeys.bomb), 4);
}

void keysSettingsCart::create_playerLabel()
{
    playerLabel=new QLabel(this);
    playerLabel->setScaledContents(true);
    playerLabel->setPixmap(color_player(playerColor, ":/images/img/players/white/Front/Bman_F_f00.png"));
}

int& keysSettingsCart::get_key_reference(int row)
{
    if (row==0)
        return playerKeys.up;
    else if (row==1)
        return playerKeys.down;
    else if (row==2)
        return playerKeys.left;
    else if (row==3)
        return playerKeys.right;
    else
        return playerKeys.bomb;
}

void keysSettingsCart::onChangeKeyButtonClicked()
{
    //if some key is selected then remove that selection
    if (currChangedKey!=nullptr)
        select_changedKey(false);

    currChangedKey=qobject_cast<keyButton*>(sender());
    select_changedKey(true);
}
