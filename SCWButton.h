#ifndef SCWBUTTON_H
#define SCWBUTTON_H

#include <QAudioOutput>
#include <QColor>
#include <QElapsedTimer>
#include <QEvent>
#include <QFont>
#include <QMediaPlayer>
#include <QMouseEvent>
#include <QPixmap>
#include <QPushButton>
#include <QSize>
#include <QString>

class SCWButton : public QPushButton
{
    Q_OBJECT

public:
    explicit SCWButton(QWidget* parent = nullptr);
    SCWButton(const QString& text,
              const QString& normalPicPath,
              const QString& hoveredPicPath,
              const QString& hoverSoundPath,
              const QString& clickedSoundPath,
              const QSize& size,
              QWidget* parent = nullptr);
    ~SCWButton();

    void setButtonText(const QString& text);
    void setTextStyle(const QFont& font,
                      const QColor& fillColor,
                      const QColor& outlineColor,
                      double outlineWidth);
    void resetHoverState();
    void refreshHoverState(bool playSound = false);

private:
    QString button_text;
    QPixmap* pic_normal = nullptr;
    QPixmap* pic_hovered = nullptr;
    QMediaPlayer* sound_hovered_player = nullptr;
    QAudioOutput* sound_hovered_output = nullptr;
    QMediaPlayer* sound_clicked_player = nullptr;
    QAudioOutput* sound_clicked_output = nullptr;
    QElapsedTimer hover_sound_timer;

    QFont text_font;
    QColor text_fill_color = QColor(255,255,255);
    QColor text_outline_color = QColor(92,52,24);
    double text_outline_width = 1.5;
    bool hovered = false;

    void initBasicState();
    void loadAssets(const QString& normalPicPath,
                    const QString& hoveredPicPath,
                    const QString& hoverSoundPath,
                    const QString& clickedSoundPath,
                    const QSize& size);
    void setHoverState(bool isHovered,bool playSound);
    void playHoverSound();
    void playClickedSound();

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
};

#endif // SCWBUTTON_H
