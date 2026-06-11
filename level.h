#ifndef LEVEL_H
#define LEVEL_H

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QRandomGenerator>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QDebug>

#include "config.h"

class Level
{
public:
    Level(bool hard,QPixmap* pic_bg,QPixmap* pic_cg,QPixmap* text_cg,QUrl* bgm_bg,QUrl* bgm_cg,QUrl* sound_text,bool dark);
    Level(bool hard,int i);
    ~Level();

    //关卡序号
    int index = 0;

    //背景是否阴暗,以便于我们调整音乐图标
    bool isdark = false;
    //关卡是否已经失败
    bool fail = false;
    //cg是否在播放
    bool cg_on = false;
    //允许尝试次数
    int dig_trials = 0;
    //现在的洞数
    int now_hole_num = 0;
    //宝藏坐标
    double target_x = 0.0;
    double target_y = 0.0;

    //一大堆洞
    QGraphicsPixmapItem* Holes[22] = {0};

    //bg图片
    QPixmap* pic_of_bg = nullptr;
    QGraphicsPixmapItem* level_bg = nullptr;

    //把cg图片封入level中,为了便于显示cg
    QPixmap* pic_of_cg = nullptr;
    QGraphicsPixmapItem* level_cg = nullptr;

    //bgm
    QUrl* bgm_of_bg = nullptr;
    QUrl* bgm_of_cg = nullptr;

    //cg文本框和音效
    QGraphicsPixmapItem* text_of_cg = nullptr;
    QPixmap* pic_text_of_cg = nullptr;

    QMediaPlayer* sound_of_text_player = nullptr;
    QAudioOutput* sound_of_text_output = nullptr;
    QUrl* sound_of_text = nullptr;
};

#endif // LEVEL_H
