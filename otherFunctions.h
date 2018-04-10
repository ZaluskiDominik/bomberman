#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <QString>
#include <QPixmap>

//file with enums, structers importing player's data, functions coloring player's helmet

//enum with available player's colors
namespace playerColor
{
    enum class color{White=0, Blue, Green, Yellow};
}

//functions
//***********************************************************

//convert color represented as string to playerColor::color object
playerColor::color string_to_playerColor(QString str);

//convert white player's pixmap to other colors
QPixmap color_player(playerColor::color color, QString imgPath);

//structures
//***********************************************************

//represent player's choosen keys
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
    keys_t keys;
    QString color, name;
    //how many times player can be hit by bomb's explosion
    int lifes;
};

#endif // STRUCTURES_H
