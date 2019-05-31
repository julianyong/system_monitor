#include "processwindow.h"
#include "ui_processwindow.h"


processwindow::processwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::processwindow)
{
    ui->setupUi(this);
    //整行选中的方式
    ui->ProcessInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
    //多个选中
    ui->ProcessInfo->setSelectionMode(QAbstractItemView::ExtendedSelection);

    //设置默认列宽
    ui->ProcessInfo->setColumnWidth(0,200);
    ui->ProcessInfo->setColumnWidth(1,100);
    ui->ProcessInfo->setColumnWidth(2,100);
    ui->ProcessInfo->setColumnWidth(3,100);
    //隐藏行表头
    ui->ProcessInfo->verticalHeader()->setVisible(false);
    //设置内容不可编辑
    ui->ProcessInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置背景交替颜色
    ui->ProcessInfo->setAlternatingRowColors(true);
    //设置表头背景色
    ui->ProcessInfo->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");


    //创建定时器，设置时间为0.5秒，每0.5秒刷新一遍监视列表界面和列表信息
    timer = new QTimer;
    timer->start(500);
//    connect(timer, SIGNAL(timeout()), this, SLOT(updateProcessInfo()));
//    connect(timer, SIGNAL(timeout()), this, SLOT(refreshProcessInfo()));
    showProcessInfo();
//    sleep(1);
//    updateProcessInfo();
}

processwindow::~processwindow()
{
    delete ui;
}

void processwindow::on_pushButton_refresh_clicked()
{
//    showProcessInfo();
}

void processwindow::on_pushButton_kill_clicked()
{
//    QTableWidgetItem *item = ui->ProcessInfo->currentItem();
//    item->column()
//    QListWidgetItem *item = ui->listWidget_process->currentItem();
//    QString pro = item->text();
//    pro = pro.section("\t", 0, 0);
//    system("kill " + );
//    QMessageBox::warning(this, tr("kill"), QString::fromUtf8("该进程已被杀死!"), QMessageBox::Yes);
//    //刷新进程信息
//    refreshProcessInfo();
}

void processwindow::showProcessInfo()
{
    ui->ProcessInfo->clearContents();               //清空列表，不包括表头
    ui->ProcessInfo->setSortingEnabled(false);      //更改列表时，禁止排序，以防数据显示错误
    getProcessListWithOutCpuUsage(processlist);

    size_t row = processlist.size();
    ui->ProcessInfo->setRowCount(int(row));
    for(size_t i = 0; i < row; i++){

        QTableWidgetItem *processID = new QTableWidgetItem();
        processID->setData(Qt::EditRole, QVariant(processlist[i].pid));
        ui->ProcessInfo->setItem(int(i), 0, processID);
        ui->ProcessInfo->item(int(i),1)->setTextAlignment(Qt::AlignCenter);//内容居中

        QTableWidgetItem *processName = new QTableWidgetItem();
        processName->setText(processlist[i].comm.c_str());
        ui->ProcessInfo->setItem(int(i), 1, processName);

        QTableWidgetItem *taskState = new QTableWidgetItem();
        taskState->setText(processlist[i].task_state.c_str());
        ui->ProcessInfo->setItem(int(i), 2, taskState);
        ui->ProcessInfo->item(int(i),2)->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *rss = new QTableWidgetItem();
  //      rss->setData(Qt::EditRole, QVariant(processlist[i].rss));
        ui->ProcessInfo->setItem(int(i), 3, rss);
        ui->ProcessInfo->item(int(i),3)->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *cpuUsage = new QTableWidgetItem();
        cpuUsage->setData(Qt::EditRole, QVariant(processlist[i].cpuUsage));
        ui->ProcessInfo->setItem(int(i), 4, cpuUsage);
        ui->ProcessInfo->item(int(i),4)->setTextAlignment(Qt::AlignCenter);

        //根据不同的CPU值，设置不同的背景颜色
        if(processlist[i].cpuUsage < cpulevel1){
            ui->ProcessInfo->item(int(i),4)->setBackgroundColor(QColor(255,244,196));
        }
        else if(processlist[i].cpuUsage < cpulevel2){
            ui->ProcessInfo->item(int(i),4)->setBackgroundColor(QColor(249,236,168));
        }
        else if(processlist[i].cpuUsage < cpulevel3){
            ui->ProcessInfo->item(int(i),4)->setBackgroundColor(QColor(255,228,135));
        }
        else {
            ui->ProcessInfo->item(int(i),4)->setBackgroundColor(QColor(255,198,61));
        }


    }
    ui->ProcessInfo->setSortingEnabled(true);       //列表更改完毕，设置为可排序
}
void processwindow::updateProcessInfo()
{
    updateProcessList(processlist);
}
void processwindow::refreshProcessInfo()
{

}
