#include "widget.h"
#include "ui_widget.h"
#include "PathUtils.h"
#include "ResourceManager.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
    Widget::init_game();
}

Widget::~Widget()
{

    delete game_timer;
    //背景对象,与背景图片绑定以显示背景
    delete bg_start;//开始界面背景
    delete bg_select;


    //背景图片
    delete pic_bg_start;//开始界面背景图片
    delete pic_bg_select;

    delete bgm_player;//用来播放bgm
    delete bgm_output;//bgm输出端

    //bg场景的bgm
    delete bgm_bg_start;//开始界面

    //测试用洞
    delete hole_test;

    //洞
    delete pic_of_hole;

    delete Meguru;

    delete refresh_button;
    for(int i = 1;i<=5;++i){
        delete main_scene_button[i];
    }
    delete music_button;
    delete retry_button;
    for(int i = 1;i<=10;++i){
        delete level_button[i];
    }
    for(int i = 1;i<=2;++i){
        delete select_scene_button[i];
    }
    delete select_in_level_button;

    delete backlog;

    delete ui;
}

QPoint Widget::layoutPoint(const LayoutConfig::PositionRatio &position) const
{
    if(pic_now_using == nullptr){
        return QPoint();
    }
    return QPoint(
        static_cast<int>(pic_now_using->width() * position.x),
        static_cast<int>(pic_now_using->height() * position.y)
    );
}

void Widget::placeButton(QPushButton *button,const LayoutConfig::PositionRatio &position)
{
    if(button == nullptr){
        return;
    }
    button->move(layoutPoint(position));
}

void Widget::moveAndPlaceButton(QPushButton *button,QGraphicsScene *targetScene,const LayoutConfig::PositionRatio &position)
{
    if(button == nullptr || targetScene == nullptr){
        return;
    }
    move_button(button,targetScene);
    placeButton(button,position);
}

void Widget::arrangeStartSceneButtons()
{
    placeButton(refresh_button,LayoutConfig::start_refresh_button_pos);
    for(int i = 1;i<=5;++i){
        placeButton(main_scene_button[i],LayoutConfig::main_scene_button_pos[i]);
    }
    placeButton(music_button,LayoutConfig::start_music_button_pos);
}

void Widget::arrangeSelectSceneButtons()
{
    moveAndPlaceButton(refresh_button,select_scene,LayoutConfig::select_refresh_button_pos);
    moveAndPlaceButton(music_button,select_scene,LayoutConfig::select_music_button_pos);
    moveAndPlaceButton(select_scene_button[Default::return_title_button],select_scene,LayoutConfig::select_return_title_button_pos);
    moveAndPlaceButton(select_scene_button[Default::return_prev_button],select_scene,LayoutConfig::select_return_prev_button_pos);
}

void Widget::arrangeLevelSceneButtons(bool showCg)
{
    moveAndPlaceButton(select_scene_button[Default::return_title_button],scene,LayoutConfig::level_return_title_button_pos);
    moveAndPlaceButton(select_in_level_button,scene,LayoutConfig::level_select_button_pos);
    moveAndPlaceButton(refresh_button,scene,LayoutConfig::level_refresh_button_pos);
    moveAndPlaceButton(music_button,scene,LayoutConfig::level_music_button_pos);
    moveAndPlaceButton(retry_button,scene,showCg ? LayoutConfig::level_cg_retry_button_pos : LayoutConfig::level_retry_button_pos);
}

void Widget::arrangeLevelButtons()
{
    for(int i = 1;i<=10;++i){
        select_scene->addWidget(level_button[i]);
        placeButton(level_button[i],LayoutConfig::levelButtonPosition(i));
    }
}

void Widget::load_timer()
{
    game_timer = new QTimer(this);
    game_timer->setInterval(Default::Game_Period);
    game_timer->start();
}

void Widget::general_update()
{
    Meguru_move();
}

void Widget::fadein_PixmapItem(QGraphicsPixmapItem *item,int fadein_time)
{
    QTimer* fadein_timer = new QTimer;
    fadein_timer->setInterval(Default::Game_Period); // ≈60FPS

    double step = 1.0 / ( (double)fadein_time / (double)Default::Game_Period); // 每次增加的透明度

    item->setOpacity(0.0);
    item->setVisible(true);

    connect(fadein_timer, &QTimer::timeout, [item, fadein_timer, step, currentOpacity = 0.0]() mutable{
        currentOpacity += step;
        item->setOpacity(currentOpacity);
        if (currentOpacity >= 1.0) {
            item->setOpacity(1.0);
            fadein_timer->stop();
            fadein_timer->deleteLater();
        }
    });
    fadein_timer->start();
}

void Widget::fadeout_PixmapItem(QGraphicsPixmapItem *item,int fadeout_time)
{
    QTimer* fadeout_timer = new QTimer;
    fadeout_timer->setInterval(Default::Game_Period); // ≈60FPS

    double step = 1.0 / ( (double)fadeout_time / (double)Default::Game_Period); // 每次增加的透明度

    item->setOpacity(1.0);
    item->setVisible(true);

    connect(fadeout_timer, &QTimer::timeout, [item, fadeout_timer, step, currentOpacity = 1.0]()mutable {
        currentOpacity -= step;
        item->setOpacity(currentOpacity);
        if (currentOpacity <= 0.0) {
            item->setOpacity(0.0);
            fadeout_timer->stop();
            fadeout_timer->deleteLater();
        }
    });
    fadeout_timer->start();
}

void Widget::fadein_music(QUrl *music, int fadein_time)
{
    bgm_player->setSource(*music);
    bgm_output->setVolume(0.0);

    QTimer* fadein_timer = new QTimer(this);
    fadein_timer->setInterval(Default::Game_Period);

    double step = Default::bgm_volume / ((double)fadein_time / (double)Default::Game_Period);

    connect(fadein_timer, &QTimer::timeout, this, [this, fadein_timer, step, currentVolume = 0.0]() mutable {
        currentVolume += step;
        if (currentVolume >= Default::bgm_volume) {
            currentVolume = Default::bgm_volume;
            fadein_timer->stop();
            fadein_timer->deleteLater();
        }
        bgm_output->setVolume(currentVolume);
    });
    bgm_player->setLoops(QMediaPlayer::Infinite);
    bgm_player->play();
    fadein_timer->start();
}

void Widget::fadeout_music(int fadeout_time)
{
    bgm_output->setVolume(Default::bgm_volume);

    QTimer* fadeout_timer = new QTimer(this);
    fadeout_timer->setInterval(Default::Game_Period);

    double step = Default::bgm_volume / ((double)fadeout_time / (double)Default::Game_Period);

    connect(fadeout_timer, &QTimer::timeout, this, [this, fadeout_timer, step, currentVolume = Default::bgm_volume]() mutable {
        currentVolume -= step;
        if (currentVolume <= 0.0) {
            currentVolume = 0.0;
            fadeout_timer->stop();
            fadeout_timer->deleteLater();
        }
        bgm_output->setVolume(currentVolume);
        if(currentVolume == 0.0) bgm_player->stop();
    });
    bgm_player->play();
    fadeout_timer->start();
}

void Widget::fadein_scene(QGraphicsScene *scene, int fadein_time)
{
    // 创建淡入效果
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
    ui->bg->viewport()->setGraphicsEffect(effect);
    effect->setOpacity(0.0);
    ui->bg->setScene(scene);
    // 淡入动画
    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(fadein_time);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    // 动画完成后清理效果
    connect(animation, &QPropertyAnimation::finished, [effect, animation]() {
        effect->deleteLater();
        animation->deleteLater();
    });
    animation->start();
}

void Widget::fadeout_scene(int fadeout_time)
{
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
    ui->bg->viewport()->setGraphicsEffect(effect);
    effect->setOpacity(1.0);

    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(fadeout_time);
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    animation->setEasingCurve(QEasingCurve::InOutQuad);

    connect(animation, &QPropertyAnimation::finished, [effect, animation]() {
        effect->deleteLater();
        animation->deleteLater();
    });
    animation->start();
}

void Widget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    ui->bg->scene()->setSceneRect(pic_now_using->rect());//初始化舞台的矩形尺寸,以契合演员图像的大小
    ui->bg->fitInView(ui->bg->scene()->sceneRect());//将摄像机的大小初始与舞台大小固定
}

void Widget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    ui->bg->scene()->setSceneRect(pic_now_using->rect());//重新设置舞台的矩形尺寸,以契合演员图像的大小
    ui->bg->fitInView(ui->bg->scene()->sceneRect(),Qt::KeepAspectRatio);//将摄像机的大小初始与舞台大小固定,第二个参数保证图像的长宽比
}

void Widget::Meguru_move()
{
    if(!Meguru) return; //角色未初始化,返回
    if(Meguru->statement == 2) return; //游戏已经胜利,返回
    if(level_now_playing){
        if(level_now_playing->fail) return; //游戏失败,返回
    }
    if(Meguru->win) return; //游戏胜利,返回
    double x = Meguru->pos().x();
    double y = Meguru->pos().y();
    double speed = Meguru->meguru_speed;
    double dx = 0;
    double dy = 0;
    bool moving_right = false;
    bool moving_left = false;
    int key_count = 0;
    for(int key : move_set){ //遍历键值
        switch(key){
        case Qt::Key_Up:
            dy -= speed;
            break;
        case Qt::Key_Down:
            dy += speed;
            break;
        case Qt::Key_Left:
            dx -=speed;
            moving_left = true;
            break;
        case Qt::Key_Right:
            dx +=speed;
            moving_right = true;
            break;
        }
        ++key_count;
    }
    if(key_count >=1){
        if(Meguru->digging){ //从挖掘状态变为移动状态,收回铲子和对话框
            Meguru->digging = false;
            ui->bg->scene()->removeItem(Meguru->text_now_on);
            Meguru->text_now_on = nullptr;
            if(moving_left && !moving_right){ //更新角色朝向和状态
                if(Meguru->statement == 0){
                    Meguru->setPixmap(*(Meguru->left_0_normal));
                }
                else{
                    Meguru->setPixmap(*(Meguru->left_0_excited));
                }
                Meguru->face_left = true;
            }
            else if(moving_right && !moving_left){ //更新角色朝向和状态
                if(Meguru->statement == 0){
                    Meguru->setPixmap(*(Meguru->right_0_normal));
                }
                else{
                    Meguru->setPixmap(*(Meguru->right_0_excited));
                }
                Meguru->face_left = false;
            }
            else{
                if(Meguru->face_left){ //更新角色朝向和状态
                    if(Meguru->statement == 0){
                        Meguru->setPixmap(*(Meguru->left_0_normal));
                    }
                    else{
                        Meguru->setPixmap(*(Meguru->left_0_excited));
                    }
                }
                else{
                    if(Meguru->statement == 0){
                        Meguru->setPixmap(*(Meguru->right_0_normal));
                    }
                    else{
                        Meguru->setPixmap(*(Meguru->right_0_excited));
                    }
                }
            }
        }
    }
    if(key_count == 2){ //对角线减速
        dx *= 0.7071;
        dy *= 0.7071;
    }
    //边界判断
    if( (Meguru->pos().x() <= pic_now_using->width()*0 && dx < 0) || (Meguru->pos().x() >= pic_now_using->width()*114/128 && dx > 0) ) dx *= 0.0;
    if( (Meguru->pos().y() <= pic_now_using->height()*3/128 && dy < 0) || (Meguru->pos().y() >= pic_now_using->height()*85/128 && dy > 0) ) dy *= 0.0;
    x += dx;
    y += dy;
    //更新状态
    if(!moving_left||!moving_right){
        if(moving_left){
            if(Meguru->statement == 0){
                Meguru->setPixmap(*(Meguru->left_0_normal));
            }
            else{
                Meguru->setPixmap(*(Meguru->left_0_excited));
            }
            Meguru->face_left = true;
        }
        else if(moving_right){
            if(Meguru->statement == 0){
                Meguru->setPixmap(*(Meguru->right_0_normal));
            }
            else{
                Meguru->setPixmap(*(Meguru->right_0_excited));
            }
            Meguru->face_left = false;
        }
    }
    //改变位置
    Meguru->setPos(x,y);
    return;
}

void Widget::Meguru_dig()
{
    if(level_now_playing == nullptr) return;
    if(level_now_playing->fail) return;
    if(!Meguru) return;
    if(Meguru->digging) return;
    if(Meguru->win) return;
    double target_x = level_now_playing->target_x;
    double target_y = level_now_playing->target_y;
    ++(level_now_playing->now_hole_num);
    level_now_playing->Holes[level_now_playing->now_hole_num] = new QGraphicsPixmapItem(*pic_of_hole);
    level_now_playing->Holes[level_now_playing->now_hole_num]->setPixmap(*pic_of_hole);
    level_now_playing->Holes[level_now_playing->now_hole_num]->setPos(Meguru->pos().x()+Default::hole_dx,Meguru->pos().y()+Default::hole_dy);
    level_now_playing->Holes[level_now_playing->now_hole_num]->setZValue(level_now_playing->now_hole_num+1);
    ui->bg->scene()->addItem(level_now_playing->Holes[level_now_playing->now_hole_num]);
    qDebug()<<"洞的横坐标："<<get_hole_center(level_now_playing->Holes[level_now_playing->now_hole_num]).x();
    qDebug()<<"洞的纵坐标："<<get_hole_center(level_now_playing->Holes[level_now_playing->now_hole_num]).y();
    (*backlog)<<"洞的坐标:("<<get_hole_center(level_now_playing->Holes[level_now_playing->now_hole_num]).x()\
               <<","<<get_hole_center(level_now_playing->Holes[level_now_playing->now_hole_num]).y()<<")"<<std::endl;
    (*backlog)<<"剩余次数:"<<level_now_playing->dig_trials-level_now_playing->now_hole_num<<std::endl;
    double hole_x = get_hole_center(level_now_playing->Holes[level_now_playing->now_hole_num]).x();
    double hole_y = get_hole_center(level_now_playing->Holes[level_now_playing->now_hole_num]).y();
    //获胜
    if( (hole_x - target_x)*(hole_x - target_x) + (hole_y - target_y)*(hole_y - target_y) <= Default::win_boundary*Default::win_boundary ){
        Meguru->sound_win_player->play();
        Meguru->win = true;
        Meguru->digging = true;
        Meguru->statement = 2;
        if(Meguru->face_left){
            Meguru->setPixmap(*(Meguru->left_1_win));
        }
        else{
            Meguru->setPixmap(*(Meguru->right_1_win));
        }
        Meguru->text_now_on = Meguru->thats_it;
        ui->bg->scene()->addItem(Meguru->thats_it);
        (*backlog)<<"找到了!"<<std::endl<<std::endl;
    }
    else if( (hole_x - target_x)*(hole_x - target_x) + (hole_y - target_y)*(hole_y - target_y) <= Default::very_near_boundary*Default::very_near_boundary ){
        Meguru->sound_ciallo_player->play();
        Meguru->digging = true;
        Meguru->statement = 1;
        if(Meguru->face_left){
            Meguru->setPixmap(*(Meguru->left_1_excited));
        }
        else{
            Meguru->setPixmap(*(Meguru->right_1_excited));
        }
        Meguru->text_now_on = Meguru->very_near;
        ui->bg->scene()->addItem(Meguru->very_near);
        (*backlog)<<"就差一点了!直线距离在"<<Default::very_near_boundary<<"个像素点以内!"<<std::endl<<std::endl;
    }
    else if( (hole_x - target_x)*(hole_x - target_x) + (hole_y - target_y)*(hole_y - target_y) <= Default::near_boundary*Default::near_boundary ){
        Meguru->sound_ciallo_player->play();
        Meguru->digging = true;
        Meguru->statement = 1;
        if(Meguru->face_left){
            Meguru->setPixmap(*(Meguru->left_1_excited));
        }
        else{
            Meguru->setPixmap(*(Meguru->right_1_excited));
        }
        Meguru->text_now_on = Meguru->near;
        ui->bg->scene()->addItem(Meguru->near);
        (*backlog)<<"已经很近了,直线距离在"<<Default::near_boundary<<"个像素点以内!"<<std::endl<<std::endl;
    }
    else if( (hole_x - target_x)*(hole_x - target_x) + (hole_y - target_y)*(hole_y - target_y) <= Default::medium_boundary*Default::medium_boundary ){
        Meguru->sound_ciallo_player->play();
        Meguru->digging = true;
        Meguru->statement = 0;
        if(Meguru->face_left){
            Meguru->setPixmap(*(Meguru->left_1_normal));
        }
        else{
            Meguru->setPixmap(*(Meguru->right_1_normal));
        }
        Meguru->text_now_on = Meguru->medium;
        ui->bg->scene()->addItem(Meguru->medium);
        (*backlog)<<"距离已经不远了,直线距离在"<<Default::medium_boundary<<"个像素点以内."<<std::endl<<std::endl;
    }
    else if( (hole_x - target_x)*(hole_x - target_x) + (hole_y - target_y)*(hole_y - target_y) <= Default::far_boundary*Default::far_boundary ){
        Meguru->sound_ciallo_player->play();
        Meguru->digging = true;
        Meguru->statement = 0;
        if(Meguru->face_left){
            Meguru->setPixmap(*(Meguru->left_1_normal));
        }
        else{
            Meguru->setPixmap(*(Meguru->right_1_normal));
        }
        Meguru->text_now_on = Meguru->far;
        ui->bg->scene()->addItem(Meguru->far);
        (*backlog)<<"是这个方向没有错,直线距离在"<<Default::far_boundary<<"个像素点以内."<<std::endl<<std::endl;
    }
    else {
        Meguru->sound_ciallo_player->play();
        Meguru->digging = true;
        Meguru->statement = 0;
        if(Meguru->face_left){
            Meguru->setPixmap(*(Meguru->left_1_normal));
        }
        else{
            Meguru->setPixmap(*(Meguru->right_1_normal));
        }
        Meguru->text_now_on = Meguru->far_away;
        ui->bg->scene()->addItem(Meguru->far_away);
        (*backlog)<<"距离还很远,换个方向吧."<<std::endl<<std::endl;
    }
    if(!Meguru->win && level_now_playing->now_hole_num == level_now_playing->dig_trials){
        ui->bg->scene()->removeItem(Meguru->text_now_on);
        Meguru->text_now_on = Meguru->failed;
        ui->bg->scene()->addItem(Meguru->failed);
        level_now_playing->fail = true;
        (*backlog)<<"铲子坏掉了呢,再试一次吧."<<std::endl<<std::endl;
    }
}

void Widget::Meguru_dig_test()
{
    /*hole_test->setPos(Meguru->pos().x()+DEFAULT_HOLE_DX,Meguru->pos().y()+DEFAULT_HOLE_DY);
    hole_test->setZValue(2);*/
}

void Widget::Meguru_refresh()
{
    Meguru->text_now_on = nullptr;
    Meguru->setPixmap(*(Meguru->right_0_normal));
    Meguru->setPos(pic_now_using->width()/2,pic_now_using->height()/2);
    Meguru->setZValue(23);
    Meguru->digging = false;
    Meguru->statement = 0;
    Meguru->win = false;
}

QPointF Widget::get_hole_center(QGraphicsPixmapItem *hole)
{
    QPointF holePos = hole->pos();
    QRectF holeRect = hole->boundingRect();
    return QPointF(
        holePos.x() + holeRect.width() / 2,
        holePos.y() + holeRect.height() / 2
    );
}

void Widget::build_level()
{
    for(int i = 1;i<=10;++i){
        level[i] = new Level(hard,i);
    }
}

void Widget::refresh_level(Level *level)
{
    scene->removeItem(level->level_bg);
    scene->removeItem(level->level_cg);
    level->fail = false;
    level->cg_on = false;
    if(level->now_hole_num!=0){
        for(int i = 1;i<=level->now_hole_num;++i){
            ui->bg->scene()->removeItem(level_now_playing->Holes[i]);
            delete level_now_playing->Holes[i];
        }
    }
    level->now_hole_num = 0;
    if(hard){
        level->dig_trials = 7;
    }
    else{
        level->dig_trials = 21;
    }
    //生成浮点比例[0,1)
    double percent_x =QRandomGenerator::global()->generateDouble();
    double percent_y =QRandomGenerator::global()->generateDouble();
    //分配宝藏位置
    level->target_x = Default::treasure_min_x + percent_x * (Default::treasure_max_x - Default::treasure_min_x);
    level->target_y = Default::treasure_min_y + percent_y * (Default::treasure_max_y - Default::treasure_min_y);
    if(backlog){
        delete backlog;
        backlog = new std::ofstream(PathUtils::nativeDataPath("backlog.txt"),std::ios::trunc);
    }
    (*backlog)<<"关卡"<<level->index<<"刷新完成."<<std::endl;
    (*backlog)<<"宝藏坐标: "<<'('<<level->target_x<<','<<level->target_y<<')'<<std::endl<<std::endl;
    qDebug()<<"宝藏坐标: "<<'('<<level->target_x<<','<<level->target_y<<')';
    setFocus();
}

void Widget::turn_to_level(Level *level)
{
    refresh_level(level);
    level_now_playing = level;
    if(music_on){
        fadeout_music(Default::fadeout_time);
    }
    fadeout_scene(Default::fadeout_time);
    delay_for(Default::fadeout_time);
    fadein_scene(scene,Default::fadein_time);//摄像机对准舞台并淡入,在接下来的短暂时间里加载场景
    music_now_playing = level->bgm_of_bg;
    pic_now_using = level->pic_of_bg;//加载背景图片
    scene->addItem(level->level_bg);//演员登台
    scene->setSceneRect(pic_now_using->rect());//设置舞台的矩形尺寸,以契合演员图像的大小
    ui->bg->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);//将摄像机的大小初始与舞台大小固定
    prev_scene = scene;

    //刷新Meguru
    Meguru_refresh();
    scene->addItem(Meguru);

    arrangeLevelSceneButtons(false);

    if(level->isdark) music_button->turn_to_white(Default::music_button);
    else music_button->turn_to_black(Default::music_button);
    if(music_on){
        delay_for(Default::fadein_time);
        fadein_music(music_now_playing,Default::fadein_time);
    }
    setFocus();
}


/*
void Widget::move_button(SCButton *button, QGraphicsScene *scene)
{
    QGraphicsProxyWidget* oldProxy = button->graphicsProxyWidget();
    if (oldProxy) {
        // 从原场景移除并删除旧代理
        if (oldProxy->scene()) {
            oldProxy->scene()->removeItem(oldProxy);
        }
        oldProxy->setWidget(nullptr); // 解除与按钮的关联
        oldProxy->deleteLater();
    }
    // 添加到场景
    scene->addWidget(button);
    return;
}

void Widget::move_button(TButton *button, QGraphicsScene *scene)
{
    QGraphicsProxyWidget* oldProxy = button->graphicsProxyWidget();
    if (oldProxy) {
        // 从原场景移除并删除旧代理
        if (oldProxy->scene()) {
            oldProxy->scene()->removeItem(oldProxy);
        }
        oldProxy->setWidget(nullptr); // 解除与按钮的关联
        oldProxy->deleteLater();
    }
    // 添加到场景
    scene->addWidget(button);
    return;
}

void Widget::move_button(ColoredButton *button, QGraphicsScene *scene)
{
    QGraphicsProxyWidget* oldProxy = button->graphicsProxyWidget();
    if (oldProxy) {
        // 从原场景移除并删除旧代理
        if (oldProxy->scene()) {
            oldProxy->scene()->removeItem(oldProxy);
        }
        oldProxy->setWidget(nullptr); // 解除与按钮的关联
        oldProxy->deleteLater();
    }
    // 添加到场景
    scene->addWidget(button);
    return;
}
*/
void Widget::move_button(QPushButton *button, QGraphicsScene *scene)
{
    QGraphicsProxyWidget* oldProxy = button->graphicsProxyWidget();
    if (oldProxy) {
        // 从原场景移除并删除旧代理
        if (oldProxy->scene()) {
            oldProxy->scene()->removeItem(oldProxy);
        }
        oldProxy->setWidget(nullptr); // 解除与按钮的关联
        oldProxy->deleteLater();
    }
    // 添加到场景
    scene->addWidget(button);
    return;
}

void Widget::show_cg(Level *level)
{
    level_now_playing->cg_on = true;
    ui->bg->scene()->removeItem(Meguru->text_now_on);
    if(music_on){
        fadeout_music(Default::fadeout_time);
    }
    fadeout_scene(Default::fadeout_time);
    delay_for(Default::fadeout_time);
    fadein_scene(scene,Default::fadein_time);//摄像机对准舞台并淡入,在接下来的短暂时间里加载场景
    music_now_playing = level->bgm_of_cg;
    pic_now_using = level->pic_of_cg;//加载背景图片
    scene->removeItem(level->level_bg);
    scene->removeItem(Meguru);
    for(int i = 1;i<=level_now_playing->now_hole_num;++i){
        scene->removeItem(level_now_playing->Holes[i]);
    }
    scene->addItem(level->level_cg);//演员登台
    scene->setSceneRect(pic_now_using->rect());//设置舞台的矩形尺寸,以契合演员图像的大小
    ui->bg->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);//将摄像机的大小初始与舞台大小固定

    arrangeLevelSceneButtons(true);
    if(music_on){
        delay_for(Default::fadein_time);
        fadein_music(music_now_playing,Default::fadein_time);
    }
    if(level_now_playing->pic_text_of_cg != nullptr){
        scene->addItem(level_now_playing->text_of_cg);
    }
    if(level_now_playing->sound_of_text != nullptr){
        level_now_playing->sound_of_text_player->play();
    }
    return;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Left:
    case Qt::Key_Right:
        move_set.insert(event->key());
        break;
    case Qt::Key_Space:
        if(Meguru) Meguru_dig();
        break;
    case Qt::Key_Enter:
        if(Meguru){
            if(Meguru->win && !level_now_playing->cg_on) show_cg(level_now_playing);
        }
        break;
    }
    setFocus();
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Left:
    case Qt::Key_Right:
        move_set.remove(event->key());
        break;
    }
    setFocus();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton){
        if(level_now_playing != nullptr){
            if(level_now_playing->cg_on && level_now_playing->pic_text_of_cg !=nullptr){
                ui->bg->scene()->removeItem(level_now_playing->text_of_cg);
            }
        }
    }
    if (event->button() == Qt::LeftButton){
        if(level_now_playing != nullptr){
            if(level_now_playing->cg_on && level_now_playing->pic_text_of_cg !=nullptr){
                ui->bg->scene()->addItem(level_now_playing->text_of_cg);
            }
        }
    }
    setFocus();
    return;
}

void Widget::changeEvent(QEvent *event)
{
    setFocus();
    return;
}

void Widget::import_pics()
{
    //演员,也就是bg和cg的初始化,这里首先是bg
    bg_start = new QGraphicsPixmapItem();
    pic_bg_start = ResourceManager::loadPixmap(PATH_OF_PIC_BG_START);//给演员设置图像
    *pic_bg_start = pic_bg_start->scaled(Default::bg_size,Qt::KeepAspectRatio);//缩放大小,并保证长宽比
    bg_start->setPixmap(*pic_bg_start);

    bg_select = new QGraphicsPixmapItem();
    pic_bg_select = ResourceManager::loadPixmap(PATH_OF_PIC_BG_SELECT);//给演员设置图像
    *pic_bg_select = pic_bg_select->scaled(Default::bg_size,Qt::KeepAspectRatio);//缩放大小,并保证长宽比
    bg_select->setPixmap(*pic_bg_select);

    //洞
    pic_of_hole = ResourceManager::loadPixmap(PATH_OF_HOLE);
    *pic_of_hole = pic_of_hole->scaled(Default::hole_size,Qt::KeepAspectRatio);
    hole_test = new QGraphicsPixmapItem(*pic_of_hole);

}

void Widget::import_buttons()
{
    refresh_button = new SCButton(1,0);
    for(int i = 1;i<=5;++i){
        main_scene_button[i] = new SCButton(2,i);
    }
    music_button = new TButton(3,0);
    retry_button = new SCButton(4,0);
    for(int i = 1;i<=10;++i){
        level_button[i] = new SCButton(5,i);
        connect(level_button[i], &SCButton::clicked, this, [this, i]() {
            level_button_Clicked(i);
            setFocus();
        });
    }
    for(int i = 1;i<=2;++i){
        select_scene_button[i] = new SCButton(6,i);
    }
    select_in_level_button = new SCButton(7,0);
}

void Widget::import_audios()
{
    bgm_bg_start = ResourceManager::loadAudioUrl(PATH_OF_bgm_bg_start);
}

void Widget::music_button_Clicked()
{
    if(music_on){
        music_on = false;
        music_button->is_on = false;
        if(!music_button->is_white) music_button->setIcon(*music_button->pic_black_off);
        else music_button->setIcon(*music_button->pic_white_off);
        music_button->setIconSize(Default::button_size[Default::music_button]);
        bgm_player->stop();
    }
    else{
        music_on = true;
        music_button->is_on = true;
        fadein_music(music_now_playing,Default::fadein_time);
        if(!music_button->is_white) music_button->setIcon(*music_button->pic_black_on);
        else music_button->setIcon(*music_button->pic_white_on);
        music_button->setIconSize(Default::button_size[Default::music_button]);
    }
    setFocus();
    return;
}

void Widget::refresh_button_Clicked(){
    refresh_button->sound_clicked_player->stop();
    resize(Default::bg_size);
    refresh_button->sound_clicked_player->play();
    //fadeout_music(1000);
    //fadeout_PixmapItem(Meguru,1000);
    //delay_for(1000);
    //fadein_PixmapItem(Meguru,1000);
    //fadein_music(bgm_bg_shinku,1000);
    setFocus();
    return;
}

void Widget::guide_button_Clicked()
{
    main_scene_button[Default::guide_button]->sound_clicked_player->stop();
    main_scene_button[Default::guide_button]->sound_clicked_player->stop();
    main_scene_button[Default::guide_button]->setIcon(*main_scene_button[Default::guide_button]->pic_normal);
    turn_to_level(level[1]);
    setFocus();
    return;
}

void Widget::select_button_Clicked()
{
    main_scene_button[Default::select_button]->sound_clicked_player->stop();
    main_scene_button[Default::select_button]->sound_clicked_player->play();
    pic_now_using = pic_bg_select;
    prev_scene = start_scene;
    select_scene->setSceneRect(pic_now_using->rect());
    ui->bg->fitInView(select_scene->sceneRect(),Qt::KeepAspectRatio);//将摄像机的大小初始与舞台大小固定
    ui->bg->setScene(select_scene);
    arrangeSelectSceneButtons();
    music_button->turn_to_black(Default::music_button);
    main_scene_button[Default::select_button]->setIcon(*main_scene_button[Default::select_button]->pic_normal);
    setFocus();
    return;
}

void Widget::extra_button_Clicked()
{
    main_scene_button[Default::extra_button]->sound_clicked_player->stop();
    main_scene_button[Default::extra_button]->sound_clicked_player->play();

    setFocus();
    return;
}

void Widget::system_button_Clicked()
{
    main_scene_button[Default::system_button]->sound_clicked_player->stop();
    main_scene_button[Default::system_button]->sound_clicked_player->play();

    setFocus();
    return;
}

void Widget::exit_button_Clicked()
{
    main_scene_button[Default::exit_button]->sound_clicked_player->stop();
    main_scene_button[Default::exit_button]->sound_clicked_player->play();
    setFocus();
    this->close();
    return;
}

void Widget::select_in_level_button_Clicked(){
    select_in_level_button->sound_clicked_player->stop();
    select_in_level_button->sound_clicked_player->play();
    select_in_level_button->setIcon(*select_in_level_button->pic_normal);
    pic_now_using = pic_bg_select;
    prev_scene = scene;
    select_scene->setSceneRect(pic_now_using->rect());
    ui->bg->fitInView(select_scene->sceneRect(),Qt::KeepAspectRatio);//将摄像机的大小初始与舞台大小固定
    ui->bg->setScene(select_scene);
    arrangeSelectSceneButtons();
    music_button->turn_to_black(Default::music_button);
    setFocus();
    return;
}

void Widget::level_button_Clicked(int i)
{
    level_button[i]->sound_clicked_player->stop();
    level_button[i]->sound_clicked_player->play();
    level_button[i]->setIcon(*level_button[i]->pic_normal);
    if(prev_scene == scene){
        scene->removeItem(Meguru->text_now_on);
        scene->removeItem(level_now_playing->text_of_cg);
        refresh_level(level_now_playing);
    }
    turn_to_level(level[i]);
    setFocus();
}

void Widget::return_prev_button_Clicked(){
    select_scene_button[Default::return_prev_button]->sound_clicked_player->stop();
    select_scene_button[Default::return_prev_button]->sound_clicked_player->play();
    select_scene_button[Default::return_prev_button]->setIcon(*select_scene_button[Default::return_prev_button]->pic_normal);
    if(prev_scene == start_scene){
        ui->bg->fitInView(start_scene->sceneRect(),Qt::KeepAspectRatio);//将摄像机的大小初始与舞台大小固定
        ui->bg->setScene(start_scene);
        move_button(refresh_button,start_scene);
        move_button(music_button,start_scene);
        pic_now_using = pic_bg_start;
        arrangeStartSceneButtons();
        setFocus();
    }
    else if(prev_scene == scene){
        ui->bg->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);//将摄像机的大小初始与舞台大小固定
        ui->bg->setScene(scene);
        pic_now_using = level_now_playing->cg_on ? level_now_playing->pic_of_cg : level_now_playing->pic_of_bg;
        arrangeLevelSceneButtons(level_now_playing->cg_on);
        if(level_now_playing->isdark) music_button->turn_to_white(Default::music_button);
        setFocus();
    }
    return;
}

void Widget::return_title_button_Clicked(){
    select_scene_button[Default::return_title_button]->sound_clicked_player->stop();
    select_scene_button[Default::return_title_button]->sound_clicked_player->play();
    select_scene_button[Default::return_title_button]->setIcon(*select_scene_button[Default::return_title_button]->pic_normal);
    if(prev_scene == start_scene){
        ui->bg->fitInView(start_scene->sceneRect(),Qt::KeepAspectRatio);//将摄像机的大小初始与舞台大小固定
        ui->bg->setScene(start_scene);
        move_button(refresh_button,start_scene);
        move_button(music_button,start_scene);
        pic_now_using = pic_bg_start;
        arrangeStartSceneButtons();
        music_button->turn_to_black(Default::music_button);
        setFocus();
    }
    else if(prev_scene == scene){
        if(music_on){
            fadeout_music(Default::fadeout_time);
        }
        fadeout_scene(Default::fadeout_time);
        scene->removeItem(Meguru->text_now_on);
        scene->removeItem(level_now_playing->text_of_cg);
        delay_for(Default::fadeout_time);
        fadein_scene(start_scene,Default::fadein_time);
        refresh_level(level_now_playing);
        level_now_playing = nullptr;
        ui->bg->fitInView(start_scene->sceneRect(),Qt::KeepAspectRatio);//将摄像机的大小初始与舞台大小固定
        ui->bg->setScene(start_scene);
        move_button(refresh_button,start_scene);
        move_button(music_button,start_scene);
        pic_now_using = pic_bg_start;
        arrangeStartSceneButtons();
        music_button->turn_to_black(Default::music_button);
        music_now_playing = bgm_bg_start;
        if(music_on){
            delay_for(Default::fadein_time);
            fadein_music(bgm_bg_start,Default::fadein_time);
        }
        setFocus();
    }
    return;
}

void Widget::retry_button_Clicked(){
    retry_button->sound_clicked_player->stop();
    retry_button->sound_clicked_player->play();
    retry_button->setIcon(*retry_button->pic_normal);
    scene->removeItem(Meguru);
    scene->removeItem(Meguru->text_now_on);
    scene->removeItem(level_now_playing->text_of_cg);
    turn_to_level(level_now_playing);
    setFocus();
};

void Widget::init_game()
{
    //ui->bg是ui界面生成的视图,视图View相当于"摄像机",场景Scene相当于"舞台",物品PixmapItem相当于"演员",Pixmap相当于演员的图像
    start_scene = new QGraphicsScene(ui->bg);//将舞台和摄像机绑定,确保二者一起随着ui界面被delete释放
    scene = new QGraphicsScene(ui->bg);
    select_scene = new QGraphicsScene(ui->bg);
    ui->bg->setScene(start_scene);//摄像机对准舞台
    prev_scene = start_scene;

    import_pics();//导入图片
    import_audios();

    pic_now_using=pic_bg_start;//引入背景图片
    *pic_now_using = pic_now_using->scaled(Default::bg_size,Qt::KeepAspectRatio);

    //加载背景图片
    start_scene->addPixmap(*pic_now_using);//演员登台
    start_scene->setSceneRect(pic_now_using->rect());//设置舞台的矩形尺寸,以契合演员图像的大小
    ui->bg->fitInView(start_scene->sceneRect(),Qt::KeepAspectRatio);//将摄像机的大小初始与舞台大小固定

    select_scene->addPixmap(*pic_bg_select);//演员登台
    select_scene->setSceneRect(pic_bg_select->rect());//设置舞台的矩形尺寸,以契合演员图像的大小

    bgm_player = new QMediaPlayer(this);//加载音乐播放器
    bgm_output = new QAudioOutput(this);//加载输出端
    bgm_player->setAudioOutput(bgm_output);//把输出端和播放器相连接
    bgm_output->setVolume(Default::bgm_volume);//调节音量

    //加载Meguru
    Meguru = new meguru();
    Meguru->setPixmap(*(Meguru->right_0_normal));
    Meguru->setPos(pic_now_using->width()/2,pic_now_using->height()/2);
    Meguru->setZValue(22);
    //start_scene->addItem(Meguru);

    import_buttons();

    //加载按钮
    arrangeStartSceneButtons();
    start_scene->addWidget(refresh_button);
    connect(refresh_button,&SCButton::clicked,this,&Widget::refresh_button_Clicked);
    start_scene->addWidget(main_scene_button[Default::guide_button]);
    connect(main_scene_button[Default::guide_button],&SCButton::clicked,this,&Widget::guide_button_Clicked);
    start_scene->addWidget(main_scene_button[Default::select_button]);
    connect(main_scene_button[Default::select_button],&SCButton::clicked,this,&Widget::select_button_Clicked);
    start_scene->addWidget(main_scene_button[Default::extra_button]);
    connect(main_scene_button[Default::extra_button],&SCButton::clicked,this,&Widget::extra_button_Clicked);
    start_scene->addWidget(main_scene_button[Default::system_button]);
    connect(main_scene_button[Default::system_button],&SCButton::clicked,this,&Widget::system_button_Clicked);
    start_scene->addWidget(main_scene_button[Default::exit_button]);
    connect(main_scene_button[Default::exit_button],&SCButton::clicked,this,&Widget::exit_button_Clicked);

    select_scene->addWidget(select_scene_button[Default::return_title_button]);
    placeButton(select_scene_button[Default::return_title_button],LayoutConfig::select_return_title_button_pos);
    select_scene->addWidget(select_scene_button[Default::return_prev_button]);
    placeButton(select_scene_button[Default::return_prev_button],LayoutConfig::select_return_prev_button_pos);
    connect(select_scene_button[Default::return_title_button],&SCButton::clicked,this,&Widget::return_title_button_Clicked);
    connect(select_scene_button[Default::return_prev_button],&SCButton::clicked,this,&Widget::return_prev_button_Clicked);

    connect(select_in_level_button,&SCButton::clicked,this,&Widget::select_in_level_button_Clicked);

    arrangeLevelButtons();


    start_scene->addWidget(music_button);
    connect(music_button,&TButton::clicked,this,&Widget::music_button_Clicked);

    placeButton(retry_button,LayoutConfig::start_music_button_pos);
    scene->addWidget(retry_button);
    connect(retry_button,&SCButton::clicked,this,&Widget::retry_button_Clicked);

    load_timer();
    connect(game_timer,&QTimer::timeout,this,&Widget::general_update);
    build_level();

    bgm_player = new QMediaPlayer(this);
    bgm_output = new QAudioOutput(this);
    bgm_player->setAudioOutput(bgm_output);
    if(music_on) fadein_music(bgm_bg_start,Default::fadein_time);
    music_now_playing = bgm_bg_start;

    backlog = new std::ofstream(PathUtils::nativeDataPath("backlog.txt"),std::ios::trunc);
    std::ifstream ayachi_nene(PathUtils::nativeDataPath("0d00____.txt"));
    std::string onanii;
    std::getline(ayachi_nene,onanii);
    if(onanii != "0721") hard =true;
}
