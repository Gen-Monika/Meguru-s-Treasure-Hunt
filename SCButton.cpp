#include "SCButton.h"
#include "ResourceManager.h"

namespace {
constexpr qint64 hover_sound_cooldown_ms = 80;
}

SCButton::SCButton()
    : QPushButton() {
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover, true);
}

SCButton::SCButton(QWidget* parent)
    : QPushButton(parent) {
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover, true);
}

SCButton::SCButton(const QString& text, QWidget* parent)
    : QPushButton(text, parent) {
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover, true);
}

SCButton::SCButton(int i, int j)
{
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover, true);
    const QSize size = Default::buttonSize(i,j);
    this->setFixedSize(size);
    pic_normal = ResourceManager::loadIcon(Default::file_of_button[i][j].scb.pic_normal);
    pic_hovered = ResourceManager::loadIcon(Default::file_of_button[i][j].scb.pic_hovered);
    this->setStyleSheet("QPushButton { background-color: transparent; border: none; }"
                        "QPushButton:hover { background-color: rgba(255,255,255,50); }");
    this->setIcon(*pic_normal);
    this->setIconSize(size);

    sound_clicked_player = new QMediaPlayer();
    sound_clicked_output = new QAudioOutput();
    sound_clicked_player->setAudioOutput(sound_clicked_output);
    sound_clicked_player->setSource(ResourceManager::audioUrl(Default::file_of_button[i][j].scb.sound_clicked));
    sound_clicked_output->setVolume(Default::button_volume);

    sound_hovered_player = new QMediaPlayer();
    sound_hovered_output = new QAudioOutput();
    sound_hovered_player->setAudioOutput(sound_hovered_output);
    sound_hovered_player->setSource(ResourceManager::audioUrl(Default::file_of_button[i][j].scb.sound_hovered));
    sound_hovered_output->setVolume(Default::button_volume);

    connect(this,&SCButton::hoverEnter,this,&SCButton::button_hoverEnter);
    connect(this,&SCButton::hoverLeave,this,&SCButton::button_hoverLeave);
}

SCButton::~SCButton()
{
    delete pic_hovered;
    delete pic_normal;

    delete sound_clicked_player;
    delete sound_clicked_output;
    delete sound_hovered_player;
    delete sound_hovered_output;
}

void SCButton::enterEvent(QEnterEvent* event) {
    QPushButton::enterEvent(event);
    emit hoverEnter();
}

void SCButton::leaveEvent(QEvent* event) {
    QPushButton::leaveEvent(event);
    emit hoverLeave();
}

void SCButton::mouseMoveEvent(QMouseEvent* event) {
    QPushButton::mouseMoveEvent(event);
    emit hoverMove(event->pos());
}

void SCButton::button_hoverEnter()
{
    if(pic_hovered != nullptr){
        this->setIcon(*pic_hovered);
        this->setIconSize(size());
    }
    update();

    if(sound_hovered_player != nullptr
        && !sound_hovered_player->source().isEmpty()
        && (!hover_sound_timer.isValid()
            || hover_sound_timer.elapsed() >= hover_sound_cooldown_ms)){
        sound_hovered_player->stop();
        sound_hovered_player->play();
        hover_sound_timer.restart();
    }
    return;
}

void SCButton::button_hoverLeave()
{
    if(pic_normal != nullptr){
        this->setIcon(*pic_normal);
        this->setIconSize(size());
    }
    update();
    return;
}
