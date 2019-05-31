#include "mainwindow.h"
#include "swapinfo.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QStringList>
#include <QCloseEvent>
//关闭程序时保存监视文件watchlist.txt
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    timer = new QTimer(this);
    SysInfo = new sysInfo();
    ui->tabWidget->insertTab(0, SysInfo, tr("系统信息"));
    MemWindow = new memWindow();
    ui->tabWidget->insertTab(1, MemWindow, tr("内存信息"));
    ProcessInfo = new processwindow();
    ui->tabWidget->insertTab(2,ProcessInfo,tr("进程信息"));
    WatchList = new List();
    ui->tabWidget->insertTab(3,WatchList,tr("监视列表"));
    //将窗口页面固定在0号标签页
    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_reboot_clicked()
{
    system("reboot");
}

void MainWindow::on_pushButton_halt_clicked()
{
    system("halt");
}

//关闭主窗口时弹出提示框
void MainWindow::closeEvent(QCloseEvent *event)
{
    switch( QMessageBox::information(this,tr("提示"),tr("你确定退出该软件?"),tr("确定"), tr("取消"),0,1))
        {
        case 0:
            event->accept();
            break;
        case 1:
        default:
            event->ignore();
            break;
        }
}
