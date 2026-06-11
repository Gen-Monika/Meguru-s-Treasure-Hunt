#include "level.h"
#include "ResourceManager.h"

Level::Level(bool hard,QPixmap* pic_bg,QPixmap* pic_cg,QPixmap* pic_text_cg,QUrl* bgm_bg,QUrl* bgm_cg,QUrl* sound_text,bool dark) {
    if(hard){
        dig_trials = 7;
    }
    else{
        dig_trials = 21;
    }
    this->isdark = dark;
    this->pic_of_bg = pic_bg;
    this->pic_of_cg = pic_cg;
    this->pic_text_of_cg = pic_text_cg;
    this->bgm_of_bg = bgm_bg;
    this->bgm_of_cg = bgm_cg;
    this->sound_of_text = sound_text;

    level_bg = new QGraphicsPixmapItem();
    *pic_of_bg=pic_of_bg->scaled(Default::bg_size,Qt::KeepAspectRatio);
    level_bg->setPixmap(*pic_of_bg);
    level_bg->setPos(0,0);
    level_bg->setZValue(0);

    level_cg = new QGraphicsPixmapItem();
    *pic_of_cg = pic_of_cg->scaled(Default::cg_size,Qt::KeepAspectRatio);
    level_cg->setPixmap(*pic_of_cg);
    level_cg->setPos(0,0);
    level_cg->setZValue(0);

    if(this->sound_of_text != nullptr){
        this->sound_of_text_player = new QMediaPlayer();
        this->sound_of_text_output = new QAudioOutput();
        this->sound_of_text_player->setAudioOutput(this->sound_of_text_output);
        this->sound_of_text_player->setSource(*this->sound_of_text);
        this->sound_of_text_output->setVolume(Default::voice_volume);
    }

    if(this->pic_text_of_cg != nullptr){
        *pic_text_of_cg = pic_text_of_cg->scaled(Default::text_size,Qt::KeepAspectRatio);//缩放大小,并保证长宽比
        this->text_of_cg = new QGraphicsPixmapItem();
        text_of_cg->setPixmap(*pic_text_of_cg);
        text_of_cg->setPos(0,Default::bg_size.height()-Default::text_size.height());
        text_of_cg->setZValue(25);
    }

    //生成浮点比例[0,1)
    double percent_x =QRandomGenerator::global()->generateDouble();
    double percent_y =QRandomGenerator::global()->generateDouble();
    //分配宝藏位置
    this->target_x = Default::treasure_min_x + percent_x * (Default::treasure_max_x - Default::treasure_min_x);
    this->target_y = Default::treasure_min_y + percent_y * (Default::treasure_max_y - Default::treasure_min_y);
    qDebug()<<"宝藏坐标: "<<'('<<target_x<<','<<target_y<<')';
}

Level::Level(bool hard,int i)
{
    if(hard){
        dig_trials = 7;
    }
    else{
        dig_trials = 21;
    }
    this->index = i;

    if(i == Default::shinku || i == Default::cafe_0) this->isdark = true;

    this->pic_of_bg = ResourceManager::loadPixmap(Default::file_of_level[i].bg);
    this->pic_of_cg = ResourceManager::loadPixmap(Default::file_of_level[i].cg);
    this->pic_text_of_cg = ResourceManager::loadPixmap(Default::file_of_level[i].text);
    this->bgm_of_bg = ResourceManager::loadAudioUrl(Default::file_of_level[i].bgm_bg);
    this->bgm_of_cg = ResourceManager::loadAudioUrl(Default::file_of_level[i].bgm_cg);
    this->sound_of_text = ResourceManager::loadAudioUrl(Default::file_of_level[i].sound_text);

    level_bg = new QGraphicsPixmapItem();
    *pic_of_bg=pic_of_bg->scaled(Default::bg_size,Qt::KeepAspectRatio);
    level_bg->setPixmap(*pic_of_bg);
    level_bg->setPos(0,0);
    level_bg->setZValue(0);

    level_cg = new QGraphicsPixmapItem();
    *pic_of_cg = pic_of_cg->scaled(Default::cg_size,Qt::KeepAspectRatio);
    level_cg->setPixmap(*pic_of_cg);
    level_cg->setPos(0,0);
    level_cg->setZValue(0);

    if(this->sound_of_text != nullptr){
        this->sound_of_text_player = new QMediaPlayer();
        this->sound_of_text_output = new QAudioOutput();
        this->sound_of_text_player->setAudioOutput(this->sound_of_text_output);
        this->sound_of_text_player->setSource(*this->sound_of_text);
        this->sound_of_text_output->setVolume(Default::voice_volume);
    }

    if(this->pic_text_of_cg != nullptr){
        *pic_text_of_cg = pic_text_of_cg->scaled(Default::text_size,Qt::KeepAspectRatio);//缩放大小,并保证长宽比
        this->text_of_cg = new QGraphicsPixmapItem();
        text_of_cg->setPixmap(*pic_text_of_cg);
        text_of_cg->setPos(0,Default::bg_size.height()-Default::text_size.height());
        text_of_cg->setZValue(25);
    }

    //生成浮点比例[0,1)
    double percent_x =QRandomGenerator::global()->generateDouble();
    double percent_y =QRandomGenerator::global()->generateDouble();
    //分配宝藏位置
    this->target_x = Default::treasure_min_x + percent_x * (Default::treasure_max_x - Default::treasure_min_x);
    this->target_y = Default::treasure_min_y + percent_y * (Default::treasure_max_y - Default::treasure_min_y);
    qDebug()<<"宝藏坐标: "<<'('<<target_x<<','<<target_y<<')';
}

Level::~Level(){
    if(now_hole_num!=0){
        for(int i = 1;i<=now_hole_num;++i){
            delete Holes[i];
        }
    }
    delete level_bg;
    delete pic_of_bg;

    delete level_cg;
    delete pic_of_cg;

    delete bgm_of_bg;
    delete bgm_of_cg;

    if(text_of_cg) delete text_of_cg;

    if(sound_of_text_player) delete sound_of_text_player;
    if(sound_of_text_output) delete sound_of_text_output;
}
