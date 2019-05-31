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
    ui->ProcessInfo->setColumnWidth(0,100);
    ui->ProcessInfo->setColumnWidth(1,200);
    ui->ProcessInfo->setColumnWidth(2,100);
    ui->ProcessInfo->setColumnWidth(3,100);
    ui->ProcessInfo->setColumnWidth(4,100);
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
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateProcessInfo()));
//    connect(timer, SIGNAL(timeout()), this, SLOT(refreshProcessInfo()));
    showProcessInfo();
    sleep(1);
    updateProcessInfo();
}

processwindow::~processwindow()
{
    delete ui;
}



void processwindow::on_pushButton_kill_clicked()
{
    ui->ProcessInfo->setSortingEnabled(false);
    int row = ui->ProcessInfo->currentIndex().row();
    QString pid = ui->ProcessInfo->item(row,0)->text();
    pid = pid.section("\t", 0, 0);
    system("kill " + pid.toLatin1());
    QMessageBox::warning(this, tr("kill"), QString::fromUtf8("该进程已被杀死!"), QMessageBox::Yes);
    //刷新进程信息
    refreshProcessInfo();
}

void processwindow::showProcessInfo()
{
    vector<struct Process> nowProcessList;
    getProcessListWithOutCpuUsage(nowProcessList);
    processlist = nowProcessList;

    ui->ProcessInfo->clearContents();               //清空列表，不包括表头
    ui->ProcessInfo->setSortingEnabled(false);      //更改列表时，禁止排序，以防数据显示错误

    int totalProNum = 0, number_of_run = 0, number_of_sleep = 0, number_of_zombie = 0;

    size_t row = processlist.size();
    totalProNum = row;
    ui->ProcessInfo->setRowCount(int(row));
    for(size_t i = 0; i < row; i++)
    {

        QTableWidgetItem *processID = new QTableWidgetItem();
        processID->setData(Qt::EditRole, QVariant(processlist[i].pid));
        ui->ProcessInfo->setItem(int(i), 0, processID);

        QTableWidgetItem *processName = new QTableWidgetItem();
        processName->setText(processlist[i].comm.c_str());
        ui->ProcessInfo->setItem(int(i), 1, processName);
        ui->ProcessInfo->item(int(i),1)->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *taskState = new QTableWidgetItem();
        taskState->setText(processlist[i].task_state.c_str());
        ui->ProcessInfo->setItem(int(i), 2, taskState);
        ui->ProcessInfo->item(int(i),2)->setTextAlignment(Qt::AlignCenter);

        if(*processlist[i].task_state.c_str() == 'S')
        {
            number_of_sleep++;
        }
        if(*processlist[i].task_state.c_str() == 'R')
        {
            number_of_run++;
        }
        if(*processlist[i].task_state.c_str() == 'Z')
        {
            number_of_zombie++;
        }

        QTableWidgetItem *processRss = new QTableWidgetItem();
        QString process_Rss;
        process_Rss = QString::number (processlist[i].rss,10 );
        processRss->setText(process_Rss);
        ui->ProcessInfo->setItem(int(i), 3, processRss);
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
    QString temp;
    temp = QString::number(totalProNum, 10);
    ui->label_pNum->setText(temp);
    temp = QString::number(number_of_run, 10);
    ui->label_pRun->setText(temp);
    temp = QString::number(number_of_sleep, 10);
    ui->label_pSleep->setText(temp);
    temp = QString::number(number_of_zombie, 10);
    ui->label_pZombie->setText(temp);
    ui->ProcessInfo->setSortingEnabled(true);       //列表更改完毕，设置为可排序
}
void processwindow::updateProcessInfo()
{
    updateProcessList(processlist);
    ui->ProcessInfo->setSortingEnabled(false);          //更改列表时，禁止排序，以防数据显示错误
    int totalProNum = 0, number_of_run = 0, number_of_sleep = 0, number_of_zombie = 0;
    size_t count = processlist.size();
    //对界面的表格中的每一行依次进行更新
    int row = ui->ProcessInfo->rowCount();
    totalProNum = row;
    for(int i = 0;i < row;i++)
    {
        bool ok;
        int pid = ui->ProcessInfo->item(i,0)->text().toInt(&ok,10);     //从界面的列表中获取进程的ID
        for(int j = 0;j < count;j++)
        {
            if(processlist[j].pid == pid)
            {
                QTableWidgetItem *taskState = new QTableWidgetItem();
                taskState->setText(processlist[j].task_state.c_str());
                ui->ProcessInfo->setItem(int(j), 2, taskState);
                ui->ProcessInfo->item(int(j),2)->setTextAlignment(Qt::AlignCenter);

                if(*processlist[j].task_state.c_str() == 'S')
                {
                    number_of_sleep++;
                }
                if(*processlist[j].task_state.c_str() == 'R')
                {
                    number_of_run++;
                }
                if(*processlist[j].task_state.c_str() == 'Z')
                {
                    number_of_zombie++;
                }

                QTableWidgetItem *processRss = new QTableWidgetItem();
                QString process_Rss;
                process_Rss = QString::number (processlist[j].rss,10 );
                processRss->setText(process_Rss);
                ui->ProcessInfo->setItem(int(j), 3, processRss);
                ui->ProcessInfo->item(int(j),3)->setTextAlignment(Qt::AlignCenter);

                QTableWidgetItem *cpuUsage = new QTableWidgetItem();
                cpuUsage->setData(Qt::EditRole, QVariant(processlist[j].cpuUsage));
                ui->ProcessInfo->setItem(int(j), 4, cpuUsage);
                ui->ProcessInfo->item(int(j),4)->setTextAlignment(Qt::AlignCenter);

                //根据不同的CPU值，设置不同的背景颜色
                if(processlist[j].cpuUsage < cpulevel1){
                    ui->ProcessInfo->item(int(j),4)->setBackgroundColor(QColor(255,244,196));
                }
                else if(processlist[j].cpuUsage < cpulevel2){
                    ui->ProcessInfo->item(int(j),4)->setBackgroundColor(QColor(249,236,168));
                }
                else if(processlist[j].cpuUsage < cpulevel3){
                    ui->ProcessInfo->item(int(j),4)->setBackgroundColor(QColor(255,228,135));
                }
                else {
                    ui->ProcessInfo->item(int(j),4)->setBackgroundColor(QColor(255,198,61));
                }
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
    ui->ProcessInfo->setSortingEnabled(true);           //列表更改完毕，设置为可排序
    }
}

void processwindow::refreshProcessInfo()
{
    showProcessInfo();
    sleep(1);
    updateProcessInfo();
}

void processwindow::on_pushButton_SpareOneCore_clicked()
{
    vector<int> newOperateRecord;
    spareOneCore(newOperateRecord);
    operateRecord = newOperateRecord;
}

void processwindow::on_pushButton_UndoSpare_clicked()
{
    undoSpare(operateRecord);
}
