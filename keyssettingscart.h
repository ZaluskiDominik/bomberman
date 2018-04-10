#ifndef KEYSSETTINGSCART_H
#define KEYSSETTINGSCART_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include "otherFunctions.h"

class keysSettingsCart : public QWidget
{
    Q_OBJECT
public:
    explicit keysSettingsCart(QWidget *parent, playerColor::color color);

protected:
    void paintEvent(QPaintEvent *);

private:
    const playerColor::color playerColor;
    QLabel* playerLabel;
    QGroupBox* keysBox;
    QFormLayout* keysLayout;

    void create_keys_box();
    //create one row containing key label and name of selected key
    void create_key_form();

    void create_playerLabel();

private slots:
    void onChangeKeyButtonClicked();
};

#endif // KEYSSETTINGSCART_H
