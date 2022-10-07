#ifndef CONFIG_H
#define CONFIG_H

/**
 * @brief 游戏配置数据
 */
#define GAME_WIDTH  480 // 宽度
#define GAME_HEIGHT 760 // 高度
#define GAME_TITLE "飞机大战 v1.0" //标题
#define GAME_ICON ":/resource/sprites/favicon.ico"
#define GAME_RATE 10 // 定时器刷新时间间隔（单位毫秒）

/**
 * @brief 地图配置数据
 */
#define MAP_PATH ":/resource/images/background.png"
#define MAP_SCROLL_SPEED 2;

/**
 * @brief 飞机配置数据
 */
#define HERO_PATH ":/resource/sprites/hero2.png"

#endif // CONFIG_H
