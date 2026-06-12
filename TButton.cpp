#include "TButton.h"
#include "ResourceManager.h"

TButton::TButton()
    : QPushButton() {
    setMouseTracking(true);
}

TButton::TButton(QWidget* parent)
    : QPushButton(parent) {
    setMouseTracking(true);
}

TButton::TButton(const QString& text, QWidget* parent)
    : QPushButton(text, parent) {
    setMouseTracking(true);
}

TButton::TButton(int i, int j)
{
    setMouseTracking(true);
    button_group = i;
    button_variant = j;
    const QSize size = Default::buttonSize(i,j);
    this->setFixedSize(size);
    pic_white_on = ResourceManager::loadIcon(Default::file_of_button[i][j].t.pic_white_on);
    pic_white_off = ResourceManager::loadIcon(Default::file_of_button[i][j].t.pic_white_off);
    pic_black_on = ResourceManager::loadIcon(Default::file_of_button[i][j].t.pic_black_on);
    pic_black_off = ResourceManager::loadIcon(Default::file_of_button[i][j].t.pic_black_off);
    this->setStyleSheet("QPushButton { background-color: transparent; border: none; }"
                        "QPushButton:hover { background-color: rgba(255,255,255,50); }");
    this->setIcon(*pic_black_off);
    this->setIconSize(size);
}

TButton::~TButton()
{
    delete pic_white_on;
    delete pic_white_off;
    delete pic_black_on;
    delete pic_black_off;
}

void TButton::turn_to_white()
{
    if(is_white) return;
    if(is_on) this->setIcon(*pic_white_on);
    else this->setIcon(*pic_white_off);
    this->setIconSize(Default::buttonSize(button_group,button_variant));
    is_white = true;
    return;
}

void TButton::turn_to_black()
{
    if(!is_white) return;
    if(is_on) this->setIcon(*pic_black_on);
    else this->setIcon(*pic_black_off);
    this->setIconSize(Default::buttonSize(button_group,button_variant));
    is_white = false;
    return;
}
