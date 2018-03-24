#ifndef POWERUP_H
#define POWERUP_H
#include <QGraphicsPixmapItem>

class powerup : public QGraphicsPixmapItem
{
public:
    powerup();

    enum powerupType{NewBomb=0, BiggerRange, PushBombs, Boots};

    //number of differents powerups
    static const int numDiffPowers=3;

    powerupType get_powerType()
    {
        return power;
    }

    bool is_pickable()
    {
        return pickable;
    }

    void set_as_pickable()
    {
        pickable=true;
    }

    void set_powerType(powerupType p);

private:
    powerupType power;
    bool pickable;

};

#endif // POWERUP_H
