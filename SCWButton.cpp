#include "SCWButton.h"

#include "config.h"
#include "PathUtils.h"
#include "ResourceManager.h"

#include <QFontMetricsF>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QCursor>

namespace {
constexpr qint64 hover_sound_cooldown_ms = 80;
}

SCWButton::SCWButton(QWidget* parent)
    : QPushButton(parent)
{
    initBasicState();
}

SCWButton::SCWButton(const QString& text,
                     const QString& normalPicPath,
                     const QString& hoveredPicPath,
                     const QString& hoverSoundPath,
                     const QString& clickedSoundPath,
                     const QSize& size,
                     QWidget* parent)
    : QPushButton(parent)
    , button_text(text)
{
    initBasicState();
    loadAssets(normalPicPath,hoveredPicPath,hoverSoundPath,clickedSoundPath,size);
}

SCWButton::~SCWButton()
{
    delete pic_normal;
    delete pic_hovered;
    delete sound_hovered_player;
    delete sound_hovered_output;
    delete sound_clicked_player;
    delete sound_clicked_output;
}

void SCWButton::setButtonText(const QString& text)
{
    button_text = text;
    update();
}

void SCWButton::setTextStyle(const QFont& font,
                             const QColor& fillColor,
                             const QColor& outlineColor,
                             double outlineWidth)
{
    text_font = font;
    text_fill_color = fillColor;
    text_outline_color = outlineColor;
    text_outline_width = outlineWidth;
    update();
}

void SCWButton::resetHoverState()
{
    setHoverState(false,false);
}

void SCWButton::refreshHoverState(bool playSound)
{
    if(!isVisible() || !isEnabled()){
        setHoverState(false,false);
        return;
    }

    const QPoint cursorPos = mapFromGlobal(QCursor::pos());
    setHoverState(rect().contains(cursorPos),playSound);
}

void SCWButton::initBasicState()
{
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover,true);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet("QPushButton { background-color: transparent; border: none; }");
    text_font.setPixelSize(34);
    text_font.setBold(true);
}

void SCWButton::loadAssets(const QString& normalPicPath,
                           const QString& hoveredPicPath,
                           const QString& hoverSoundPath,
                           const QString& clickedSoundPath,
                           const QSize& size)
{
    pic_normal = ResourceManager::loadPixmap(normalPicPath);
    pic_hovered = ResourceManager::loadPixmap(hoveredPicPath);

    QSize fittedSize = size;
    if(pic_normal != nullptr && !pic_normal->isNull()){
        fittedSize = pic_normal->size().scaled(size,Qt::KeepAspectRatio);
    }
    setFixedSize(fittedSize);

    if(pic_normal != nullptr && !pic_normal->isNull()){
        *pic_normal = pic_normal->scaled(
            fittedSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    }
    if(pic_hovered != nullptr && !pic_hovered->isNull()){
        *pic_hovered = pic_hovered->scaled(
            fittedSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    }

    sound_hovered_player = new QMediaPlayer(this);
    sound_hovered_output = new QAudioOutput(this);
    sound_hovered_player->setAudioOutput(sound_hovered_output);
    sound_hovered_player->setSource(ResourceManager::audioUrl(hoverSoundPath));
    sound_hovered_output->setVolume(Default::button_volume);

    sound_clicked_player = new QMediaPlayer(this);
    sound_clicked_output = new QAudioOutput(this);
    sound_clicked_player->setAudioOutput(sound_clicked_output);
    sound_clicked_player->setSource(ResourceManager::audioUrl(clickedSoundPath));
    sound_clicked_output->setVolume(Default::button_volume);
}

void SCWButton::setHoverState(bool isHovered,bool playSound)
{
    if(hovered == isHovered){
        return;
    }

    hovered = isHovered;
    if(hovered && playSound){
        playHoverSound();
    }
    update();
}

void SCWButton::playHoverSound()
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

void SCWButton::playClickedSound()
{
    if(sound_clicked_player != nullptr && !sound_clicked_player->source().isEmpty()){
        sound_clicked_player->stop();
        sound_clicked_player->play();
    }
}

void SCWButton::enterEvent(QEnterEvent* event)
{
    QPushButton::enterEvent(event);
    setHoverState(true,true);
}

void SCWButton::leaveEvent(QEvent* event)
{
    QPushButton::leaveEvent(event);
    setHoverState(false,false);
}

void SCWButton::mouseReleaseEvent(QMouseEvent* event)
{
    QPushButton::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton && rect().contains(event->pos())){
        playClickedSound();
    }
}

void SCWButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setRenderHint(QPainter::TextAntialiasing,true);

    const QPixmap* bg = hovered ? pic_hovered : pic_normal;
    if(bg != nullptr && !bg->isNull()){
        QSize bgSize = bg->size().scaled(rect().size(),Qt::KeepAspectRatio);
        QRect bgRect(QPoint(0,0),bgSize);
        bgRect.moveCenter(rect().center());
        painter.drawPixmap(bgRect,*bg);
    }

    painter.setFont(text_font);
    QFontMetricsF metrics(text_font);
    QRectF textRect = rect();
    const double textWidth = metrics.horizontalAdvance(button_text);
    const double x = textRect.center().x() - textWidth / 2.0;
    const double y = textRect.center().y()
        - (metrics.ascent() + metrics.descent()) / 2.0
        + metrics.ascent()
        + Default::story_choice_text_y_offset;

    QPainterPath path;
    path.addText(QPointF(x,y),text_font,button_text);

    QPen outline(text_outline_color);
    outline.setWidthF(text_outline_width);
    outline.setJoinStyle(Qt::RoundJoin);

    painter.setPen(outline);
    painter.setBrush(text_fill_color);
    painter.drawPath(path);
}
