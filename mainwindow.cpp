#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include <QIcon>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <ctime>
#include <QFontDatabase>

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
//    int fontId = QFontDatabase::addApplicationFont(":/typeface/resource/font/Megatron.ttf");
//    if(fontId >= 0){

//        QString mysh = QFontDatabase::applicationFontFamilies(fontId).at(0);
//        qDebug() << mysh;
//        QFont font(mysh);
//        QApplication::setFont(font);
//    }
//    else{

//        qDebug() << "fontId:" << fontId << " 字体加载失败";
//    }

    // 设置窗口固定尺寸
    setFixedSize(GAME_WIDTH, GAME_HEIGHT);

    // 设置标题
    setWindowTitle(GAME_TITLE);

    // 加载图标
    setWindowIcon(QIcon(GAME_ICON));

    // 定时器初始化
    m_timer.setInterval(GAME_RATE);

    srand((unsigned int)time(NULL));

    QFont font = ui->fraction_label->font();

    int fontID = QFontDatabase::addApplicationFont(":/typeface/resource/font/Momo.ttf");
    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(fontID);

    if(!loadedFontFamilies.empty()) {
        font = loadedFontFamilies.at(0);
        font.setFamily(loadedFontFamilies.at(0));
        ui->fraction_label->setFont(font);
    }

    ui->reload_btn->setVisible(false);
    ui->continue_btn->setVisible(false);
    ui->over_btn->setVisible(false);

    connect(ui->reload_btn, &QPushButton::clicked, [=]() {
        heavyLoad();
    });
}

void MainWindow::playGame()
{
    // 启动定时器
    m_timer.start();

    // 监听定时器发送的信号
    connect(&m_timer, &QTimer::timeout, [=]() {

        // 敌机出场
        enemyToScene();

        // 更新游戏中元素的坐标
        updatePosition();

        //碰撞检测
        collisionDetection();

        // 绘制到屏幕中
        update();
    });
}

void MainWindow::updatePosition()
{
    // 更新地图的坐标
    m_map.mapPosition();

    if(!gameOver) {
        m_hero.updateInfo();

        // 发射子弹
        m_hero.shoot();
    }

    if(m_heroBomb.m_heroFree == false) {
      bool status =  m_heroBomb.updateHeroInfo();

      // 播放完爆炸效果就停止
      if(status) {
          stopTimer = true;
          m_timer.stop();

          // 显示重新按钮
          ui->reload_btn->setVisible(true);
      }
    }

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

    // 计算爆炸的播放的图片
    for(int i = 0; i < 4; i++) {
        if(m_bombs[i].m_free == false) {
            m_bombs[i].updateInfo();
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // 绘制地图
    painter.drawPixmap(0, m_map.m_map1_posY, m_map.m_map1);
    painter.drawPixmap(0, m_map.m_map2_posY, m_map.m_map2);

    if(!gameOver) {
        // 绘制英雄飞机
        painter.drawPixmap(m_hero.m_x, m_hero.m_y, m_hero.m_plane);
        painter.drawRect(m_hero.m_rect);
    } else {
        // 绘制英雄飞机爆炸
        if(!stopTimer) {
            painter.drawPixmap(m_heroBomb.m_x, m_heroBomb.m_y, m_heroBomb.m_pixHero[m_heroBomb.m_heroIndex]);
        }
    }

    // 绘制子弹
    for(int i = 0; i < BULLET_NUM; i++) {
        if(m_hero.m_bullets[i].m_free == false) {
            painter.drawPixmap(m_hero.m_bullets[i].m_x, m_hero.m_bullets[i].m_y, m_hero.m_bullets[i].m_bullet);
        }
    }

    // 绘制敌机
    for(int i = 0; i < ENEMY_NUM; i++) {
        if(m_enemys[i].m_free == false) {
            painter.drawPixmap(m_enemys[i].m_x, m_enemys[i].m_y, m_enemys[i].m_enemy);
            painter.drawRect(m_enemys[i].m_rect);
        }
    }

    // 绘制爆炸
    for(int i = 0; i <= BOMB_NUM; i++) {
        if(m_bombs[i].m_free == false) {
            painter.drawPixmap(m_bombs[i].m_x, m_bombs[i].m_y, m_bombs[i].m_pixArr[m_bombs[i].m_index]);
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent * event)
{
    if(gameOver) return;

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

    if(y >= GAME_HEIGHT - m_hero.m_rect.height() - 32) {
        y = GAME_HEIGHT - m_hero.m_rect.height() - 32;
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
            m_enemys[i].m_x = rand() % (GAME_WIDTH - m_enemys[i].m_rect.width() - 30);
            m_enemys[i].m_y = -m_enemys[i].m_rect.height();

            break;
        }
    }
}

void MainWindow::collisionDetection()
{
    if(gameOver) return;

    // 遍历所有非空闲的敌机
    for(int i = 0; i < ENEMY_NUM; i++) {
        // 空闲敌机 执行下一次循环
        if(m_enemys[i].m_free) continue;


        if(m_hero.m_rect.intersects(m_enemys[i].m_rect)) {
            gameOver = true;

            m_heroBomb.m_heroFree = false;

            m_heroBomb.m_x = m_hero.m_x;
            m_heroBomb.m_y = m_hero.m_y;

//            m_timer.stop();
            return;
        }



        // 遍历所有非空闲的子弹
        for(int j = 0; j < BULLET_NUM; j++) {
            // 空闲子弹 执行下一次循环
            if(m_hero.m_bullets[j].m_free) continue;

            // 子弹和敌机相交 发生碰撞
            if(m_enemys[i].m_rect.intersects(m_hero.m_bullets[j].m_rect)) {
                m_enemys[i].m_free = true;
                m_hero.m_bullets[j].m_free = true;

                // 播放爆炸效果
                for(int k = 0; k < 4; k++) {
                    if(m_bombs[k].m_free) {
                        // 空闲的爆炸
                        m_bombs[k].m_free = false;

                        // 更新爆炸坐标
                        m_bombs[k].m_x = m_enemys[i].m_x;
                        m_bombs[k].m_y = m_enemys[i].m_y;

                        fraction += 10;
                        ui->fraction_label->setText(QString::number(fraction));

                        break;
                    }
                }
            }
        }
    }
}

void MainWindow::heavyLoad()
{
    gameOver = false;
    stopTimer = false;

    ui->reload_btn->setVisible(false);
    ui->fraction_label->setText("0");

    // 全部设为空闲敌机
    for(int i = 0 ; i < ENEMY_NUM; i++) {
        m_enemys[i].m_free = true;
    }

    m_hero.initialization();

    m_timer.start();
}

