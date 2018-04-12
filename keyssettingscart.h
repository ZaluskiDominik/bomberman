#ifndef KEYSSETTINGSCART_H
#define KEYSSETTINGSCART_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include "otherFunctions.h"
#include <QKeyEvent>

class keyButton : public QPushButton
{
    Q_OBJECT
public:
    keyButton(QWidget* parent, int& _keyValue)
        :QPushButton(parent), keyValue(_keyValue)
    {
        defaultStyle=styleSheet();
    }
    int& keyValue;
    static bool receiveKeyEvents;

    void set_defaul_style()
    {
        setStyleSheet(defaultStyle);
    }

protected:
    void keyPressEvent(QKeyEvent* e)
    {
        if (receiveKeyEvents==true)
            QPushButton::keyPressEvent(e);
        else
            e->ignore();
    }
private:
    QString defaultStyle;
};

class keysSettingsCart : public QWidget
{
    Q_OBJECT
public:
    explicit keysSettingsCart(QWidget *parent, playerColor::color color, const keys_t &keys);

    keys_t get_keys();

    static bool changedKey_selected();
    static void select_changedKey(bool yesNo);
    static void change_key(int key);

protected:
    void paintEvent(QPaintEvent *);

private:
    //current highlighted key that is changed by user
    static keyButton* currChangedKey;
    const playerColor::color playerColor;
    keys_t playerKeys;
    QLabel* playerLabel;
    //group box containing all player's keys
    QGroupBox* keysBox;
    QGridLayout* keysLayout;
    keyButton* keyButtons[5];

    //transform intiger value of a key to string representation
    static QString from_key_number_to_string(int key);

    //create groupBox for player's keys
    void create_keys_box();
    //create one row containing key label and name of selected key
    void create_key_row(const QPixmap& pixmap, int rotation, QString buttonText, int row);
    void create_in_row_pixmapLabel(const QPixmap& pixmap, int rotation, int row);
    void create_in_row_keyButton(QString buttonText, int row);

    void create_all_keys();

    void create_playerLabel();

    //map integer value from keyButton
    int& get_key_reference(int row);

private slots:
    void onChangeKeyButtonClicked();
};

#endif // KEYSSETTINGSCART_H
