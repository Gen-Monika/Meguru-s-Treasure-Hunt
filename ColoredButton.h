#ifndef COLOREDBUTTON_H
#define COLOREDBUTTON_H

#include <QPushButton>
#include <QString>
#include <QMouseEvent>
#include <QEvent>
#include <QWidget>
#include <QIcon>

#include "config.h"

class ColoredButton : public QPushButton {
    Q_OBJECT
public:
    ColoredButton();
    ColoredButton(QWidget* parent);
    ColoredButton(const QString& text, QWidget* parent);//继承构造函数
    ColoredButton(int i,int j);
    ~ColoredButton();
    QIcon* pic = nullptr;
};

#endif // COLOREDBUTTON_H
