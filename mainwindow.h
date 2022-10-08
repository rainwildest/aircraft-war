#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "map.h"
#include "heroplane.h"
#include "enemyplane.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initScence();

    // 启动游戏
    void playGame();

    // 更新所有游戏中元素的坐标
    void updatePosition();

    // 绘制到屏幕中
    void paintEvent(QPaintEvent *);

    // 重定义鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);

    // 敌机出场
    void enemyToScene();

public:
    // 地图对象
    Map m_map;

    // 飞机对象
    HeroPlane m_hero;

    // 定时器
    QTimer m_timer;

    // 敌机数组
    EnemyPlane m_enemys[ENEMY_NUM];

    //敌机出场间隔记录
    int m_recorder;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
