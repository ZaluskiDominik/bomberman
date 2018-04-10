#include "keyssettingscart.h"

keysSettingsCart::keysSettingsCart(QWidget *parent, playerColor::color color)
    :QWidget(parent), playerColor(color)
{
    create_playerLabel();
    create_keys_box();
}

void keysSettingsCart::paintEvent(QPaintEvent *)
{
    playerLabel->setGeometry(0, 0, width(), height()*0.6);
    keysBox->setGeometry(0, playerLabel->height() + 10, width(), height() - playerLabel->height() - 10);
}

void keysSettingsCart::create_keys_box()
{
    keysBox=new QGroupBox(" Keys ", this);
    QFont f;
    f.setBold(true);
    f.setPointSize(12);
    keysBox->setFont(f);
    keysLayout=new QFormLayout(this);
    keysBox->setLayout(keysLayout);
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
