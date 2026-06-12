#ifndef CONFIG_H
#define CONFIG_H

#include <QSize>
#include <QSizeF>
#include <QString>

namespace Default{

//主界面等界面的文件路径
struct File_of_scene{
    QString bg;
    QString bgm;
};

//关卡界面的文件路径
struct File_of_level{
    QString bg;
    QString cg;
    QString text;
    QString bgm_bg;
    QString bgm_cg;
    QString sound_text;
};

//Colored类型按钮的文件路径
struct Colored{
    QString pic;
};

//T类型按钮的文件路径
struct T{
    QString pic_white_on;
    QString pic_black_on;
    QString pic_white_off;
    QString pic_black_off;
};

//SCButton类型按钮的文件路径
struct SCB{
    QString pic_normal;
    QString pic_hovered;
    QString sound_hovered;
    QString sound_clicked;
};

//SCCheckbox类型按钮的文件路径
struct SCC{
    QString pic_normal_on;
    QString pic_normal_off;
    QString pic_hovered_on;
    QString pic_hovered_off;
    QString sound_hover;
    QString sound_clicked;
};

struct File_of_button{
    struct Colored colored = {};
    struct SCB scb = {};
    struct T t = {};
    struct SCC scc = {};
};

//游戏默认周期
const int Game_Period = 10;

//淡入淡出默认时间
const int fadein_time = 1000;
const int fadeout_time = 1000;

//bg界面默认大小
const QSize bg_size(1280,720);
//cg界面默认大小
const QSize cg_size(1280,720);
//Meguru默认大小
const QSize Meguru_size(140,232);
//对话框的默认大小
const QSize text_size(1280,202);
//洞的默认大小
const QSize hole_size(90,52);

//洞图像的默认偏移量
const int hole_dx = 30;
const int hole_dy = 200;
//Meguru的默认速度
const double Meguru_speed = 3.605;
const double diagonal_speed_factor = 0.7071;

//Meguru移动边界比例
const double Meguru_min_x_ratio = 0.0;
const double Meguru_max_x_ratio = 114.0 / 128.0;
const double Meguru_min_y_ratio = 3.0 / 128.0;
const double Meguru_max_y_ratio = 85.0 / 128.0;

//默认挖掘次数
const int normal_dig_trials = 21;
const int hard_dig_trials = 7;

//宝藏的生成坐标区间
//横坐标区间
const double treasure_min_x = 70.0;
const double treasure_max_x = 1220.0;
//纵坐标区间
const double treasure_min_y = 235.0;
const double treasure_max_y = 705.0;

//获胜半径以及各种提示半径
const double win_boundary = 36.05;
const double very_near_boundary = 72.1;
const double near_boundary = 108.15;
const double medium_boundary = 180.25;
const double far_boundary = 288.4;

//按钮对应的键值
//0空着不用
//刷新按钮
const int refresh_button = 1;

//主界面按钮,包括新手教程按钮,选择关卡按钮,退出游戏按钮,设置按钮,额外内容按钮
const int main_scene_button = 2;
    //新手教程按钮
    const int guide_button = 1;
    //选择关卡按钮
    const int select_button = 2;
    //退出游戏按钮
    const int exit_button = 3;
    //设置按钮
    const int system_button = 4;
    //额外内容按钮
    const int extra_button = 5;

//音乐开关按钮
const int music_button = 3;
//游戏内重试按钮
const int retry_button = 4;

//选择每个关卡的按钮
const int level_button = 5;
    //以下按钮键值同时也是关卡键值
    const int senren_0 = 1;
    const int senren_1 = 2;
    const int sanoba_nene = 3;
    const int sanoba_touko = 4;
    const int cafe_0 = 5;
    const int cafe_1 = 6;
    const int rj = 7;
    const int shinku = 8;
    const int tenshi = 9;
    const int tenshin = 10;

//选关界面的按钮,包括返回之前的画面按钮,返回标题界面按钮
const int select_scene_button = 6;
    //返回之前的画面按钮
    const int return_prev_button = 1;
    //返回标题界面按钮
    const int return_title_button = 2;

//关卡内进入选关界面的按钮
const int select_in_level_button = 7;

//按钮数组容量和常用按钮数量
const int button_group_capacity = 10;
const int button_variant_capacity = 11;
const int main_scene_button_count = 5;//主界面按钮数量
const int level_button_count = 10;//关卡按钮数量
const int select_scene_button_count = 2;//选关界面按钮数量

const int compact_button_height = 30;
const QSize compact_button_layout_slot_size(144,compact_button_height);
const QSize refresh_button_size = compact_button_layout_slot_size;
const QSize main_scene_button_size(280,70);
const QSize music_button_size(32,36);
const QSize retry_button_size(30,30);
const QSize level_button_size(412,132);

const double select_scene_button_asset_width = 161.0;
const double select_scene_button_asset_height = 37.0;
const int select_scene_button_width = static_cast<int>(
    compact_button_height * select_scene_button_asset_width / select_scene_button_asset_height + 0.5
);
const QSize select_scene_navigation_button_size(select_scene_button_width,compact_button_height);

const double select_in_level_button_asset_width = 2080.0;
const double select_in_level_button_asset_height = 510.0;
const int select_in_level_button_width = static_cast<int>(
    compact_button_height * select_in_level_button_asset_width / select_in_level_button_asset_height + 0.5
);
const QSize select_in_level_button_size(select_in_level_button_width,compact_button_height);

//按钮的默认大小,这里用数组保存,下标对应键值
const bool button_size_is_variant_specific[button_group_capacity] = {
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false
};

const QSize button_size[button_group_capacity][button_variant_capacity] = {
    { QSize(0,0) },
    { refresh_button_size },
    { main_scene_button_size },
    { music_button_size },
    { retry_button_size },
    { level_button_size },
    { select_scene_navigation_button_size },
    { select_in_level_button_size }
};
                                  //空白     //刷新        //主界面       //音乐开关    //游戏内重试   //选择每个关卡   //选关界面    //关卡进选关

inline QSize buttonSize(int group,int variant = 0)
{
    if(group < 0 || group >= button_group_capacity
        || variant < 0 || variant >= button_variant_capacity){
        return QSize(0,0);
    }
    const int sizeIndex = button_size_is_variant_specific[group] ? variant : 0;
    return button_size[group][sizeIndex];
}

//按钮类型的键值
//0空着不用
const int colored = 1;//ColoredButton
const int sc = 2;//SCButton
const int t = 3;//TButton
const int scc = 4;//SCCheckbox

//按钮的类型,同样用数组保存
const int button_type[button_group_capacity][button_variant_capacity] =
{
    {0},//0索引空着不用
    {2},//1：刷新按钮
    {0,2,2,2,2,2},//2：主界面按钮
    {4},//3：音乐开关按钮
    {2},//4：游戏内重试按钮
    {0,2,2,2,2,2,2,2,2,2,2},//5：选择每个关卡的按钮
    {0,2,2},//6：选关界面的按钮
    {2}//7：关卡内进入选关界面的按钮
};


//保存按钮文件的路径
const File_of_button file_of_button[button_group_capacity][button_variant_capacity] = {

    // === 索引0：空着不用 ===
    {
        {},//如果只有一个实例的话就只放在第2维的第0个位置,如果有多个,第2维的第0位就空着不放,从1开始放
        {
            //pic
            {   ""  }, //colored

            //pic_normal  //pic_hovered  //sound_hovered   //sound_clicked
            {       ""    ,       ""     ,        ""       ,        ""       }, //scb

            //pic_white_on //pic_black_on  //pic_white_off   //pic_black_off
            {     ""      ,       ""     ,       ""       ,        ""       }, //t

            //pic_normal_on //pic_normal_off //pic_hovered_on //pic_hovered_off //sound_hover //sound_clicked
            {       ""      ,       ""       ,       ""       ,       ""        ,      ""     ,       ""      } //scc
        }
    },

    // === 索引1：刷新按钮 (SCB类型) ===
    {
        {    // refresh_button = 1
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/refresh_button.png",
                "../../Resources/Pictures/Icon-s/Button-s/refresh_button_over.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        }
    },

    // === 索引2：主界面按钮 (SCB类型) ===
    {
        {},  // 子索引0不用
        {    // guide_button = 1 - 新手教程按钮
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/新手教程-0.png",
                "../../Resources/Pictures/Icon-s/Button-s/新手教程-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        },
        {    // select_button = 2 - 选择关卡按钮
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/选择按钮-0.png",
                "../../Resources/Pictures/Icon-s/Button-s/选择按钮-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        },
        {    // exit_button = 3 - 退出游戏按钮
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/退出按钮-0.png",
                "../../Resources/Pictures/Icon-s/Button-s/退出按钮-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        },
        {    // system_button = 4 - 设置按钮
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/系统设置-0.png",
                "../../Resources/Pictures/Icon-s/Button-s/系统设置-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        },
        {    // extra_button = 5 - 额外内容按钮
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/额外按钮-0.png",
                "../../Resources/Pictures/Icon-s/Button-s/额外按钮-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        }
    },

    // === 索引3：音乐开关按钮 (SCC类型) ===
    {
        {    // music_button = 3
            {},
            {},
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/Music_Checkbox_On_Normal.png",
                "../../Resources/Pictures/Icon-s/Button-s/Music_Checkbox_Off_Normal.png",
                "../../Resources/Pictures/Icon-s/Button-s/Music_Checkbox_On_Hover.png",
                "../../Resources/Pictures/Icon-s/Button-s/Music_Checkbox_Off_Hover.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/check_box_chosen.ogg"
            }
        }
    },

    // === 索引4：游戏内重试按钮 (SCB类型) ===
    {
        {    // retry_button = 4
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/重试按钮-0.png",
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/重试按钮-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        }
    },

    // === 索引5：选择关卡按钮 (SCB类型) ===
    {
        {},  // 子索引0不用
        {    // senren_0 = 1
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/教学关卡-0.png",
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/教学关卡-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        },
        {    // senren_1 = 2
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/神社-0.png",
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/神社-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        },
        {    // sanoba_nene = 3
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/宁宁的家-0.png",
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/宁宁的家-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        },
        {    // sanoba_touko = 4
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/黑猫咖啡厅-0.png",
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/黑猫咖啡厅-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        },
        {    // cafe_0 = 5
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/阁楼-0.png",
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/阁楼-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        },
        {    // cafe_1 = 6
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/星光咖啡馆-0.png",
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/星光咖啡馆-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        },
        {    // rj = 7
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/实验室-0.png",
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/实验室-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        },
        {    // shinku = 8
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/管理员室-0.png",
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/管理员室-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        },
        {    // tenshi = 9
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/高层公寓-0.png",
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/高层公寓-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        },
        {    // tenshin = 10
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/家-0.png",
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/关卡入口/家-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        }
    },

    // === 索引6：选关界面按钮 (SCB类型) ===
    {
        {},  // 子索引0不用
        {    // return_prev_button = 1 - 返回之前的画面按钮
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/返回之前的画面-0.png",
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/返回之前的画面-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/return_button_clicked.ogg"
            },
            {},
            {}
        },
        {    // return_title_button = 2 - 返回标题界面按钮
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/返回标题-0.png",
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/返回标题-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        }
    },

    // === 索引7：关卡内选关按钮 (SCB类型) ===
    {
        {    // select_in_level_button = 7
            {},
            {
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/选择关卡.png",
                "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/选择关卡-1.png",
                "../../Resources/Audios/Sound-s/button_hovered.ogg",
                "../../Resources/Audios/Sound-s/push_button_clicked.ogg"
            },
            {},
            {}
        }
    }
};

//按钮音效音量
const double button_volume = 0.5;
//bgm音量
const double bgm_volume = 0.5;
//语音音量
const double voice_volume = 0.8;

//关卡文件路径配置
const File_of_level file_of_level[level_button_count + 1] = {
    // 索引0：空着不用
    {
        "", //bg
        "", //cg
        "", //text
        "", //bgm_bg
        "", //bgm_cg
        ""  //sound_text
    },

    // 索引1：senren_0 - 教学关卡
    {
        "../../Resources/Pictures/Bg-s/Senren_Banka/神社_神社内（刀）A.png",
        "../../Resources/Pictures/Bg-s/Senren_Banka/Mura_cg_0.png",
        "",
        "../../Resources/Audios/BGM-s/Senren_Banka/Angel Note,井ノ原智 - はて？.flac",
        "../../Resources/Audios/BGM-s/Senren_Banka/Angel Note,井ノ原智 - キズナヒトツ Instrument Version.flac",
        "",
    },

    // 索引2：senren_1 - 神社
    {
        "../../Resources/Pictures/Bg-s/Senren_Banka/神社_神社内A.png",
        "../../Resources/Pictures/Bg-s/Senren_Banka/Mura_cg_1.png",
        "",
        "../../Resources/Audios/BGM-s/Senren_Banka/Famishin,Angel Note - 恋ひ恋ふ縁＜Piano Version＞.flac",
        "../../Resources/Audios/BGM-s/Senren_Banka/Famishin,春風まゆき - ふたつの影.mp3",
        ""
    },

    // 索引3：sanoba_nene - 宁宁的家
    {
        "../../Resources/Pictures/Bg-s/Sanoba_Witch/ヒロイン_寧々の部屋B.png",
        "../../Resources/Pictures/Bg-s/Sanoba_Witch/Nene_cg.png",
        "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/对话框/Levels/sanoba_nene.png",
        "../../Resources/Audios/BGM-s/Sanoba_Witch/Famishin - まったり日和.flac",
        "../../Resources/Audios/BGM-s/Sanoba_Witch/Famishin - ありえない.flac",
        "../../Resources/Audios/Voice-s/Nene.flac"
    },

    // 索引4：sanoba_touko - 黑猫咖啡厅
    {
        "../../Resources/Pictures/Bg-s/Sanoba_Witch/喫茶店_店内A.png",
        "../../Resources/Pictures/Bg-s/Sanoba_Witch/Touko_cg.png",
        "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/对话框/Levels/sanoba_touko.png",
        "../../Resources/Audios/BGM-s/Sanoba_Witch/Famishin - Schwarze Katze.flac",
        "../../Resources/Audios/BGM-s/Sanoba_Witch/Angel Note,井ノ原智 - Midday Star instrument Version.flac",
        "../../Resources/Audios/Voice-s/Touko.flac"
    },

    // 索引5：cafe_0 - 阁楼
    {
        "../../Resources/Pictures/Bg-s/Cafe_Stella/喫茶_栞那の部屋C.png",
        "../../Resources/Pictures/Bg-s/Cafe_Stella/Kanna_cg_0.png",
        "",
        "../../Resources/Audios/BGM-s/Cafe_Stella/森脇正敏 - 残滓.flac",
        "../../Resources/Audios/BGM-s/Cafe_Stella/森脇正敏 - ひとすじの光明.flac",
        ""
    },

    // 索引6：cafe_1 - 星光咖啡馆
    {
        "../../Resources/Pictures/Bg-s/Cafe_Stella/喫茶_店内A.png",
        "../../Resources/Pictures/Bg-s/Cafe_Stella/Kanna_cg_1.png",
        "",
        "../../Resources/Audios/BGM-s/Cafe_Stella/森脇正敏 - Casual Everyday.flac",
        "../../Resources/Audios/BGM-s/Cafe_Stella/Famishin,森脇正敏 - おんりいゆう.flac",
        ""
    },

    // 索引7：rj - 实验室
    {
        "../../Resources/Pictures/Bg-s/Riddle_Joker/Mayu_lab.png",
        "../../Resources/Pictures/Bg-s/Riddle_Joker/Mayu.png",
        "",  // text 空着
        "../../Resources/Audios/BGM-s/Riddle_Joker/新井健史 - Common Scenery.flac",
        "../../Resources/Audios/BGM-s/Riddle_Joker/Famishin - 強がるオトナのSecret Labo (Instrumental Version).flac",
        ""
    },

    // 索引8：shinku - 管理员室
    {
        "../../Resources/Pictures/Bg-s/Nikaidou_Shinku/Manager_room.png",
        "../../Resources/Pictures/Bg-s/Nikaidou_Shinku/SHINKU_HD.png",
        "",
        "../../Resources/Audios/BGM-s/Nikaidou_Shinku/忍 - 紅い瞳に映るセカイ(Arrange ver.).flac",
        "../../Resources/Audios/BGM-s/Nikaidou_Shinku/忍 - 紅い瞳に映るセカイ.flac",
        ""
    },

    // 索引9：tenshi - 高层公寓
    {
        "../../Resources/Pictures/Bg-s/Tenshi_Souzou/Kaguya_house.png",
        "../../Resources/Pictures/Bg-s/Tenshi_Souzou/Kaguya.png",
        "",
        "../../Resources/Audios/BGM-s/Tenshi_Souzou/鈴木ルヒカ,伊福部武史,仲村洸祐 - Blazing Sunshine.flac",
        "../../Resources/Audios/BGM-s/Tenshi_Souzou/Famishin,鈴木ルヒカ,伊福部武史 - 惚れて揺れて恋焦がれ (BGM Version).flac",
        ""
    },

    // 索引10：tenshin - 家
    {
        "../../Resources/Pictures/Bg-s/Tenshin_Ranman/主人公の家_リビング_A.png",
        "../../Resources/Pictures/Bg-s/Tenshin_Ranman/Hime_cg.png",
        "",
        "../../Resources/Audios/BGM-s/Tenshin_Ranman/Famishin,Angel Note - Good Morning!.mp3",
        "../../Resources/Audios/BGM-s/Tenshin_Ranman/Famishin,Angel Note - 茶味を武装る;.mp3",
        ""
    }
};

//界面的键值
const int main_scene = 1;
const int select_scene = 2;
const int scene_capacity = 5;

const File_of_scene file_of_scene[scene_capacity] = {
    //索引0：空着不用
    {
        "", //bg
        ""  //bgm
    },

    //索引1：主界面
    {
        "../../Resources/Pictures/Bg-s/main_scene.png",
        "../../Resources/Audios/BGM-s/Sanoba_Witch/Famishin - 恋せよ乙女！(Title Version).flac"
    },

    //索引2：选关界面
    {
        "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/选择关卡界面.png",
        "../../Resources/Audios/BGM-s/Sanoba_Witch/Famishin - 恋せよ乙女！(Title Version).flac"
    }
};

}

#define PATH_OF_PIC_BG_START "../../Resources/Pictures/Bg-s/main_scene.png"
#define PATH_OF_PIC_BG_SELECT "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/选择关卡界面.png"

#define PATH_OF_bgm_bg_start "../../Resources/Audios/BGM-s/Sanoba_Witch/Famishin - 恋せよ乙女！(Title Version).flac"

#define PATH_OF_SOUND_CIALLO "../../Resources/Audios/Ciallo/Ciallo_Meguru_Pure.flac"

#define PATH_OF_SOUND_WIN "../../Resources/Audios/Voice-s/Meguru.flac"

//Meguru
#define PATH_OF_left_0_normal "../../Resources/Pictures/item-s/Meguru-s/m_v0_m.png"
#define PATH_OF_right_0_normal "../../Resources/Pictures/item-s/Meguru-s/m_v0.png"
#define PATH_OF_left_0_excited "../../Resources/Pictures/item-s/Meguru-s/m_v1_m.png"
#define PATH_OF_right_0_excited "../../Resources/Pictures/item-s/Meguru-s/m_v1.png"

#define PATH_OF_left_1_normal "../../Resources/Pictures/item-s/Meguru_with_shovel-s/m_s_v0_m.png"
#define PATH_OF_right_1_normal "../../Resources/Pictures/item-s/Meguru_with_shovel-s/m_s_v0.png"
#define PATH_OF_left_1_excited "../../Resources/Pictures/item-s/Meguru_with_shovel-s/m_s_v1_m.png"
#define PATH_OF_right_1_excited "../../Resources/Pictures/item-s/Meguru_with_shovel-s/m_s_v1.png"
#define PATH_OF_left_1_win "../../Resources/Pictures/item-s/Meguru_with_shovel-s/m_s_v2_m.png"
#define PATH_OF_right_1_win "../../Resources/Pictures/item-s/Meguru_with_shovel-s/m_s_v2.png"

#define PATH_OF_far_away "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/对话框/Meguru/far_away.png"
#define PATH_OF_far "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/对话框/Meguru/far.png"
#define PATH_OF_medium "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/对话框/Meguru/medium.png"
#define PATH_OF_near "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/对话框/Meguru/near.png"
#define PATH_OF_very_near "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/对话框/Meguru/very_near.png"
#define PATH_OF_thats_it "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/对话框/Meguru/thats_it.png"
#define PATH_OF_failed "../../Resources/Pictures/Icon-s/Button-s/非主菜单的按钮/对话框/Meguru/failed.png"

//洞
#define PATH_OF_HOLE "../../Resources/Pictures/item-s/shovel_and_hole/Hole_final.png"

#endif // CONFIG_H
