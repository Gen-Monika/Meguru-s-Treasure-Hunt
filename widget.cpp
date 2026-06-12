#include "widget.h"
#include "ui_widget.h"
#include "PathUtils.h"
#include "ResourceManager.h"
#include "StoryConfig.h"

namespace {
constexpr int story_choice_button_delete_delay_ms = 1200;
}

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
    clearDialogue(true);

    delete game_timer;
    //背景对象,与背景图片绑定以显示背景
    delete bg_start;//开始界面背景
    delete bg_select;


    //背景图片
    delete pic_bg_start;//开始界面背景图片
    delete pic_bg_select;

    delete bgm_player;//用来播放bgm
    delete bgm_output;//bgm输出端
    delete dialogue_voice_player;
    delete dialogue_voice_output;
    delete dialogue_hide_sound_player;
    delete dialogue_hide_sound_output;

    //bg场景的bgm
    delete bgm_bg_start;//开始界面

    //测试用洞
    delete hole_test;

    //洞
    delete pic_of_hole;

    delete Meguru;

    delete refresh_button;
    for(int i = 1;i<=Default::main_scene_button_count;++i){
        delete main_scene_button[i];
    }
    delete music_button;
    delete retry_button;
    for(int i = 1;i<=Default::level_button_count;++i){
        delete level_button[i];
    }
    for(int i = 1;i<=Default::select_scene_button_count;++i){
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
    for(int i = 1;i<=Default::main_scene_button_count;++i){
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
    for(int i = 1;i<=Default::level_button_count;++i){
        select_scene->addWidget(level_button[i]);
        placeButton(level_button[i],LayoutConfig::levelButtonPosition(i));
    }
}

bool Widget::hasActiveDialogue() const
{
    return dialogue_type != DialogueSceneType::None
        && dialogue_line_index >= 0
        && dialogue_item != nullptr;
}

void Widget::startDialogue(DialogueSceneType type,const QList<Default::DialogueLineConfig>& lines)
{
    clearDialogue(true);
    if(lines.isEmpty()){
        return;
    }

    dialogue_type = type;
    dialogue_lines = lines;
    showDialogueLine(0,true);
}

void Widget::showDialogueLine(int lineIndex,bool restartVoice)
{
    if(lineIndex < 0 || lineIndex >= dialogue_lines.size()){
        return;
    }

    clearDialogueItem();
    dialogue_line_index = lineIndex;
    dialogue_hidden = false;

    dialogue_item = new QGraphicsPixmapItem(
        DialogueRenderer::makeDialoguePixmap(dialogue_lines[dialogue_line_index]));
    dialogue_item->setPos(0,Default::bg_size.height() - Default::text_size.height());
    dialogue_item->setZValue(25);
    if(scene != nullptr){
        scene->addItem(dialogue_item);
    }

    if(restartVoice){
        playDialogueVoice(dialogue_lines[dialogue_line_index].voice);
    }
}

void Widget::advanceDialogue()
{
    if(!hasActiveDialogue()){
        return;
    }

    if(dialogue_line_index + 1 < dialogue_lines.size()){
        showDialogueLine(dialogue_line_index + 1,true);
        return;
    }

    if(dialogue_type == DialogueSceneType::DigHint){
        finishDialogue();
    }
}

void Widget::hideDialogue()
{
    if(!hasActiveDialogue() || dialogue_hidden || dialogue_item == nullptr){
        return;
    }

    if(dialogue_item->scene() != nullptr){
        dialogue_item->scene()->removeItem(dialogue_item);
    }
    dialogue_hidden = true;
    for(QGraphicsProxyWidget* proxy : story_choice_proxies){
        if(proxy != nullptr){
            proxy->setVisible(false);
        }
    }
    for(SCWButton* button : story_choice_buttons){
        if(button != nullptr){
            button->resetHoverState();
            button->hide();
        }
    }
    playDialogueHideSound();
}

void Widget::restoreDialogue()
{
    if(!hasActiveDialogue() || !dialogue_hidden || dialogue_item == nullptr){
        return;
    }

    if(dialogue_item->scene() == nullptr && scene != nullptr){
        scene->addItem(dialogue_item);
    }
    dialogue_hidden = false;
    for(QGraphicsProxyWidget* proxy : story_choice_proxies){
        if(proxy != nullptr){
            proxy->setVisible(true);
        }
    }
    for(SCWButton* button : story_choice_buttons){
        if(button != nullptr){
            button->show();
            button->refreshHoverState(false);
        }
    }
}

void Widget::finishDialogue()
{
    const DialogueSceneType finishedType = dialogue_type;
    clearDialogue(true);
    if(finishedType == DialogueSceneType::DigHint){
        restoreMeguruAfterDigHint();
    }
}

void Widget::clearDialogue(bool stopVoice)
{
    clearStoryChoices();
    story_active = false;
    story_start_pending = false;
    story_steps.clear();
    story_label_to_index.clear();
    story_step_index = -1;
    story_choice_show_sound_played = false;
    clearDialogueItem();
    dialogue_lines.clear();
    dialogue_line_index = -1;
    dialogue_hidden = false;
    dialogue_type = DialogueSceneType::None;
    if(stopVoice && dialogue_voice_player != nullptr){
        dialogue_voice_player->stop();
    }
}

void Widget::clearDialogueItem()
{
    if(dialogue_item == nullptr){
        return;
    }

    if(dialogue_item->scene() != nullptr){
        dialogue_item->scene()->removeItem(dialogue_item);
    }
    delete dialogue_item;
    dialogue_item = nullptr;
}

void Widget::playDialogueVoice(const QString& voicePath)
{
    if(dialogue_voice_player == nullptr){
        return;
    }

    dialogue_voice_player->stop();
    if(voicePath.isEmpty()){
        return;
    }

    const QUrl voiceUrl = ResourceManager::audioUrl(voicePath);
    if(voiceUrl.isEmpty()){
        return;
    }
    dialogue_voice_player->setLoops(QMediaPlayer::Once);
    dialogue_voice_player->setSource(voiceUrl);
    dialogue_voice_player->play();
}

void Widget::playDialogueHideSound()
{
    if(dialogue_hide_sound_player == nullptr){
        return;
    }

    dialogue_hide_sound_player->stop();
    dialogue_hide_sound_player->play();
}

void Widget::restoreMeguruAfterDigHint()
{
    if(Meguru == nullptr){
        return;
    }

    Meguru->digging = false;
    Meguru->text_now_on = nullptr;
    if(Meguru->face_left){
        Meguru->setPixmap(Meguru->statement == 0
            ? *(Meguru->left_0_normal)
            : *(Meguru->left_0_excited));
    }
    else{
        Meguru->setPixmap(Meguru->statement == 0
            ? *(Meguru->right_0_normal)
            : *(Meguru->right_0_excited));
    }
}

bool Widget::hasActiveStory() const
{
    return story_active
        && story_step_index >= 0
        && story_step_index < story_steps.size();
}

bool Widget::hasPreGameStoryForLevel(Level* level) const
{
    return level != nullptr
        && level->index >= 0
        && level->index <= Default::level_button_count
        && !StoryConfig::level_pre_game_story_steps[level->index].isEmpty();
}

bool Widget::isLevelSceneVisible() const
{
    return ui != nullptr
        && ui->bg != nullptr
        && ui->bg->scene() == scene
        && level_now_playing != nullptr;
}

bool Widget::isLevelControlLocked() const
{
    return !isLevelSceneVisible()
        || ui_transition_locked
        || story_start_pending
        || hasActiveStory();
}

bool Widget::tryBeginUiTransition()
{
    if(ui_transition_locked || story_choice_transitioning){
        setFocus();
        return false;
    }

    ui_transition_locked = true;
    move_set.clear();
    return true;
}

void Widget::endUiTransition()
{
    ui_transition_locked = false;
    setFocus();
}

void Widget::startStory(DialogueSceneType type,const QList<Default::StoryStepConfig>& steps)
{
    clearDialogue(true);
    if(steps.isEmpty()){
        return;
    }

    move_set.clear();
    story_start_pending = false;
    story_active = true;
    story_steps = steps;
    story_step_index = -1;
    story_choice_show_sound_played = false;
    story_choice_transitioning = false;
    dialogue_type = type;
    buildStoryLabelIndex();
    showStoryStep(0,true);
}

void Widget::buildStoryLabelIndex()
{
    story_label_to_index.clear();
    for(int i = 0; i < story_steps.size(); ++i){
        const QString& label = story_steps[i].label;
        if(!label.isEmpty()){
            story_label_to_index.insert(label,i);
        }
    }
}

void Widget::showStoryStep(int stepIndex,bool restartVoice)
{
    if(stepIndex < 0 || stepIndex >= story_steps.size()){
        finishStory();
        return;
    }

    clearStoryChoices();
    clearDialogueItem();
    story_step_index = stepIndex;
    dialogue_line_index = stepIndex;
    dialogue_hidden = false;
    story_choice_transitioning = false;

    const Default::StoryStepConfig& step = story_steps[story_step_index];
    if(step.type == Default::StoryStepType::End){
        finishStory();
        return;
    }

    dialogue_item = new QGraphicsPixmapItem(
        DialogueRenderer::makeDialoguePixmap(step.speaker,step.text));
    dialogue_item->setPos(0,Default::bg_size.height() - Default::text_size.height());
    dialogue_item->setZValue(25);
    if(scene != nullptr){
        scene->addItem(dialogue_item);
    }

    if(restartVoice){
        playDialogueVoice(step.voice);
    }

    if(step.type == Default::StoryStepType::Choice){
        showStoryChoices(step);
        if(!story_choice_show_sound_played){
            playDialogueHideSound();
            story_choice_show_sound_played = true;
        }
    }
}

void Widget::advanceStory()
{
    if(!hasActiveStory()){
        return;
    }

    const Default::StoryStepConfig& step = story_steps[story_step_index];
    if(step.type == Default::StoryStepType::Choice){
        return;
    }
    if(step.type == Default::StoryStepType::End){
        finishStory();
        return;
    }

    const int nextIndex = nextStoryStepIndex(step);
    if(nextIndex < 0){
        finishStory();
        return;
    }
    showStoryStep(nextIndex,true);
}

void Widget::jumpToStoryLabel(const QString& label)
{
    if(!story_active){
        return;
    }

    if(label.isEmpty()){
        advanceStory();
        return;
    }

    if(!story_label_to_index.contains(label)){
        qWarning() << "Missing story label:" << label;
        finishStory();
        return;
    }
    showStoryStep(story_label_to_index.value(label),true);
}

int Widget::nextStoryStepIndex(const Default::StoryStepConfig& step) const
{
    if(!step.nextLabel.isEmpty()){
        if(story_label_to_index.contains(step.nextLabel)){
            return story_label_to_index.value(step.nextLabel);
        }
        qWarning() << "Missing story nextLabel:" << step.nextLabel;
        return -1;
    }

    const int nextIndex = story_step_index + 1;
    return nextIndex < story_steps.size() ? nextIndex : -1;
}

void Widget::showStoryChoices(const Default::StoryStepConfig& step)
{
    if(scene == nullptr){
        return;
    }

    for(const Default::StoryChoiceConfig& choice : step.choices){
        SCWButton* button = new SCWButton(
            choice.text,
            Default::story_choice_normal_path,
            Default::story_choice_hovered_path,
            Default::story_choice_hover_sound_path,
            Default::story_choice_clicked_sound_path,
            Default::story_choice_button_size);
        button->setEnabled(false);

        QFont font;
        font.setPixelSize(Default::story_choice_button_font_px);
        font.setBold(true);
        button->setTextStyle(
            font,
            QColor(255,248,236),
            QColor(92,52,24,230),
            Default::story_choice_text_outline_width);

        QGraphicsProxyWidget* proxy = scene->addWidget(button);
        proxy->setZValue(30);
        proxy->setOpacity(0.0);
        story_choice_buttons.append(button);
        story_choice_proxies.append(proxy);

        const QString targetLabel = choice.targetLabel;
        connect(button,&SCWButton::clicked,this,[this,targetLabel]() {
            fadeOutStoryChoicesAndJump(targetLabel);
            setFocus();
        });
    }
    layoutStoryChoiceButtons();
    fadeInStoryChoices();
}

void Widget::layoutStoryChoiceButtons()
{
    const int count = story_choice_buttons.size();
    if(count <= 0){
        return;
    }

    const QSize stageSize = pic_now_using != nullptr ? pic_now_using->size() : Default::bg_size;
    const QSize buttonSize = story_choice_buttons.first() != nullptr
        ? story_choice_buttons.first()->size()
        : Default::story_choice_button_size;
    const int totalHeight = count * buttonSize.height()
        + (count - 1) * Default::story_choice_button_gap;
    const int left = (stageSize.width() - buttonSize.width()) / 2;

    const int availableTop = levelTopBarBottomForStoryChoices()
        + Default::story_choice_layout_top_margin;
    const int dialogueTop = stageSize.height() - Default::text_size.height();
    const int availableBottom = dialogueTop - Default::story_choice_layout_bottom_margin;
    const int availableHeight = availableBottom - availableTop;

    int top = (stageSize.height() - totalHeight) / 2;
    if(availableHeight > 0){
        top = availableTop + (availableHeight - totalHeight) / 2
            + Default::story_choice_layout_y_offset;
        if(totalHeight <= availableHeight){
            top = qBound(availableTop,top,availableBottom - totalHeight);
        }
        else{
            top = availableTop;
        }
    }

    for(int i = 0; i < count; ++i){
        const QPoint pos(
            left,
            top + i * (buttonSize.height() + Default::story_choice_button_gap));
        if(i < story_choice_proxies.size() && story_choice_proxies[i] != nullptr){
            story_choice_proxies[i]->setPos(pos);
        }
    }
}

int Widget::levelTopBarBottomForStoryChoices() const
{
    int bottom = 0;
    const auto includeButton = [&bottom](QPushButton* button) {
        if(button == nullptr){
            return;
        }

        QGraphicsProxyWidget* proxy = button->graphicsProxyWidget();
        if(proxy != nullptr){
            bottom = qMax(bottom,static_cast<int>(proxy->sceneBoundingRect().bottom() + 0.5));
            return;
        }

        bottom = qMax(bottom,button->y() + button->height());
    };

    includeButton(select_scene_button[Default::return_title_button]);
    includeButton(select_in_level_button);
    includeButton(refresh_button);
    includeButton(music_button);
    includeButton(retry_button);
    return bottom;
}

void Widget::fadeInStoryChoices()
{
    if(story_choice_proxies.isEmpty()){
        return;
    }

    stopStoryChoiceAnimation();
    story_choice_animation = new QParallelAnimationGroup(this);
    for(QGraphicsProxyWidget* proxy : story_choice_proxies){
        if(proxy == nullptr){
            continue;
        }
        QPropertyAnimation* animation = new QPropertyAnimation(proxy,"opacity");
        animation->setDuration(Default::story_choice_button_fadein_time);
        animation->setStartValue(proxy->opacity());
        animation->setEndValue(1.0);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        story_choice_animation->addAnimation(animation);
    }

    QParallelAnimationGroup* animationGroup = story_choice_animation;
    connect(animationGroup,&QParallelAnimationGroup::finished,this,[this,animationGroup]() {
        if(story_choice_animation == animationGroup){
            story_choice_animation = nullptr;
        }
        for(SCWButton* button : story_choice_buttons){
            if(button != nullptr){
                button->setEnabled(true);
            }
        }
        animationGroup->deleteLater();
    });
    story_choice_animation->start();
}

void Widget::fadeOutStoryChoicesAndJump(const QString& targetLabel)
{
    if(story_choice_transitioning){
        return;
    }

    if(story_choice_proxies.isEmpty()){
        jumpToStoryLabel(targetLabel);
        return;
    }

    story_choice_transitioning = true;
    for(SCWButton* button : story_choice_buttons){
        if(button != nullptr){
            button->setEnabled(false);
        }
    }

    stopStoryChoiceAnimation();
    story_choice_animation = new QParallelAnimationGroup(this);
    for(QGraphicsProxyWidget* proxy : story_choice_proxies){
        if(proxy == nullptr){
            continue;
        }
        QPropertyAnimation* animation = new QPropertyAnimation(proxy,"opacity");
        animation->setDuration(Default::story_choice_button_fadeout_time);
        animation->setStartValue(proxy->opacity());
        animation->setEndValue(0.0);
        animation->setEasingCurve(QEasingCurve::InCubic);
        story_choice_animation->addAnimation(animation);
    }

    QParallelAnimationGroup* animationGroup = story_choice_animation;
    connect(animationGroup,&QParallelAnimationGroup::finished,this,[this,animationGroup,targetLabel]() {
        if(story_choice_animation == animationGroup){
            story_choice_animation = nullptr;
        }
        animationGroup->deleteLater();
        story_choice_transitioning = false;
        jumpToStoryLabel(targetLabel);
    });
    story_choice_animation->start();
}

void Widget::clearStoryChoices()
{
    stopStoryChoiceAnimation();
    story_choice_transitioning = false;

    for(QGraphicsProxyWidget* proxy : story_choice_proxies){
        if(proxy == nullptr){
            continue;
        }
        proxy->setVisible(false);
        if(proxy->scene() != nullptr){
            proxy->scene()->removeItem(proxy);
        }
        proxy->setWidget(nullptr);
        proxy->deleteLater();
    }
    story_choice_proxies.clear();

    for(SCWButton* button : story_choice_buttons){
        if(button != nullptr){
            button->hide();
            button->setEnabled(false);
            button->setParent(this);
            QTimer::singleShot(
                story_choice_button_delete_delay_ms,
                button,
                &QObject::deleteLater);
        }
    }
    story_choice_buttons.clear();
}

void Widget::finishStory()
{
    clearDialogue(true);
    move_set.clear();
    setFocus();
}

void Widget::startPreGameStoryForLevel(Level* level)
{
    if(!hasPreGameStoryForLevel(level)){
        story_start_pending = false;
        return;
    }

    startStory(
        DialogueSceneType::PreGameStory,
        StoryConfig::level_pre_game_story_steps[level->index]);
}

void Widget::stopStoryChoiceAnimation()
{
    if(story_choice_animation == nullptr){
        return;
    }

    story_choice_animation->stop();
    story_choice_animation->deleteLater();
    story_choice_animation = nullptr;
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
    if(music == nullptr || music->isEmpty()){
        return;
    }

    const int fadeGeneration = ++bgm_fade_generation;
    bgm_player->setSource(*music);
    bgm_output->setVolume(0.0);

    QTimer* fadein_timer = new QTimer(this);
    fadein_timer->setInterval(Default::Game_Period);

    double step = Default::bgm_volume / ((double)fadein_time / (double)Default::Game_Period);

    connect(fadein_timer, &QTimer::timeout, this, [this, fadein_timer, step, fadeGeneration, currentVolume = 0.0]() mutable {
        if(fadeGeneration != bgm_fade_generation){
            fadein_timer->stop();
            fadein_timer->deleteLater();
            return;
        }

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
    const int fadeGeneration = ++bgm_fade_generation;
    bgm_output->setVolume(Default::bgm_volume);

    QTimer* fadeout_timer = new QTimer(this);
    fadeout_timer->setInterval(Default::Game_Period);

    double step = Default::bgm_volume / ((double)fadeout_time / (double)Default::Game_Period);

    connect(fadeout_timer, &QTimer::timeout, this, [this, fadeout_timer, step, fadeGeneration, currentVolume = Default::bgm_volume]() mutable {
        if(fadeGeneration != bgm_fade_generation){
            fadeout_timer->stop();
            fadeout_timer->deleteLater();
            return;
        }

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
    if(isLevelControlLocked()) return; //剧情播放或等待弹出时暂时禁止移动
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
            if(dialogue_type == DialogueSceneType::DigHint){
                finishDialogue();
            }
            else{
                Meguru->digging = false;
                if(Meguru->text_now_on != nullptr){
                    scene->removeItem(Meguru->text_now_on);
                    Meguru->text_now_on = nullptr;
                }
            }
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
        dx *= Default::diagonal_speed_factor;
        dy *= Default::diagonal_speed_factor;
    }
    //边界判断
    if( (Meguru->pos().x() <= pic_now_using->width() * Default::Meguru_min_x_ratio && dx < 0)
        || (Meguru->pos().x() >= pic_now_using->width() * Default::Meguru_max_x_ratio && dx > 0) ) dx *= 0.0;
    if( (Meguru->pos().y() <= pic_now_using->height() * Default::Meguru_min_y_ratio && dy < 0)
        || (Meguru->pos().y() >= pic_now_using->height() * Default::Meguru_max_y_ratio && dy > 0) ) dy *= 0.0;
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
    if(!isLevelSceneVisible()) return;
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
    scene->addItem(level_now_playing->Holes[level_now_playing->now_hole_num]);
    qDebug()<<"洞的横坐标："<<get_hole_center(level_now_playing->Holes[level_now_playing->now_hole_num]).x();
    qDebug()<<"洞的纵坐标："<<get_hole_center(level_now_playing->Holes[level_now_playing->now_hole_num]).y();
    (*backlog)<<"洞的坐标:("<<get_hole_center(level_now_playing->Holes[level_now_playing->now_hole_num]).x()\
               <<","<<get_hole_center(level_now_playing->Holes[level_now_playing->now_hole_num]).y()<<")"<<std::endl;
    (*backlog)<<"剩余次数:"<<level_now_playing->dig_trials-level_now_playing->now_hole_num<<std::endl;
    double hole_x = get_hole_center(level_now_playing->Holes[level_now_playing->now_hole_num]).x();
    double hole_y = get_hole_center(level_now_playing->Holes[level_now_playing->now_hole_num]).y();
    //获胜
    if( (hole_x - target_x)*(hole_x - target_x) + (hole_y - target_y)*(hole_y - target_y) <= Default::win_boundary*Default::win_boundary ){
        Meguru->win = true;
        Meguru->digging = true;
        Meguru->statement = 2;
        if(Meguru->face_left){
            Meguru->setPixmap(*(Meguru->left_1_win));
        }
        else{
            Meguru->setPixmap(*(Meguru->right_1_win));
        }
        startDialogue(DialogueSceneType::WinHint,{Default::dialogue_thats_it});
        (*backlog)<<"找到了!"<<std::endl<<std::endl;
    }
    else if( (hole_x - target_x)*(hole_x - target_x) + (hole_y - target_y)*(hole_y - target_y) <= Default::very_near_boundary*Default::very_near_boundary ){
        Meguru->digging = true;
        Meguru->statement = 1;
        if(Meguru->face_left){
            Meguru->setPixmap(*(Meguru->left_1_excited));
        }
        else{
            Meguru->setPixmap(*(Meguru->right_1_excited));
        }
        startDialogue(DialogueSceneType::DigHint,{Default::dialogue_very_near});
        (*backlog)<<"就差一点了!直线距离在"<<Default::very_near_boundary<<"个像素点以内!"<<std::endl<<std::endl;
    }
    else if( (hole_x - target_x)*(hole_x - target_x) + (hole_y - target_y)*(hole_y - target_y) <= Default::near_boundary*Default::near_boundary ){
        Meguru->digging = true;
        Meguru->statement = 1;
        if(Meguru->face_left){
            Meguru->setPixmap(*(Meguru->left_1_excited));
        }
        else{
            Meguru->setPixmap(*(Meguru->right_1_excited));
        }
        startDialogue(DialogueSceneType::DigHint,{Default::dialogue_near});
        (*backlog)<<"已经很近了,直线距离在"<<Default::near_boundary<<"个像素点以内!"<<std::endl<<std::endl;
    }
    else if( (hole_x - target_x)*(hole_x - target_x) + (hole_y - target_y)*(hole_y - target_y) <= Default::medium_boundary*Default::medium_boundary ){
        Meguru->digging = true;
        Meguru->statement = 0;
        if(Meguru->face_left){
            Meguru->setPixmap(*(Meguru->left_1_normal));
        }
        else{
            Meguru->setPixmap(*(Meguru->right_1_normal));
        }
        startDialogue(DialogueSceneType::DigHint,{Default::dialogue_medium});
        (*backlog)<<"距离已经不远了,直线距离在"<<Default::medium_boundary<<"个像素点以内."<<std::endl<<std::endl;
    }
    else if( (hole_x - target_x)*(hole_x - target_x) + (hole_y - target_y)*(hole_y - target_y) <= Default::far_boundary*Default::far_boundary ){
        Meguru->digging = true;
        Meguru->statement = 0;
        if(Meguru->face_left){
            Meguru->setPixmap(*(Meguru->left_1_normal));
        }
        else{
            Meguru->setPixmap(*(Meguru->right_1_normal));
        }
        startDialogue(DialogueSceneType::DigHint,{Default::dialogue_far});
        (*backlog)<<"是这个方向没有错,直线距离在"<<Default::far_boundary<<"个像素点以内."<<std::endl<<std::endl;
    }
    else {
        Meguru->digging = true;
        Meguru->statement = 0;
        if(Meguru->face_left){
            Meguru->setPixmap(*(Meguru->left_1_normal));
        }
        else{
            Meguru->setPixmap(*(Meguru->right_1_normal));
        }
        startDialogue(DialogueSceneType::DigHint,{Default::dialogue_far_away});
        (*backlog)<<"距离还很远,换个方向吧."<<std::endl<<std::endl;
    }
    if(!Meguru->win && level_now_playing->now_hole_num == level_now_playing->dig_trials){
        level_now_playing->fail = true;
        startDialogue(DialogueSceneType::LoseHint,{Default::dialogue_failed});
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
    for(int i = 1;i<=Default::level_button_count;++i){
        level[i] = new Level(hard,i);
    }
}

void Widget::refresh_level(Level *level)
{
    if(level == nullptr){
        return;
    }

    clearDialogue(true);
    scene->removeItem(level->level_bg);
    scene->removeItem(level->level_cg);
    level->fail = false;
    level->cg_on = false;
    if(level->now_hole_num!=0){
        for(int i = 1;i<=level->now_hole_num;++i){
            if(level->Holes[i] != nullptr){
                scene->removeItem(level->Holes[i]);
                delete level->Holes[i];
                level->Holes[i] = nullptr;
            }
        }
    }
    level->now_hole_num = 0;
    if(hard){
        level->dig_trials = Default::hard_dig_trials;
    }
    else{
        level->dig_trials = Default::normal_dig_trials;
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
    story_start_pending = hasPreGameStoryForLevel(level);
    if(story_start_pending){
        move_set.clear();
    }
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

    if(level->isdark) music_button->turn_to_white();
    else music_button->turn_to_black();
    delay_for(Default::fadein_time);
    if(music_on){
        fadein_music(music_now_playing,Default::fadein_time);
    }
    startPreGameStoryForLevel(level);
    if(!hasActiveStory()){
        story_start_pending = false;
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
    if(level == nullptr || !isLevelSceneVisible()){
        return;
    }

    level_now_playing->cg_on = true;
    if(dialogue_voice_player != nullptr){
        dialogue_voice_player->stop();
    }
    if(music_on){
        fadeout_music(Default::fadeout_time);
    }
    fadeout_scene(Default::fadeout_time);
    delay_for(Default::fadeout_time);
    clearDialogue(false);
    if(Meguru->text_now_on != nullptr){
        scene->removeItem(Meguru->text_now_on);
        Meguru->text_now_on = nullptr;
    }
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
    if(level->index >= 0 && level->index <= Default::level_button_count
        && !Default::level_dialogue_lines[level->index].isEmpty()){
        startDialogue(DialogueSceneType::CgStory,Default::level_dialogue_lines[level->index]);
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
        if(isLevelControlLocked()){
            move_set.remove(event->key());
            break;
        }
        if(hasActiveDialogue()){
            if(dialogue_type == DialogueSceneType::DigHint){
                finishDialogue();
            }
            else{
                break;
            }
        }
        move_set.insert(event->key());
        break;
    case Qt::Key_Space:
        if(isLevelControlLocked() || hasActiveDialogue()){
            break;
        }
        if(Meguru) Meguru_dig();
        break;
    case Qt::Key_Enter:
        if(isLevelControlLocked()){
            break;
        }
        if(Meguru){
            if(Meguru->win && !level_now_playing->cg_on && tryBeginUiTransition()){
                show_cg(level_now_playing);
                endUiTransition();
            }
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
    if(ui_transition_locked){
        setFocus();
        return;
    }

    if(story_choice_transitioning){
        setFocus();
        return;
    }

    if((hasActiveStory() || hasActiveDialogue()) && !isLevelSceneVisible()){
        setFocus();
        return;
    }

    if(event->button() == Qt::RightButton && (hasActiveStory() || hasActiveDialogue())){
        hideDialogue();
        setFocus();
        return;
    }

    if(event->button() == Qt::LeftButton && hasActiveStory()){
        if(dialogue_hidden){
            restoreDialogue();
        }
        else{
            advanceStory();
        }
        setFocus();
        return;
    }

    if(event->button() == Qt::LeftButton && hasActiveDialogue()){
        if(dialogue_hidden){
            restoreDialogue();
        }
        else{
            advanceDialogue();
        }
        setFocus();
        return;
    }

    setFocus();
    return;
}

void Widget::changeEvent(QEvent *event)
{
    Q_UNUSED(event);
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
    refresh_button = new SCButton(Default::refresh_button,0);
    for(int i = 1;i<=Default::main_scene_button_count;++i){
        main_scene_button[i] = new SCButton(Default::main_scene_button,i);
    }
    music_button = new SCCheckbox(Default::music_button,0);
    retry_button = new SCButton(Default::retry_button,0);
    for(int i = 1;i<=Default::level_button_count;++i){
        level_button[i] = new SCButton(Default::level_button,i);
        connect(level_button[i], &SCButton::clicked, this, [this, i]() {
            level_button_Clicked(i);
            setFocus();
        });
    }
    for(int i = 1;i<=Default::select_scene_button_count;++i){
        select_scene_button[i] = new SCButton(Default::select_scene_button,i);
    }
    select_in_level_button = new SCButton(Default::select_in_level_button,0);
}

void Widget::import_audios()
{
    bgm_bg_start = ResourceManager::loadAudioUrl(PATH_OF_bgm_bg_start);

    dialogue_voice_player = new QMediaPlayer(this);
    dialogue_voice_output = new QAudioOutput(this);
    dialogue_voice_player->setAudioOutput(dialogue_voice_output);
    dialogue_voice_output->setVolume(Default::voice_volume);

    dialogue_hide_sound_player = new QMediaPlayer(this);
    dialogue_hide_sound_output = new QAudioOutput(this);
    dialogue_hide_sound_player->setAudioOutput(dialogue_hide_sound_output);
    dialogue_hide_sound_player->setSource(ResourceManager::audioUrl(Default::dialogue_hide_sound_path));
    dialogue_hide_sound_output->setVolume(Default::button_volume);
}

void Widget::music_button_Clicked()
{
    if(music_on){
        music_on = false;
        music_button->setChecked(false);
        bgm_player->stop();
    }
    else{
        music_on = true;
        fadein_music(music_now_playing,Default::fadein_time);
        music_button->setChecked(true);
    }
    setFocus();
    return;
}

void Widget::refresh_button_Clicked(){
    refresh_button->sound_clicked_player->stop();
    resize(Default::bg_size);
    refresh_button->sound_clicked_player->play();
    setFocus();
    return;
}

void Widget::guide_button_Clicked()
{
    if(!tryBeginUiTransition()){
        return;
    }

    main_scene_button[Default::guide_button]->sound_clicked_player->stop();
    main_scene_button[Default::guide_button]->sound_clicked_player->play();
    main_scene_button[Default::guide_button]->setIcon(*main_scene_button[Default::guide_button]->pic_normal);
    turn_to_level(level[Default::senren_0]);
    endUiTransition();
    return;
}

void Widget::select_button_Clicked()
{
    if(!tryBeginUiTransition()){
        return;
    }

    main_scene_button[Default::select_button]->sound_clicked_player->stop();
    main_scene_button[Default::select_button]->sound_clicked_player->play();
    pic_now_using = pic_bg_select;
    prev_scene = start_scene;
    select_scene->setSceneRect(pic_now_using->rect());
    ui->bg->fitInView(select_scene->sceneRect(),Qt::KeepAspectRatio);//将摄像机的大小初始与舞台大小固定
    ui->bg->setScene(select_scene);
    arrangeSelectSceneButtons();
    music_button->turn_to_black();
    main_scene_button[Default::select_button]->setIcon(*main_scene_button[Default::select_button]->pic_normal);
    endUiTransition();
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
    if(!tryBeginUiTransition()){
        return;
    }

    select_in_level_button->sound_clicked_player->stop();
    select_in_level_button->sound_clicked_player->play();
    select_in_level_button->setIcon(*select_in_level_button->pic_normal);
    pic_now_using = pic_bg_select;
    prev_scene = scene;
    select_scene->setSceneRect(pic_now_using->rect());
    ui->bg->fitInView(select_scene->sceneRect(),Qt::KeepAspectRatio);//将摄像机的大小初始与舞台大小固定
    ui->bg->setScene(select_scene);
    arrangeSelectSceneButtons();
    music_button->turn_to_black();
    endUiTransition();
    return;
}

void Widget::level_button_Clicked(int i)
{
    if(!tryBeginUiTransition()){
        return;
    }

    level_button[i]->sound_clicked_player->stop();
    level_button[i]->sound_clicked_player->play();
    level_button[i]->setIcon(*level_button[i]->pic_normal);
    if(prev_scene == scene){
        clearDialogue(true);
        if(Meguru->text_now_on != nullptr){
            scene->removeItem(Meguru->text_now_on);
            Meguru->text_now_on = nullptr;
        }
        if(level_now_playing->text_of_cg != nullptr){
            scene->removeItem(level_now_playing->text_of_cg);
        }
        refresh_level(level_now_playing);
    }
    turn_to_level(level[i]);
    endUiTransition();
}

void Widget::return_prev_button_Clicked(){
    if(!tryBeginUiTransition()){
        return;
    }

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
    }
    else if(prev_scene == scene){
        ui->bg->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);//将摄像机的大小初始与舞台大小固定
        ui->bg->setScene(scene);
        pic_now_using = level_now_playing->cg_on ? level_now_playing->pic_of_cg : level_now_playing->pic_of_bg;
        arrangeLevelSceneButtons(level_now_playing->cg_on);
        if(level_now_playing->isdark) music_button->turn_to_white();
    }
    endUiTransition();
    return;
}

void Widget::return_title_button_Clicked(){
    if(!tryBeginUiTransition()){
        return;
    }

    SCButton* sound_button = prev_scene == start_scene
        ? select_scene_button[Default::return_prev_button]
        : select_scene_button[Default::return_title_button];
    sound_button->sound_clicked_player->stop();
    sound_button->sound_clicked_player->play();
    select_scene_button[Default::return_title_button]->setIcon(*select_scene_button[Default::return_title_button]->pic_normal);
    if(prev_scene == start_scene){
        ui->bg->fitInView(start_scene->sceneRect(),Qt::KeepAspectRatio);//将摄像机的大小初始与舞台大小固定
        ui->bg->setScene(start_scene);
        move_button(refresh_button,start_scene);
        move_button(music_button,start_scene);
        pic_now_using = pic_bg_start;
        arrangeStartSceneButtons();
        music_button->turn_to_black();
    }
    else if(prev_scene == scene){
        if(music_on){
            fadeout_music(Default::fadeout_time);
        }
        fadeout_scene(Default::fadeout_time);
        delay_for(Default::fadeout_time);
        clearDialogue(true);
        if(Meguru->text_now_on != nullptr){
            scene->removeItem(Meguru->text_now_on);
            Meguru->text_now_on = nullptr;
        }
        if(level_now_playing->text_of_cg != nullptr){
            scene->removeItem(level_now_playing->text_of_cg);
        }
        fadein_scene(start_scene,Default::fadein_time);
        refresh_level(level_now_playing);
        level_now_playing = nullptr;
        ui->bg->fitInView(start_scene->sceneRect(),Qt::KeepAspectRatio);//将摄像机的大小初始与舞台大小固定
        ui->bg->setScene(start_scene);
        move_button(refresh_button,start_scene);
        move_button(music_button,start_scene);
        pic_now_using = pic_bg_start;
        arrangeStartSceneButtons();
        music_button->turn_to_black();
        music_now_playing = bgm_bg_start;
        if(music_on){
            delay_for(Default::fadein_time);
            fadein_music(bgm_bg_start,Default::fadein_time);
        }
    }
    endUiTransition();
    return;
}

void Widget::retry_button_Clicked(){
    if(!tryBeginUiTransition()){
        return;
    }

    retry_button->sound_clicked_player->stop();
    retry_button->sound_clicked_player->play();
    retry_button->setIcon(*retry_button->pic_normal);

    Level* level = level_now_playing;
    if(level == nullptr){
        endUiTransition();
        return;
    }

    if(music_on){
        fadeout_music(Default::fadeout_time);
    }
    fadeout_scene(Default::fadeout_time);
    delay_for(Default::fadeout_time);

    clearDialogue(true);
    scene->removeItem(Meguru);
    if(Meguru->text_now_on != nullptr){
        scene->removeItem(Meguru->text_now_on);
        Meguru->text_now_on = nullptr;
    }
    if(level->text_of_cg != nullptr){
        scene->removeItem(level->text_of_cg);
    }
    refresh_level(level);
    story_start_pending = hasPreGameStoryForLevel(level);
    if(story_start_pending){
        move_set.clear();
    }

    fadein_scene(scene,Default::fadein_time);
    music_now_playing = level->bgm_of_bg;
    pic_now_using = level->pic_of_bg;
    scene->addItem(level->level_bg);
    scene->setSceneRect(pic_now_using->rect());
    ui->bg->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    prev_scene = scene;

    Meguru_refresh();
    scene->addItem(Meguru);

    arrangeLevelSceneButtons(false);
    if(level->isdark) music_button->turn_to_white();
    else music_button->turn_to_black();
    delay_for(Default::fadein_time);
    if(music_on){
        fadein_music(music_now_playing,Default::fadein_time);
    }
    startPreGameStoryForLevel(level);
    if(!hasActiveStory()){
        story_start_pending = false;
    }
    endUiTransition();
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
    connect(music_button,&SCCheckbox::clicked,this,&Widget::music_button_Clicked);

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
