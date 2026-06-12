#include "SCCheckbox.h"
#include "ResourceManager.h"

namespace {
constexpr qint64 hover_sound_cooldown_ms = 80;
}

SCCheckbox::SCCheckbox()
    : QPushButton()
{
    initBasicState();
}

SCCheckbox::SCCheckbox(QWidget* parent)
    : QPushButton(parent)
{
    initBasicState();
}

SCCheckbox::SCCheckbox(const QString& text,QWidget* parent)
    : QPushButton(text,parent)
{
    initBasicState();
}

SCCheckbox::SCCheckbox(int i,int j)
    : QPushButton()
{
    initBasicState();
    loadFromConfig(i,j);
}

SCCheckbox::~SCCheckbox()
{
    delete pic_normal_on;
    delete pic_normal_off;
    delete pic_hovered_on;
    delete pic_hovered_off;

    delete sound_hovered_player;
    delete sound_hovered_output;
    delete sound_clicked_player;
    delete sound_clicked_output;
}

bool SCCheckbox::isChecked() const
{
    return checked;
}

void SCCheckbox::setChecked(bool checked)
{
    if(this->checked == checked){
        return;
    }
    this->checked = checked;
    refreshIcon();
}

void SCCheckbox::toggleChecked()
{
    setChecked(!checked);
}

void SCCheckbox::turn_to_white()
{
    refreshIcon();
}

void SCCheckbox::turn_to_black()
{
    refreshIcon();
}

void SCCheckbox::initBasicState()
{
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover,true);
    setStyleSheet("QPushButton { background-color: transparent; border: none; }"
                  "QPushButton:hover { background-color: rgba(255,255,255,50); }");
}

void SCCheckbox::loadFromConfig(int i,int j)
{
    const QSize size = Default::buttonSize(i,j);
    setFixedSize(size);

    const auto& file = Default::file_of_button[i][j].scc;
    pic_normal_on = ResourceManager::loadIcon(file.pic_normal_on);
    pic_normal_off = ResourceManager::loadIcon(file.pic_normal_off);
    pic_hovered_on = ResourceManager::loadIcon(file.pic_hovered_on);
    pic_hovered_off = ResourceManager::loadIcon(file.pic_hovered_off);

    sound_hovered_player = new QMediaPlayer();
    sound_hovered_output = new QAudioOutput();
    sound_hovered_player->setAudioOutput(sound_hovered_output);
    sound_hovered_player->setSource(ResourceManager::audioUrl(file.sound_hover));
    sound_hovered_output->setVolume(Default::button_volume);

    sound_clicked_player = new QMediaPlayer();
    sound_clicked_output = new QAudioOutput();
    sound_clicked_player->setAudioOutput(sound_clicked_output);
    sound_clicked_player->setSource(ResourceManager::audioUrl(file.sound_clicked));
    sound_clicked_output->setVolume(Default::button_volume);

    refreshIcon();
}

void SCCheckbox::refreshIcon()
{
    QIcon* icon = nullptr;
    if(checked){
        icon = hovered ? pic_hovered_on : pic_normal_on;
    }
    else{
        icon = hovered ? pic_hovered_off : pic_normal_off;
    }
    if(icon != nullptr){
        setIcon(*icon);
        setIconSize(size());
    }
    update();
}

void SCCheckbox::playHoverSound()
{
    if(sound_hovered_player != nullptr
        && !sound_hovered_player->source().isEmpty()
        && (!hover_sound_timer.isValid()
            || hover_sound_timer.elapsed() >= hover_sound_cooldown_ms)){
        sound_hovered_player->stop();
        sound_hovered_player->play();
        hover_sound_timer.restart();
    }
}

void SCCheckbox::playClickedSound()
{
    if(sound_clicked_player != nullptr && !sound_clicked_player->source().isEmpty()){
        sound_clicked_player->stop();
        sound_clicked_player->play();
    }
}

void SCCheckbox::enterEvent(QEnterEvent* event)
{
    QPushButton::enterEvent(event);
    hovered = true;
    refreshIcon();
    playHoverSound();
}

void SCCheckbox::leaveEvent(QEvent* event)
{
    QPushButton::leaveEvent(event);
    hovered = false;
    refreshIcon();
}

void SCCheckbox::mouseReleaseEvent(QMouseEvent* event)
{
    QPushButton::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton && rect().contains(event->pos())){
        playClickedSound();
    }
}
