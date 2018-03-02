#include "otherFunctions.h"

playerColor::color string_to_playerColor(QString str)
{
    if (str=="white")
        return playerColor::color::White;
    if (str=="silver")
        return playerColor::color::Silver;
    if (str=="green")
        return playerColor::color::Green;
    return playerColor::color::Yellow;
}

// CHANGING PLAYER HELMET'S COLOR

QPixmap color_player(playerColor::color color, QString imgPath)
{
    QImage img(imgPath);
    if (color==playerColor::Silver)
        color_player_helper(img, 32, 40, QColor(0, 76, 153));
    else if (color==playerColor::Green)
        color_player_helper(img, 32, 40, QColor(51, 255, 51));
    else if(color==playerColor::Yellow)
        color_player_helper(img, 32, 40, Qt::yellow);
    return QPixmap::fromImage(img);
}

void color_player_helper(QImage &img, int x, int y, QColor color)
{
    if (((x-32)*(x-32)) + ((y-59)*(y-59)) > 25*25 || stop_coloring(img.pixelColor(x, y), color))
        return;

    img.setPixelColor(x, y, color);

    color_player_helper(img, x, y+1, color);
    color_player_helper(img, x, y-1, color);
    color_player_helper(img, x+1, y, color);
    color_player_helper(img, x-1, y, color);
}

bool stop_coloring(const QColor& color, const QColor& aimedColor)
{
    //yellow face
    if (color.red()==255 && color.green()==218 && color.blue()==48)
        return true;
    //brown something on helmet
    if (color.red()==203 && color.green()==99 && color.blue()==43)
        return true;
    //player's back
    if (color.red()==228 && color.green()==224 && color.blue()==195)
        return true;
    //aimed color --> this pixel already has been visited
    if (color.red()==aimedColor.red() && color.green()==aimedColor.green() && color.blue()==aimedColor.blue())
        return true;
    return false;
}
