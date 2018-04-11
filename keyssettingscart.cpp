#include "keyssettingscart.h"

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

QString keysSettingsCart::from_key_number_to_string(int key)
{
    //ascii characters
    if (key>=33 && key<=126)
        return QString(QChar::fromLatin1(key));

    switch(key)
    {
    case Qt::Key_CapsLock:
        return "Caps Lock";
    case Qt::Key_Tab:
        return "Tab";
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
    QPushButton* keyButton=new QPushButton(buttonText, this);
    //keyButton->setFlat(true);
    QFont f;
    f.setBold(true);
    f.setPointSize(10);
    keyButton->setFont(f);
    keysLayout->addWidget(keyButton, row, 1);
}

void keysSettingsCart::create_all_keys()
{
    create_key_row(QPixmap(":/img/img/key_up.png"), 0, from_key_number_to_string(playerKeys.up), 0);
    create_key_row(QPixmap(":/img/img/key_up.png"), 90, from_key_number_to_string(playerKeys.down), 1);
    create_key_row(QPixmap(":/img/img/key_up.png"), 180, from_key_number_to_string(playerKeys.left), 2);
    create_key_row(QPixmap(":/img/img/key_up.png"), 270, from_key_number_to_string(playerKeys.right), 3);
    create_key_row(QPixmap(":/images/img/bomb/bomb.png"), 0, from_key_number_to_string(playerKeys.bomb), 4);
}

void keysSettingsCart::create_playerLabel()
{
    playerLabel=new QLabel(this);
    playerLabel->setScaledContents(true);
    playerLabel->setPixmap(color_player(playerColor, ":/images/img/players/white/Front/Bman_F_f00.png"));
}

void keysSettingsCart::onChangeKeyButtonClicked()
{

}
