#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include <QIcon>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <ctime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initScence();

    playGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initScence()
{
    // 设置窗口固定尺寸
    setFixedSize(GAME_WIDTH, GAME_HEIGHT);

    // 设置标题
    setWindowTitle(GAME_TITLE);

    // 加载图标
    setWindowIcon(QIcon(GAME_ICON));

    // 定时器初始化
    m_timer.setInterval(GAME_RATE);


    // 敌机出场时间间隔 初始化
    m_recorder = 0;

    srand((unsigned int)time(NULL));
}

void MainWindow::playGame()
{
    // 启动定时器
    m_timer.start();

    // 监听定时器发送的信号
    connect(&m_timer, &QTimer::timeout, [=](){
        // 敌机出场
        enemyToScene();

        // 更新游戏中元素的坐标
        updatePosition();

        // 绘制到屏幕中
        update();
    });
}

void MainWindow::updatePosition()
{
    // 更新地图的坐标
    m_map.mapPosition();


    // 发射子弹
    m_hero.shoot();

    // 计算所有非空闲子弹的当前坐标
    for(int i = 0; i < BULLET_NUM; i++) {
        // 非空闲子弹，计算发射位置
        if(m_hero.m_bullets[i].m_free == false) {
            m_hero.m_bullets[i].updatePosition();
        }
    }

    // 敌机出场
    for(int i = 0; i < ENEMY_NUM; i++) {
        if(m_enemys[i].m_free == false) {
            m_enemys[i].updatePosition();
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // 绘制地图
    painter.drawPixmap(0, m_map.m_map1_posY, m_map.m_map1);
    painter.drawPixmap(0, m_map.m_map2_posY, m_map.m_map2);

    // 绘制英雄飞机
    painter.drawPixmap(m_hero.m_x, m_hero.m_y, m_hero.m_plane);

    // 绘制子弹
    for(int i = 0; i < BULLET_NUM; i++) {
        if(m_hero.m_bullets[i].m_free == false) {
            painter.drawPixmap(m_hero.m_bullets[i].m_x, m_hero.m_bullets[i].m_y, m_hero.m_bullets[i].m_bullet);
        }
    }

    // 绘制敌机
    for(int i = 0; i < ENEMY_NUM; i++) {
        if(m_enemys[i].m_free == false) {
//            m_enemys[i].updatePosition();
            painter.drawPixmap(m_enemys[i].m_x, m_enemys[i].m_y, m_enemys[i].m_enemy);
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent * event)
{
    QPointF point = event->position();

    int x = point.x() - m_hero.m_rect.width() * 0.5;
    int y = point.y() - m_hero.m_rect.height() * 0.5;

    // 边界检测
    if(x <= 0) {
        x = 0;
    }

    if(x >= GAME_WIDTH - m_hero.m_rect.width()) {
        x = GAME_WIDTH - m_hero.m_rect.width();
    }

    if(y <= 0) {
        y = 0;
    }

    if(y >= GAME_HEIGHT - m_hero.m_rect.height()) {
        y = GAME_HEIGHT - m_hero.m_rect.height();
    }

    m_hero.setPosition(x, y);
}

void MainWindow::enemyToScene()
{
    m_recorder++;

    // 未达到出场间隔
    if(m_recorder < ENEMY_INTERVAL) return;

    m_recorder = 0;

    for(int i = 0 ; i < ENEMY_NUM; i++) {
        //是空闲敌机
        if(m_enemys[i].m_free) {
            m_enemys[i].m_free = false;

            // 坐标
            m_enemys[i].m_x = rand() % (GAME_WIDTH - m_enemys[i].m_rect.width());
            m_enemys[i].m_y = -m_enemys[i].m_rect.height();

            break;
        }
    }
}

