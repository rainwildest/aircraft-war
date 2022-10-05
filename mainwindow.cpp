#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initScence();
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
}

