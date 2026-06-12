#ifndef SCCHECKBOX_H
#define SCCHECKBOX_H

#include <QAudioOutput>
#include <QElapsedTimer>
#include <QEvent>
#include <QIcon>
#include <QMediaPlayer>
#include <QMouseEvent>
#include <QPushButton>
#include <QString>
#include <QWidget>

#include "config.h"

class SCCheckbox : public QPushButton
{
    Q_OBJECT
public:
    SCCheckbox();
    SCCheckbox(QWidget* parent);
    SCCheckbox(const QString& text,QWidget* parent);
    SCCheckbox(int i,int j);
    ~SCCheckbox();

    bool isChecked() const;
    void setChecked(bool checked);
    void toggleChecked();

    void turn_to_white();
    void turn_to_black();

    QIcon* pic_normal_on = nullptr;
    QIcon* pic_normal_off = nullptr;
    QIcon* pic_hovered_on = nullptr;
    QIcon* pic_hovered_off = nullptr;

    QMediaPlayer* sound_hovered_player = nullptr;
    QAudioOutput* sound_hovered_output = nullptr;
    QMediaPlayer* sound_clicked_player = nullptr;
    QAudioOutput* sound_clicked_output = nullptr;

private:
    bool checked = false;
    bool hovered = false;
    QElapsedTimer hover_sound_timer;

    void initBasicState();
    void loadFromConfig(int i,int j);
    void refreshIcon();
    void playHoverSound();
    void playClickedSound();

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};

#endif // SCCHECKBOX_H
