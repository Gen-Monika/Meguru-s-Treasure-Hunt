#include "ColoredButton.h"
#include "ResourceManager.h"

ColoredButton::ColoredButton()
    : QPushButton() {
    setMouseTracking(true);
}

ColoredButton::ColoredButton(QWidget* parent)
    : QPushButton(parent) {
    setMouseTracking(true);
}

ColoredButton::ColoredButton(const QString& text, QWidget* parent)
    : QPushButton(text, parent) {
    setMouseTracking(true);
}

ColoredButton::ColoredButton(int i, int j)
{
    setMouseTracking(true);
    const QSize size = Default::buttonSize(i,j);
    this->setFixedSize(size);
    pic = ResourceManager::loadIcon(Default::file_of_button[i][j].colored.pic);
    this->setStyleSheet("QPushButton { background-color: transparent; border: none; }"
                          "QPushButton:hover { background-color: rgba(255,255,255,50); }");
    this->setIcon(*pic);
    this->setIconSize(size);
}

ColoredButton::~ColoredButton()
{
    delete pic;
}
