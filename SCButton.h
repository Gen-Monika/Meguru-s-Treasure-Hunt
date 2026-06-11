#ifndef SCBUTTON_H
#define SCBUTTON_H

#include <QPushButton>
#include <QString>
#include <QMouseEvent>
#include <QEvent>
#include <QWidget>
#include <QIcon>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QElapsedTimer>

#include "config.h"

class SCButton : public QPushButton{
    Q_OBJECT
public:
    SCButton();
    SCButton(QWidget* parent);
    SCButton(const QString& text, QWidget* parent);//继承构造函数
    SCButton(int i,int j);
    ~SCButton();
    QIcon* pic_normal = nullptr;
    QIcon* pic_hovered = nullptr;
    QMediaPlayer* sound_hovered_player = nullptr;
    QAudioOutput* sound_hovered_output = nullptr;
    QMediaPlayer* sound_clicked_player = nullptr;
    QAudioOutput* sound_clicked_output = nullptr;
private:
    QElapsedTimer hover_sound_timer;
signals:
    void hoverEnter();
    void hoverLeave();
    void hoverMove(const QPoint& pos);
public slots:
    virtual void button_hoverEnter();
    virtual void button_hoverLeave();
protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
};

#endif // SCBUTTON_H
