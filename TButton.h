#ifndef TBUTTON_H
#define TBUTTON_H

#include <QPushButton>
#include <QString>
#include <QMouseEvent>
#include <QEvent>
#include <QWidget>
#include <QIcon>

#include "config.h"

class TButton : public QPushButton {
    Q_OBJECT
public:
    TButton();
    TButton(QWidget* parent);
    TButton(const QString& text, QWidget* parent);//继承构造函数
    TButton(int i,int j);
    ~TButton();
    bool is_white = false;
    bool is_on = false;
    int button_group = 0;
    int button_variant = 0;
    QIcon* pic_white_on = nullptr;
    QIcon* pic_white_off = nullptr;
    QIcon* pic_black_on = nullptr;
    QIcon* pic_black_off = nullptr;
    void turn_to_white();
    void turn_to_black();
};

#endif // TBUTTON_H
