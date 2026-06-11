#include "meguru.h"

meguru::meguru() {
    //设置ciallo音效
    sound_ciallo = ResourceManager::loadAudioUrl(PATH_OF_SOUND_CIALLO);
    sound_ciallo_player = new QMediaPlayer();
    sound_ciallo_output = new QAudioOutput();
    sound_ciallo_player->setAudioOutput(sound_ciallo_output);
    sound_ciallo_player->setSource(*sound_ciallo);
    sound_ciallo_output->setVolume(Default::voice_volume);
    //设置获胜音效
    sound_win = ResourceManager::loadAudioUrl(PATH_OF_SOUND_WIN);
    sound_win_player = new QMediaPlayer();
    sound_win_output = new QAudioOutput();
    sound_win_player->setAudioOutput(sound_win_output);
    sound_win_player->setSource(*sound_win);
    sound_win_output->setVolume(Default::voice_volume);

    //导入图片素材
    import_meguru_item(left_0_normal);
    import_meguru_item(right_0_normal);
    import_meguru_item(left_0_excited);
    import_meguru_item(right_0_excited);

    import_meguru_item(left_1_normal);
    import_meguru_item(right_1_normal);
    import_meguru_item(left_1_excited);
    import_meguru_item(right_1_excited);
    import_meguru_item(left_1_win);
    import_meguru_item(right_1_win);

    import_meguru_related_item(failed);
    import_meguru_related_item(far_away);
    import_meguru_related_item(far);
    import_meguru_related_item(medium);
    import_meguru_related_item(near);
    import_meguru_related_item(very_near);
    import_meguru_related_item(thats_it);

    //设置速度
    meguru_speed = Default::Meguru_speed;
}

meguru::~meguru(){
    //释放图片对象
    delete left_0_normal;
    delete right_0_normal;
    delete left_0_excited;
    delete right_0_excited;

    delete left_1_normal;
    delete right_1_normal;
    delete left_1_excited;
    delete right_1_excited;
    delete left_1_win;
    delete right_1_win;

    delete far_away;
    delete far;
    delete medium;
    delete near;
    delete very_near;
    delete thats_it;
    delete failed;

    delete pic_far_away;
    delete pic_far;
    delete pic_medium;
    delete pic_near;
    delete pic_very_near;
    delete pic_thats_it;
    delete pic_failed;
    //释放音效
    delete sound_ciallo_player;
    delete sound_ciallo_output;
    delete sound_ciallo;

    delete sound_win_player;
    delete sound_win_output;
    delete sound_win;
}
