#ifndef MEGURU_H
#define MEGURU_H

#include <QGraphicsPixmapItem>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <qdebug.h>

#include "config.h"
#include "PathUtils.h"

#define import_meguru_item(state) state = new QPixmap(PathUtils::resourcePath(PATH_OF_##state));\
*state=state->scaled(Default::Meguru_size,Qt::KeepAspectRatio);

#define import_meguru_related_item(state) state = new QGraphicsPixmapItem();\
pic_##state = new QPixmap(PathUtils::resourcePath(PATH_OF_##state));\
*pic_##state=pic_##state->scaled(Default::text_size,Qt::KeepAspectRatio);\
state->setPixmap(*pic_##state);\
state->setPos(0,Default::bg_size.height()-Default::text_size.height());\
state->setZValue(25);

class meguru:public QGraphicsPixmapItem
{
public:
    meguru();
    ~meguru();

    //标记是否处在挖掘状态
    bool digging = false;

    //标记游戏是否胜利
    bool win = false;

    //移动速度
    double meguru_speed = 0;

    //表情状态
    int statement = 0;

    //左右朝向状态,“面向左方”为真时朝向左边,否则朝向右边
    bool face_left = false;

    //不使用铲子,行走时的图像,left和right表示朝向
    QPixmap* left_0_normal = nullptr;//上一次挖掘距离目标远
    QPixmap* right_0_normal = nullptr;
    QPixmap* left_0_excited = nullptr;//上一次挖掘距离目标近
    QPixmap* right_0_excited = nullptr;

    //使用铲子时的图像
    QPixmap* left_1_normal = nullptr;//挖掘时距离目标远
    QPixmap* right_1_normal = nullptr;
    QPixmap* left_1_excited = nullptr;//挖掘时距离目标近
    QPixmap* right_1_excited = nullptr;
    QPixmap* left_1_win = nullptr;//挖到了,此时不能再行动
    QPixmap* right_1_win = nullptr;

    //提示组件的图片对象
    QGraphicsPixmapItem* far_away = nullptr;
    QGraphicsPixmapItem* far = nullptr;
    QGraphicsPixmapItem* medium = nullptr;
    QGraphicsPixmapItem* near = nullptr;
    QGraphicsPixmapItem* very_near = nullptr;
    QGraphicsPixmapItem* thats_it = nullptr;
    QGraphicsPixmapItem* failed = nullptr;
    //对应图片
    QPixmap* pic_far_away = nullptr;
    QPixmap* pic_far = nullptr;
    QPixmap* pic_medium = nullptr;
    QPixmap* pic_near = nullptr;
    QPixmap* pic_very_near = nullptr;
    QPixmap* pic_thats_it = nullptr;
    QPixmap* pic_failed = nullptr;

    //现在正在展示的图片组件
    QGraphicsPixmapItem* text_now_on = nullptr;

    //用来播放ciallo等音效的组件
    QMediaPlayer* sound_ciallo_player = nullptr;//播放器
    QAudioOutput* sound_ciallo_output = nullptr;//输出端
    QUrl* sound_ciallo = nullptr;//ciallo音效

    QMediaPlayer* sound_win_player = nullptr;//播放器
    QAudioOutput* sound_win_output = nullptr;//输出端
    QUrl* sound_win = nullptr;//获胜语音
};

#endif // MEGURU_H
