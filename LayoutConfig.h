#ifndef LAYOUTCONFIG_H
#define LAYOUTCONFIG_H

namespace LayoutConfig {

struct PositionRatio {
    double x;
    double y;
};

const PositionRatio start_refresh_button_pos = {1.0 / 128.0, 1.0 / 128.0};
const PositionRatio start_music_button_pos = {1.0 / 8.0, 1.0 / 256.0};

const PositionRatio main_scene_button_pos[6] = {
    {0.0, 0.0},
    {6.7 / 9.0, 1.9 / 5.0},
    {6.7 / 9.0, 2.5 / 5.0},
    {6.7 / 9.0, 4.3 / 5.0},
    {6.7 / 9.0, 3.7 / 5.0},
    {6.7 / 9.0, 3.1 / 5.0}
};

const PositionRatio select_refresh_button_pos = {111.0 / 128.0, 3.5 / 128.0};
const PositionRatio select_music_button_pos = {106.0 / 128.0, 3.25 / 128.0};
const PositionRatio select_return_title_button_pos = {96.0 / 128.0, 120.0 / 128.0};
const PositionRatio select_return_prev_button_pos = {111.0 / 128.0, 120.0 / 128.0};

const PositionRatio level_return_title_button_pos = {95.0 / 128.0, 3.5 / 128.0};
const PositionRatio level_select_button_pos = {80.0 / 128.0, 3.5 / 128.0};
const PositionRatio level_refresh_button_pos = {111.0 / 128.0, 3.5 / 128.0};
const PositionRatio level_music_button_pos = {76.0 / 128.0, 3.25 / 128.0};
const PositionRatio level_retry_button_pos = {71.0 / 128.0, 3.25 / 128.0};
const PositionRatio level_cg_retry_button_pos = {71.0 / 128.0, 3.5 / 128.0};

const int level_button_page_size = 10;
const int level_button_grid_columns = 3;
const PositionRatio level_button_grid_origin = {1.0 / 128.0, 20.0 / 128.0};
const PositionRatio level_button_grid_step = {42.5 / 128.0, 25.0 / 128.0};

inline PositionRatio levelButtonPosition(int levelIndex)
{
    const int indexInPage = (levelIndex - 1) % level_button_page_size;
    const int column = indexInPage % level_button_grid_columns;
    const int row = indexInPage / level_button_grid_columns;
    return {
        level_button_grid_origin.x + column * level_button_grid_step.x,
        level_button_grid_origin.y + row * level_button_grid_step.y
    };
}

}

#endif // LAYOUTCONFIG_H
