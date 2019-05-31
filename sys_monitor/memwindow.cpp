#include "memwindow.h"
#include "ui_memwindow.h"

memWindow::memWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::memWindow)
{
    ui->setupUi(this);
    timer = new QTimer;
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(showMemWindow()));
}

memWindow::~memWindow()
{
    delete ui;
}

void memWindow::showMemWindow(){
    QString memUsed;
    QString memTotal;
    QString memFree;
    QString swapUsed;
    QString swapTotal;
    QString swapFree;
//        int MemUsed, MemTotal, MemFree, SwapUsed, SwapTotal, SwapFree;

    MemInfo memInfo;
    SwapInfo swapInfo;
    getMemInfo(memInfo);
    getSwapInfo(swapInfo);

    memInfo.MemTotal = memInfo.MemTotal/1024;
    memInfo.MemFree = memInfo.MemFree/1024;
    memInfo.MemAvailable = memInfo.MemAvailable/1024;
    unsigned long MemUsed;
    MemUsed = memInfo.MemTotal - memInfo.MemFree;
    swapInfo.SwapTotal = swapInfo.SwapTotal/1024;
    swapInfo.SwapFree = swapInfo.SwapFree/1024;
    unsigned long SwapUsed;
    SwapUsed = swapInfo.SwapTotal - swapInfo.SwapFree;

    memUsed = QString::number(MemUsed, 10);        //将int转化为QString
    swapUsed = QString::number(SwapUsed, 10);
    memFree = QString::number(memInfo.MemFree, 10);
    memTotal = QString::number(memInfo.MemTotal, 10);
    swapFree = QString::number(swapInfo.SwapFree, 10);
    swapTotal = QString::number(swapInfo.SwapTotal, 10);

    ui->label_RAM_USED->setText(memUsed+" MB");
    ui->label_RAM_LEFT->setText(memFree+" MB");
    ui->label_RAM_TOTAL->setText(memTotal+" MB");
    ui->label_SWAP_USED->setText(swapUsed+" MB");
    ui->label_SWAP_LEFT->setText(swapFree+" MB");
    ui->label_SWAP_TOTAL->setText(swapTotal+" MB");

    ui->progressBar_RAM->setValue(MemUsed*100/memInfo.MemTotal);
    ui->progressBar_SWAP->setValue(SwapUsed*100/swapInfo.SwapTotal);
//    tempFile.close(); //关闭内存信息文件

    double cpuUsage;
//      调用getCpuUsage得到CPU使用率
//      stat文件中cpu行七个参数的含义：
//      user 从系统启动开始累计到当前时刻，用户态的CPU时间（单位：jiffies） ，不包含 nice值为负进程。1jiffies=0.01秒
//      nice 从系统启动开始累计到当前时刻，nice值为负的进程所占用的CPU时间（单位：jiffies）
//      system 从系统启动开始累计到当前时刻，核心时间（单位：jiffies）
//      idle 从系统启动开始累计到当前时刻，除硬盘IO等待时间以外其它等待时间（单位：jiffies）
//      iowait 从系统启动开始累计到当前时刻，硬盘IO等待时间（单位：jiffies） ，
//      irq 从系统启动开始累计到当前时刻，硬中断时间（单位：jiffies）
//      softirq 从系统启动开始累计到当前时刻，软中断时间（单位：jiffies）
//      利用如下公式计算CPU利用率：
//      cpuUsage =100* (total-idle)/total

    getCpuUsage(cpuUsage);
//        printf("%lf\n", cpuUsage);
    ui->progressBar_CPU->setValue(cpuUsage*100);//转化为百分数
}

