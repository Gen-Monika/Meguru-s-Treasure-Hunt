#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QImage>
#include <QDebug>
#include <QPushButton>
#include <QSize>
#include <QMouseEvent>
#include <QPair>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QKeyEvent>
#include <QSet>
#include <QList>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QThread>
#include <QRandomGenerator>
#include <QGraphicsProxyWidget>

#include <iostream>
#include <fstream>
#include <string>

#include "config.h"
#include "ColoredButton.h"
#include "TButton.h"
#include "SCButton.h"
#include "meguru.h"
#include "level.h"

//延迟下一步调用的函数
#define delay_for(num) QEventLoop loop;\
QTimer::singleShot(num, &loop, &QEventLoop::quit);\
loop.exec();

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void showEvent(QShowEvent* event)override;
    void resizeEvent(QResizeEvent* event)override;
    void keyPressEvent(QKeyEvent* event)override;
    void keyReleaseEvent(QKeyEvent* event)override;
    void mousePressEvent(QMouseEvent *event)override;
    void changeEvent(QEvent *event)override;

    void init_game(); //初始化整个游戏

    //加载函数
    void load_start_scene();
    void load_levels();

    //游戏周期计时器
    QTimer* game_timer = nullptr;
    //计时器加载函数
    void load_timer();

    //更新函数
    void general_update();

    //界面
    QGraphicsScene* scene = nullptr;//关卡通用界面
    QGraphicsScene* select_scene = nullptr;//选关界面
    QGraphicsScene* start_scene = nullptr; //开始界面
    QGraphicsScene* prev_scene = nullptr;

    //图片的淡入淡出函数
    void fadein_PixmapItem(QGraphicsPixmapItem* item,int fadein_time);
    void fadeout_PixmapItem(QGraphicsPixmapItem* item,int fadeout_time);

    //音乐的淡入淡出函数
    void fadein_music(QUrl* music,int fadein_time);
    void fadeout_music(int fadeout_time);

    //场景的淡入淡出函数
    void fadein_scene(QGraphicsScene* scene,int fadein_time);
    void fadeout_scene(int fadeout_time);

    //背景对象,与背景图片绑定以显示背景
    QGraphicsPixmapItem* bg_start = nullptr;//开始界面背景
    QGraphicsPixmapItem* bg_select = nullptr;

    //背景图片
    QPixmap* pic_bg_start = nullptr;//开始界面背景图片
    QPixmap* pic_bg_select = nullptr;

    QPixmap* pic_now_using = nullptr;//正在使用的图片

    void import_pics();//导入图片

    //按钮
    SCButton* refresh_button = nullptr;
    SCButton* main_scene_button[6];
    TButton* music_button = nullptr;
    ColoredButton* retry_button = nullptr;
    SCButton* level_button[11];
    SCButton* select_scene_button[3];
    SCButton* select_in_level_button = nullptr;

    void import_buttons();//初始化按钮

    bool music_on = false;//标记是否静音

    //音频播放器
    QMediaPlayer* bgm_player = nullptr;//用来播放bgm
    //音频输出端,与对应的QMediaPlayer绑定,可控制音量以实现淡入淡出
    //我们并没有二重奏的需要,所以暂时不使用更多的输出端
    QAudioOutput* bgm_output = nullptr;
    //开始界面的bgm
    QUrl* bgm_bg_start = nullptr;

    QUrl* music_now_playing = nullptr;//正在播放的音乐

    //void move_button(SCButton* button,QGraphicsScene* scene);//移动按钮到新的场景
    //void move_button(TButton* button,QGraphicsScene* scene);//移动按钮到新的场景
    //void move_button(ColoredButton* button,QGraphicsScene* scene);//移动按钮到新的场景
    void move_button(QPushButton* button,QGraphicsScene* scene);//移动按钮到新的场景

    //人物
    meguru* Meguru = nullptr;
    //更新人物的函数
    void Meguru_update();

    //处理移动的set
    QSet<int> move_set;
    //处理移动的函数
    void Meguru_move();

    //Meguru挖洞的函数
    void Meguru_dig();
    void Meguru_dig_test();

    //关卡结束时,刷新Meguru状态的函数
    void Meguru_refresh();

    //测试用洞
    QGraphicsPixmapItem* hole_test = nullptr;
    //洞
    QPixmap* pic_of_hole = nullptr;

    //获取洞中心点的函数
    inline QPointF get_hole_center(QGraphicsPixmapItem* hole);

    //困难度
    bool hard = false;

    //现在进行的关卡
    Level* level_now_playing = nullptr;

    //关卡
    Level* level[11];

    std::ofstream* backlog = nullptr;

    void level_button_Clicked(int i);

    void build_level();

    void refresh_level(Level* level);

    void turn_to_level(Level* level);

    void show_cg(Level* level);

    void import_audios();//导入音频

private slots:
    void refresh_button_Clicked();

    void guide_button_Clicked();

    void select_button_Clicked();

    void extra_button_Clicked();

    void system_button_Clicked();

    void exit_button_Clicked();

    void music_button_Clicked();

    void retry_button_Clicked();

    void return_title_button_Clicked();

    void return_prev_button_Clicked();

    void select_in_level_button_Clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
