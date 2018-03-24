#include "powerup.h"

extern int fieldSize;

powerup::powerup()
{
    setZValue(-1);
    pickable=false;
}

void powerup::set_powerType(powerupType p)
{
    power=p;
    switch (p)
    {
    case powerupType::NewBomb:
        setPixmap(QPixmap(":/images/img/powerups/BombPowerup.png"));
        break;
    case powerupType::Boots:
        setPixmap(QPixmap(":/images/img/powerups/SpeedPowerup.png"));
        break;
    case powerupType::BiggerRange:
        setPixmap(QPixmap(":/images/img/powerups/FlamePowerup.png"));
        break;
    case powerupType::PushBombs:
        setPixmap(QPixmap(":/images/img/powerups/SpeedPowerup.png"));
        break;
    }
    setPixmap(pixmap().scaled(fieldSize, fieldSize));
}
