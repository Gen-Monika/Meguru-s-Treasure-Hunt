#ifndef STORYCONFIG_H
#define STORYCONFIG_H

#include "config.h"

namespace StoryConfig {

//游戏开始前的剧情节点。可通过label和targetLabel实现类似goto的跳转。
const QList<Default::StoryStepConfig> default_pre_game_story_steps = {
    {
        "skip_choice",
        Default::StoryStepType::Choice,
        "玄",
        "是否跳过剧情？",
        "",
        "",
        {
            {"跳过剧情","end"},
            {"不跳过剧情","intro_1"}
        }
    },
    {
        "intro_1",
        Default::StoryStepType::Line,
        "玄",
        "那么，开始今天的挖宝之旅吧。",
        "",
        "end",
        {}
    },
    {
        "end",
        Default::StoryStepType::End,
        "",
        "",
        "",
        "",
        {}
    }
};

//按关卡索引配置开始前剧情。空列表表示该关卡不播放开始前剧情。
const QList<Default::StoryStepConfig> level_pre_game_story_steps[Default::level_button_count + 1] = {
    {},
    default_pre_game_story_steps,
    default_pre_game_story_steps,
    default_pre_game_story_steps,
    default_pre_game_story_steps,
    default_pre_game_story_steps,
    default_pre_game_story_steps,
    default_pre_game_story_steps,
    default_pre_game_story_steps,
    default_pre_game_story_steps,
    default_pre_game_story_steps
};

}

#endif // STORYCONFIG_H
