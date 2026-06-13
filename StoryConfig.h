#ifndef STORYCONFIG_H
#define STORYCONFIG_H

#include "config.h"

namespace StoryConfig {

const QString default_pre_game_story_module = "pre_game_default";

const QString level_3_pre_game_story_module = "pre_game_level_3";

const QString level_3_cg_story_module = "cg_level_3";
const QString level_4_cg_story_module = "cg_level_4";

//游戏开始前的剧情节点。targetLabel/nextLabel可写成"label"或"module:label"。
const QList<Default::StoryStepConfig> default_pre_game_story_steps = {
    {
        "skip_choice",
        Default::StoryStepType::Choice,
        "",
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

const QList<Default::StoryStepConfig> level_3_pre_game_story_steps = {
    {
        "skip_choice",
        Default::StoryStepType::Choice,
        "",
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
        "柊史",
        "是不是稍微有些暗下来了啊？",
        "",
        "intro_2",
        {}
    },
    {
        "intro_2",
        Default::StoryStepType::Line,
        "巡",
        "那把灯打开吧。宁宁学姐，可以开灯的吧？",
        "../../Resources/Audios/Voice-s/Levels/level_3/pre/3-pre-1.flac",
        "intro_3",
        {}
    },
    {
        "intro_3",
        Default::StoryStepType::Line,
        "宁宁",
        "好的，当然没问题。开关的遥控器应该是放在沙发附近的",
        "../../Resources/Audios/Voice-s/Levels/level_3/pre/3-pre-2.flac",
        "intro_4",
        {}
    },
    {
        "intro_4",
        Default::StoryStepType::Line,
        "巡",
        "收到（Ro ger）的说",
        "../../Resources/Audios/Voice-s/Levels/level_3/pre/3-pre-3.flac",
        "intro_5",
        {}
    },
    {
        "intro_5",
        Default::StoryStepType::Line,
        "巡",
        "遥控器遥控器……",
        "../../Resources/Audios/Voice-s/Levels/level_3/pre/3-pre-4.flac",
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

//挖宝成功后的CG剧情。最后一段如果不设置nextLabel,左键不会关闭CG对话。
const QList<Default::StoryStepConfig> level_3_cg_story_steps = {
    {
        "cg_start",
        Default::StoryStepType::Line,
        "巡",
        "诶……啊，找到了哦。不过……咦？",
        "../../Resources/Audios/Voice-s/Levels/level_3/cg/3-cg-1.flac",
        "cg_1",
        {}
    },
    {
        "cg_1",
        Default::StoryStepType::Line,
        "宁宁",
        "那个，不对，啊哇，啊嘎嘎嘎——",
        "../../Resources/Audios/Voice-s/Levels/level_3/cg/Nene.flac",
        "cg_2",
        {}
    },
    {
        "cg_2",
        Default::StoryStepType::Line,
        "巡",
        "那么，我开灯了哦",
        "../../Resources/Audios/Voice-s/Levels/level_3/cg/3-cg-2.flac",
        "cg_3",
        {}
    },
    {
        "cg_3",
        Default::StoryStepType::Line,
        "宁宁",
        "等等，因幡同学，请稍等一下！！！",
        "../../Resources/Audios/Voice-s/Levels/level_3/cg/3-cg-3.flac",
        "cg_4",
        {}
    },
    {
        "cg_4",
        Default::StoryStepType::Line,
        "巡",
        "嗯？怎么了吗？",
        "../../Resources/Audios/Voice-s/Levels/level_3/cg/3-cg-4.flac",
        "cg_5",
        {}
    },
    {
        "cg_5",
        Default::StoryStepType::Line,
        "宁宁",
        "那个，不行，那个，开关，不能按，绝对不能按",
        "../../Resources/Audios/Voice-s/Levels/level_3/cg/3-cg-5.flac",
        "cg_6",
        {}
    },
    {
        "cg_6",
        Default::StoryStepType::Line,
        "宁宁",
        "那是起爆的开关！",
        "../../Resources/Audios/Voice-s/Levels/level_3/cg/3-cg-6.flac",
        "cg_7",
        {}
    },
    {
        "cg_7",
        Default::StoryStepType::Line,
        "巡",
        "起爆！？",
        "../../Resources/Audios/Voice-s/Levels/level_3/cg/3-cg-7.flac",
        "cg_8",
        {}
    },
    {
        "cg_8",
        Default::StoryStepType::Line,
        "宁宁",
        "按下那个开关的话，这个房间，就会爆炸，会自爆的！",
        "../../Resources/Audios/Voice-s/Levels/level_3/cg/3-cg-8.flac",
        "cg_9",
        {}
    },
    {
        "cg_9",
        Default::StoryStepType::Line,
        "巡",
        "为什么会有那么恐怖的开关！？",
        "../../Resources/Audios/Voice-s/Levels/level_3/cg/3-cg-9.flac",
        "cg_10",
        {}
    },
    {
        "cg_10",
        Default::StoryStepType::Line,
        "",
        "恭喜你解锁成就【起爆开关】！",
        "",
        "",
        {}
    },
};

const QList<Default::StoryStepConfig> level_4_cg_story_steps = {
    {
        "cg_start",
        Default::StoryStepType::Line,
        "憧子",
        "来，啊——嗯",
        "../../Resources/Audios/Voice-s/Touko.flac",
        "",
        {}
    }
};

const QList<Default::StoryStepConfig> empty_story_steps = {};

//按关卡索引配置开始前剧情模块。空字符串表示该关卡不播放开始前剧情。
const QString level_pre_game_story_modules[Default::level_button_count + 1] = {
    "",
    default_pre_game_story_module,
    default_pre_game_story_module,
    level_3_pre_game_story_module,
    default_pre_game_story_module,
    default_pre_game_story_module,
    default_pre_game_story_module,
    default_pre_game_story_module,
    default_pre_game_story_module,
    default_pre_game_story_module,
    default_pre_game_story_module
};

//按关卡索引配置CG剧情模块。空字符串表示该关卡暂无CG剧情。
const QString level_cg_story_modules[Default::level_button_count + 1] = {
    "",
    "",
    "",
    level_3_cg_story_module,
    level_4_cg_story_module,
    "",
    "",
    "",
    "",
    "",
    ""
};

inline bool isValidLevelIndex(int levelIndex)
{
    return levelIndex >= 0 && levelIndex <= Default::level_button_count;
}

inline QString preGameStoryModuleForLevel(int levelIndex)
{
    return isValidLevelIndex(levelIndex)
        ? level_pre_game_story_modules[levelIndex]
        : QString();
}

inline QString cgStoryModuleForLevel(int levelIndex)
{
    return isValidLevelIndex(levelIndex)
        ? level_cg_story_modules[levelIndex]
        : QString();
}

inline const QList<Default::StoryStepConfig>& storySteps(const QString& moduleId)
{
    if(moduleId == default_pre_game_story_module){
        return default_pre_game_story_steps;
    }
    if(moduleId == level_3_pre_game_story_module){
        return level_3_pre_game_story_steps;
    }
    if(moduleId == level_3_cg_story_module){
        return level_3_cg_story_steps;
    }
    if(moduleId == level_4_cg_story_module){
        return level_4_cg_story_steps;
    }
    return empty_story_steps;
}

}

#endif // STORYCONFIG_H
