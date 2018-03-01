#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <QString>

struct keys_t
{
    int up, down, left, right;
    int bomb;
    keys_t()
        :up(Qt::Key_Up), down(Qt::Key_Down), left(Qt::Key_Left), right(Qt::Key_Right), bomb(Qt::Key_Return)
    {
    }
};

struct playerData
{
    playerData()
    {

    }

    keys_t keys;
    QString color, name;
};

#endif // STRUCTURES_H
