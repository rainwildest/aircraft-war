#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include <QIcon>
#include <QPainter>

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
    m_Timer.setInterval(GAME_RATE);
}

void MainWindow::playGame()
{
    // 启动定时器
    m_Timer.start();

    // 监听定时器发送的信号
    connect(&m_Timer, &QTimer::timeout, [=](){
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
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // 绘制地图
    painter.drawPixmap(0, m_map.m_map1_posY, m_map.m_map1);
    painter.drawPixmap(0, m_map.m_map2_posY, m_map.m_map2);
}

