#ifndef KEYSSETTINGSCART_H
#define KEYSSETTINGSCART_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include "otherFunctions.h"

class keysSettingsCart : public QWidget
{
    Q_OBJECT
public:
    explicit keysSettingsCart(QWidget *parent, playerColor::color color, const keys_t &keys);

protected:
    void paintEvent(QPaintEvent *);

private:
    const playerColor::color playerColor;
    keys_t playerKeys;
    QLabel* playerLabel;
    QGroupBox* keysBox;
    QGridLayout* keysLayout;

    //transform intiger value of a key to string representation
    QString from_key_number_to_string(int key);

    //create groupBox for player's keys
    void create_keys_box();
    //create one row containing key label and name of selected key
    void create_key_row(const QPixmap& pixmap, int rotation, QString buttonText, int row);
    void create_in_row_pixmapLabel(const QPixmap& pixmap, int rotation, int row);
    void create_in_row_keyButton(QString buttonText, int row);

    void create_all_keys();

    void create_playerLabel();

private slots:
    void onChangeKeyButtonClicked();
};

#endif // KEYSSETTINGSCART_H
