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

/**
 * @brief 子弹配置数据
 */
#define BULLET_PATH ":/resource/sprites/bullet1.png"
#define BULLET_SPEED 5  // 子弹移动速度
#define BULLET_NUM 30   //弹匣中子弹总数
#define BULLET_INTERVAL 20  // 发射子弹时间间隔

/**
 * @brief 敌机配置数据
 */
#define ENEMY_PATH ":/resource/sprites/enemy1.png"
#define ENEMY_SPEED 5       // 敌机移动速度
#define ENEMY_NUM 20        // 敌机总数量
#define ENEMY_INTERVAL 30   // 敌机出场时间间隔

#endif // CONFIG_H
