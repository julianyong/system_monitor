#include "mainwindow.h"
#include "processList.h"
#include "swapinfo.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include <QCloseEvent>
//关闭程序时保存监视文件watchlist.txt
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
//    ProcessInfo = new List();
//    ui->tabWidget->insertTab(2,ProcessInfo,tr("进程信息"));
    WatchList = new List();
    ui->tabWidget->insertTab(3,WatchList,tr("监视列表"));
    //将窗口页面固定在0号标签页
    ui->tabWidget->setCurrentIndex(0);
    on_tabWidget_currentChanged(0);
    QWidget::connect( timer, SIGNAL( timeout() ), this, SLOT( timer_update_currentTabInfo() ) );//ui控件-事件响应
    QWidget::connect( ui->tabWidget, SIGNAL( currentChanged() ),
                      this, SLOT( on_tabWidget_currentChanged() ) );
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    index = ui->tabWidget->currentIndex();
    show_tabWidgetInfo(index); //显示标签页中的内容
    return ;
}

void MainWindow::timer_update_currentTabInfo()
{
    int index = ui->tabWidget->currentIndex();
    //定时器只刷新内存信息页面，用于进度条动态显示
    if (index == 1)
    {
        show_tabWidgetInfo(index);
    }
}

void MainWindow::on_pushButton_kill_clicked()
{
        QListWidgetItem *item = ui->ProcessInfoList->currentItem();
        QString pro = item->text();
        pro = pro.section("\t", 0, 0);
        system("kill " + pro.toLatin1());
        QMessageBox::warning(this, tr("kill"), QString::fromUtf8("该进程已被杀死!"), QMessageBox::Yes);
        //回到进程信息tab表
        show_tabWidgetInfo(2);
}

void MainWindow::on_pushButton_refresh_clicked()
{
     show_tabWidgetInfo(2);
}

void MainWindow::on_pushButton_reboot_clicked()
{
    system("reboot");
}

void MainWindow::on_pushButton_halt_clicked()
{
    system("halt");
}

void MainWindow::show_tabWidgetInfo(int index)
{
    QString tempStr;    //读取文件信息字符串
    QFile tempFile;     //用于打开系统文件
    int pos;            //读取文件的位置

    //系统信息标签页
    if (index == 0)
    {
        tempFile.setFileName("/proc/cpuinfo");
        //打开CPU信息文件
        if ( !tempFile.open(QIODevice::ReadOnly) )
        {
            //如果CPU信息文件打开失败弹出消息框提示：The cpuinfo file can not open!
            QMessageBox::warning(this, tr("warning"), tr("The cpuinfo file can not open!"), QMessageBox::Yes);
            return;
        }
        //循环读取文件内容，查找需要的信息
        while (1)
        {
            tempStr = tempFile.readLine();
            //QMessageBox::warning(this, tr("msg"), tempStr, QMessageBox::Yes);
            if(tempStr==NULL)
            {
                //文件读完，跳出
                break;
            }
            if (pos = tempStr.indexOf("model name"), pos != -1)
            {
                pos += 13; //跳过前面的"model name："所占用的字符
                QString *cpu_name = new QString( tempStr.mid(pos, tempStr.length()-13) );
                ui->label_CPUname->setText(*cpu_name);
            }
            if (pos = tempStr.indexOf("vendor_id"), pos != -1)
            {
                pos += 12; //跳过前面的"vendor_id："所占用的字符
                QString *cpu_type = new QString( tempStr.mid(pos, tempStr.length()-12) );
                ui->label_CPUtype->setText(*cpu_type);
            }
            if (pos = tempStr.indexOf("cpu MHz"), pos != -1)
            {
                pos += 11; //跳过前面的"cpu MHz："所占用的字符
                QString *cpu_frq = new QString( tempStr.mid(pos, tempStr.length()-11) );
                double cpufrq = cpu_frq->toDouble(); //4核CPU
                cpu_frq->setNum(cpufrq*4);
                ui->label_CPUfrequency->setText(*cpu_frq + " HZ");
            }
            if (pos = tempStr.indexOf("cache size"), pos!=-1)
            {
                pos += 13; //跳过前面的"cache size："所占用的字符
                QString *cache_size = new QString( tempStr.mid(pos, tempStr.length()-16) );
                int cachesize = cache_size->toInt(); //4核CPU
                cache_size->setNum(cachesize*4);
                ui->label_Cachecapacity->setText(*cache_size + " KB");
            }
        }
        tempFile.close(); //关闭CPU信息文件
        //打开操作系统信息文件
        tempFile.setFileName("/proc/version");
        if ( !tempFile.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(this, tr("warning"), tr("The version file can not open!"), QMessageBox::Yes);
            return ;
        }
        tempStr = tempFile.readLine();
        pos = tempStr.indexOf("version");
        QString *os_version = new QString( tempStr.mid(0, pos-1) );
        ui->label_OStype->setText(*os_version);
        int pos1 = tempStr.indexOf("(");
        QString *os_type = new QString( tempStr.mid(pos, pos1-pos-1) );
        ui->label_OSversion->setText(*os_type);
        pos = tempStr.indexOf("gcc version");
        pos1 = tempStr.indexOf("#");
        QString *gcc_info = new QString( tempStr.mid(pos+12, pos1-pos-14) );
        ui->label_GCCversion->setText(*gcc_info);
        tempFile.close(); //关闭操作系统信息文件
    }

    //内存信息标签页
    if (index == 1)
    {

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
        tempFile.close(); //关闭内存信息文件

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

    //进程信息标签页
    if (index == 2)
    {
        ui->ProcessInfoList->clear();
        QDir qd("/proc");
        QStringList qsList = qd.entryList();
        QString qs = qsList.join("\n");
        QString id_of_pro;
        bool ok;
        int find_start = 3;
        int a, b;
        int nProPid; //进程PID
        int number_of_sleep = 0, number_of_run = 0, number_of_zombie = 0;
        int totalProNum = 0; //进程总数
        QString proName; //进程名
        QString proState; //进程状态
        QString proPri; //进程优先级
        QString proMem; //进程占用内存
        QListWidgetItem *title = new QListWidgetItem("PID\t" + QString::fromUtf8("名称") + "\t\t" +
                                                     QString::fromUtf8("状态") + "\t" +
                                                     QString::fromUtf8("优先级") + "\t" +
                                                     QString::fromUtf8("占用内存") + "\t" +
                                                     QString::fromUtf8("占用CPU"), ui->ProcessInfoList);

        //循环读取进程
        while (1)
        {
            //获取进程PID
            a = qs.indexOf("\n", find_start);
            b = qs.indexOf("\n", a+1);
            find_start = b;
            id_of_pro = qs.mid(a+1, b-a-1);
            totalProNum++;
            nProPid = id_of_pro.toInt(&ok, 10);
            if(!ok)
            {
                break;
            }

            //打开PID所对应的进程状态文件
            tempFile.setFileName("/proc/" + id_of_pro + "/stat");
            if ( !tempFile.open(QIODevice::ReadOnly) )
            {
                QMessageBox::warning(this, tr("warning"), tr("The pid stat file can not open!"), QMessageBox::Yes);
                return;
            }
            tempStr = tempFile.readLine();
            if (tempStr.length() == 0)
            {
                break;
            }
            a = tempStr.indexOf("(");
            b = tempStr.indexOf(")");
            proName = tempStr.mid(a+1, b-a-1);
            proName.trimmed(); //删除两端的空格
            proState = tempStr.section(" ", 2, 2);
            proPri = tempStr.section(" ", 17, 17);
            proMem = tempStr.section(" ", 22, 22);
            switch ( proState.at(0).toLatin1() )
            {
                case 'S':   number_of_sleep++; break; //Sleep
                case 'R':   number_of_run++; break; //Running
                case 'Z':   number_of_zombie++; break; //Zombie
                default :   break;
            }
            if (proName.length() >= 12)
            {
                QListWidgetItem *item = new QListWidgetItem(id_of_pro + "\t" +
                                                            proName + "\t" +
                                                            proState + "\t" +
                                                            proPri + "\t" +
                                                            proMem, ui->ProcessInfoList);
            }
            else
            {
               QListWidgetItem *item = new QListWidgetItem(id_of_pro + "\t" +
                                                           proName + "\t\t" +
                                                           proState + "\t" +
                                                           proPri + "\t" +
                                                           proMem, ui->ProcessInfoList);
            }
        }
        QString temp;
        temp = QString::number(totalProNum, 10);
        ui->label_pNum->setText(temp);
        temp = QString::number(number_of_run, 10);
        ui->label_pRun->setText(temp);
        temp = QString::number(number_of_sleep, 10);
        ui->label_pSleep->setText(temp);
        temp = QString::number(number_of_zombie, 10);
        ui->label_pZombie->setText(temp);
        tempFile.close(); //关闭该PID进程的状态文件
    }

    return;
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
