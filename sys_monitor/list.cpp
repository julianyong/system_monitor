#include "list.h"
#include "ui_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "email.h"
#include <unistd.h>
/*
string name(int i){
    switch (i) {
    case 0:return "aaa";
    case 1:return "aaa";
    case 2:return "abc";
    case 3:return "bbb";
    case 4:return "bcd";
    case 5:return "fuc";
    case 6:return "i";
    case 7:return "have";
    case 8:return "an";
    case 9:return "apple";
    default:return "error";
    }
}


string name1(int i){
    char ch = i + 'A';
    string res;
    res.push_back(ch);
    return res;
}

int addProcess(pid_t pid,vector<Process>&watchList)
{
    Process proc;
    proc.pid=pid;
    proc.comm = name1(pid + rand()%15);
    watchList.push_back(proc);
    return 0;
}


int saveWatchList(vector<Process>watchList)
{
    FILE * file;
    file=fopen("/home/yue/Desktop/QT/myTestList_1/watchlist.txt","w");
    for(size_t i = 0; i < watchList.size(); i++)
    {
        fprintf(file,"%d\n",watchList[i].pid);
    }
    fclose(file);
    return 0;
}

int openWatchList(vector<Process> &watchList)
{
    FILE * file;
    file=fopen("/home/yue/Desktop/QT/myTestList_1/watchlist.txt","r");
    if(file == NULL){
        printf("Open file failed!\n");
    }
    int pid=0;
    watchList.clear();
    srand(time(0));
    while(fscanf(file,"%d\n",&pid)!=EOF)
    {
        Process proc;
        proc.pid=pid;
        proc.comm = name(pid);
        proc.task_state = name1(pid + rand()%15);
        proc.cpuUsage=rand()%100+rand()%10/10.0;
        proc.memoryUsage=rand()%100+rand()%10/10.0;
        watchList.push_back(proc);
    }

    fclose(file);
    return 0;
}


int updateWatchList(vector<Process> &watchList,vector<Process> &emailList)
{

    for(int i = 0 ;i < watchList.size();i++){
        srand(time(0)+i);
        watchList[i].task_state = name1(i + rand()%15);
        watchList[i].cpuUsage=rand()%100+rand()%10/10.0;
        watchList[i].memoryUsage=rand()%100+rand()%10/10.0;
        //printf("state:%s  ,cpu:%5f,memory:%5f\n",watchList[i].task_state.c_str(),watchList[i].cpuUsage,watchList[i].memoryUsage);
    }

    return 0;
}*/






List::List(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::List)
{
    ui->setupUi(this);
    //声明机制的存在，用以实现LineEdit的clicked信号效果
    ui->pidEdit->installEventFilter(this);
    //整行选中的方式
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //多个选中
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    //设置默认列宽
    ui->tableWidget->setColumnWidth(0,200);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,100);
    ui->tableWidget->setColumnWidth(3,100);
    //隐藏行表头
    ui->tableWidget->verticalHeader()->setVisible(false);
    //设置内容不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置背景交替颜色
    ui->tableWidget->setAlternatingRowColors(true);
    //设置表头背景色
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");


    //创建定时器，设置时间为0.5秒，每0.5秒刷新一遍监视列表界面和列表信息
    timer = new QTimer;
    timer->start(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshWatchList()));
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshListInfo()));

    //第一次打开程序，获取监视列表记录并显示到界面上
    firstShowWatchList();
}

void List::firstShowWatchList(){
    getWatchList();
    showWatchList();
}

void List::getWatchList(){
    vector<struct Process> nowWatchList;
    openWatchList(nowWatchList);
    watchList = nowWatchList;
}

void List::showWatchList()
{
    ui->tableWidget->clearContents();               //清空列表，不包括表头
    ui->tableWidget->setSortingEnabled(false);      //更改列表时，禁止排序，以防数据显示错误

    //把watchList中的信息填充到界面的列表中
    sz_type row = watchList.size();
    ui->tableWidget->setRowCount(int(row));

    for(sz_type i = 0;i < row;i++){

        QTableWidgetItem *processName = new QTableWidgetItem();
        processName->setText(watchList[i].comm.c_str());
        ui->tableWidget->setItem(int(i), 0, processName);

        QTableWidgetItem *processID = new QTableWidgetItem();
        processID->setData(Qt::EditRole, QVariant(watchList[i].pid));
        ui->tableWidget->setItem(int(i), 1, processID);
        ui->tableWidget->item(int(i),1)->setTextAlignment(Qt::AlignCenter);//内容居中

        QTableWidgetItem *taskState = new QTableWidgetItem();
        taskState->setText(watchList[i].task_state.c_str());
        ui->tableWidget->setItem(int(i), 2, taskState);
        ui->tableWidget->item(int(i),2)->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *cpuUsage = new QTableWidgetItem();
        cpuUsage->setData(Qt::EditRole, QVariant(watchList[i].cpuUsage));
        ui->tableWidget->setItem(int(i), 3, cpuUsage);
        ui->tableWidget->item(int(i),3)->setTextAlignment(Qt::AlignCenter);

        //根据不同的CPU值，设置不同的背景颜色
        if(watchList[i].cpuUsage < cpulevel1){
            ui->tableWidget->item(int(i),3)->setBackgroundColor(QColor(255,244,196));
        }
        else if(watchList[i].cpuUsage < cpulevel2){
            ui->tableWidget->item(int(i),3)->setBackgroundColor(QColor(249,236,168));
        }
        else if(watchList[i].cpuUsage < cpulevel3){
            ui->tableWidget->item(int(i),3)->setBackgroundColor(QColor(255,228,135));
        }
        else {
            ui->tableWidget->item(int(i),3)->setBackgroundColor(QColor(255,198,61));
        }

        QTableWidgetItem *memoryUsage = new QTableWidgetItem();
        memoryUsage->setData(Qt::EditRole, QVariant(watchList[i].memoryUsage));
        ui->tableWidget->setItem(int(i), 4, memoryUsage);
        ui->tableWidget->item(int(i),4)->setTextAlignment(Qt::AlignCenter);

        //根据不同的内存值，设置不同的背景颜色
        if(watchList[i].memoryUsage < memlevel1){
            ui->tableWidget->item(int(i),4)->setBackgroundColor(QColor(255,244,196));
        }
        else if(watchList[i].memoryUsage < memlevel2){
            ui->tableWidget->item(int(i),4)->setBackgroundColor(QColor(249,236,168));
        }
        else if(watchList[i].memoryUsage < memlevel3){
            ui->tableWidget->item(int(i),4)->setBackgroundColor(QColor(255,228,135));
        }
        else {
            ui->tableWidget->item(int(i),4)->setBackgroundColor(QColor(255,198,61));
        }
    }
    ui->tableWidget->setSortingEnabled(true);       //列表更改完毕，设置为可排序
}

void List::refreshWatchList(){
    //先更新watchList，再根据watchList中的值，更新列表中的数据
    vector<struct Process> emailList;           //用以获取状态发生变化的进程集合
    updateWatchList(watchList,emailList);       //更新watchList
    updateListData();                           //更新界面列表中的数据

    //如果监视列表进程状态无变化，不操作
    //如果监视列表进程状态变化，发邮件
    if(emailList.empty()){
        return;
    }
    else{
        sendEmail(emailList);
    }
}

void List::updateListData()
{
    ui->tableWidget->setSortingEnabled(false);          //更改列表时，禁止排序，以防数据显示错误

    //对界面的表格中的每一行依次进行更新
    int row = ui->tableWidget->rowCount();
    for(int i = 0;i < row;i++){
        bool ok;
        int pid = ui->tableWidget->item(i,1)->text().toInt(&ok,10);     //从界面的列表中获取进程的ID
        updateRowData(i,pid);           //更新界面表格中第i行、进程ID为pid的数据
    }
    ui->tableWidget->setSortingEnabled(true);           //列表更改完毕，设置为可排序
}

void List::updateRowData(int row,int pid)
{
    int count = watchList.size();
    for(int i = 0;i < count;i++){
        //如果在watchList中找到了对应的ID为pid的进程信息
        if(watchList[i].pid == pid){
            QTableWidgetItem *taskState = new QTableWidgetItem();
            taskState->setText(watchList[i].task_state.c_str());
            ui->tableWidget->setItem(row, 2, taskState);
            ui->tableWidget->item(row,2)->setTextAlignment(Qt::AlignCenter);

            QTableWidgetItem *cpuUsage = new QTableWidgetItem();
            cpuUsage->setData(Qt::EditRole, QVariant(watchList[i].cpuUsage));
            ui->tableWidget->setItem(row, 3, cpuUsage);
            ui->tableWidget->item(row,3)->setTextAlignment(Qt::AlignCenter);

            //根据不同的CPU值，设置不同的背景颜色
            if(watchList[i].cpuUsage < cpulevel1){
                ui->tableWidget->item(row,3)->setBackgroundColor(QColor(255,244,196));
            }
            else if(watchList[i].cpuUsage < cpulevel2){
                ui->tableWidget->item(row,3)->setBackgroundColor(QColor(249,236,168));
            }
            else if(watchList[i].cpuUsage < cpulevel3){
                ui->tableWidget->item(row,3)->setBackgroundColor(QColor(255,228,135));
            }
            else {
                ui->tableWidget->item(row,3)->setBackgroundColor(QColor(255,198,61));
            }

            QTableWidgetItem *memoryUsage = new QTableWidgetItem();
            memoryUsage->setData(Qt::EditRole, QVariant(watchList[i].memoryUsage));
            ui->tableWidget->setItem(row, 4, memoryUsage);
            ui->tableWidget->item(row,4)->setTextAlignment(Qt::AlignCenter);

            //根据不同的内存值，设置不同的背景颜色
            if(watchList[i].memoryUsage < memlevel1){
                ui->tableWidget->item(row,4)->setBackgroundColor(QColor(255,244,196));
            }
            else if(watchList[i].memoryUsage < memlevel2){
                ui->tableWidget->item(row,4)->setBackgroundColor(QColor(249,236,168));
            }
            else if(watchList[i].memoryUsage < memlevel3){
                ui->tableWidget->item(row,4)->setBackgroundColor(QColor(255,228,135));
            }
            else {
                ui->tableWidget->item(row,4)->setBackgroundColor(QColor(255,198,61));
            }

            return;
        }
    }

    //如果在watchList中没有找到对应的进程
    printf("Error! Not find the Process of ID %d in WatchList.\n",pid);
}

void List::getListInfo(QString &listInfo){
    //获取界面中选中的行数，即用选中的单元格个数除以列数
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
    int count = items.count()/ui->tableWidget->columnCount();

    //构造列表信息字符串
    //由两部分组成：1.监视进程个数，即列表行数；2.选中的进程个数
    QString WatchProcessCount = "Process count: " + QString::number(watchList.size());
    QString SeletedProcessCount = "Seleted Process Count: " + QString::number(count);
    QString ListState;
    listInfo = WatchProcessCount + "   |   " + SeletedProcessCount;
}

void List::showListInfo(QString listInfo){
    ui->listInfo->setText(listInfo);
}

void List::refreshListInfo(){
    QString listInfo;
    getListInfo(listInfo);
    showListInfo(listInfo);
}

void List::deleteSeleted()
{
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
    int count = items.count();
    for(int i = 0;i < count;i++){
        int index = items.at(i)->row();
        deleteFromWatchList(index);
        deleteRow(index);
    }
}

int List::deleteRow(int row){
    //当row为-1时，表示当前没有选中的行
    //当row不为-1时，从界面列表中删除该行
    if (row != -1){
         ui->tableWidget->removeRow(row);
         return 0;
    }
    else {
         return 1;
    }
}

void List::deleteFromWatchList(int row)
{
    //当row为-1时，表示当前没有选中的行
    //当row不为-1时，从watchList中删除对应进程的信息
    if (row != -1){
        bool ok;
        int pid = ui->tableWidget->item(row,1)->text().toInt(&ok,10);
        int cnt = watchList.size();
        for(int i = 0;i < cnt;i++){
            //当watchList中有对应的进程，则删除
            if(watchList[i].pid == pid){
                watchList.erase(watchList.begin()+i);
                return;
            }
        }
        //若watchList中没有对应的进程
        printf("Delete pid %d error!\n",pid);
    }
    else printf("Row %d does not exist!\n",row);
}

void List::on_deleteBtn_clicked()
{
    ui->tableWidget->setSortingEnabled(false);      //更改列表时，禁止排序，以防数据显示错误

    //从监视列表中删除进程
    //有两种途径：
    //      1：在界面列表中选择要删除的进程
    //      2：在输入框中输入要删除的进程ID
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
    int count = items.count();

    //当列表中有选中的进程，则删除选中的进程
    //当列表中无选中的进程，则删除输入框中的pid对应的进程
    if(count > 0){
        deleteSeleted();
        refreshWatchList();
    }
    else{
        deleteInputPid();
    }

    ui->tableWidget->setSortingEnabled(true);        //列表更改完毕，设置为可排序
}

void List::deleteInputPid()
{
    //遍历watchList删除对应的进程
    bool ok;
    int pid = ui->pidEdit->text().toInt(&ok,10);
    int i;
    int count = watchList.size();
    for(i = 0; i < count ;i++){
        if(watchList[i].pid == pid){
            watchList.erase(watchList.begin()+i);
            break;
        }
    }
    //当watchList中没有对应的进程时
    if(i == count){
        printf("Error! Not Find %d Process in WatchList.\n",pid);
    }

    //遍历界面列表删除对应的进程
    int rowcount = ui->tableWidget->rowCount();
    int row;
    for(row = 0; row < rowcount ;row++){
        int id = ui->tableWidget->item(row,1)->text().toInt(&ok,10);
        if(id == pid){
            ui->tableWidget->removeRow(row);
            break;
        }
    }
    //当列表中没有对应的进程时
    if(row == rowcount){
        printf("Error! Not Find %d Process in GUI WatchList.\n",pid);
    }
}

List::~List()
{
    //关闭窗口前，保存watchList
    saveWatchList(watchList);
    delete ui;
}

void List::on_addBtn_clicked()
{
    bool ok;
    int pid = ui->pidEdit->text().toInt(&ok,10);
    //添加ID为pid的进程
    if(addProcess(pid,watchList)!=0){
        printf("Error! %d Process has existed in WatchList or does not exist.\n");
        return;
    }

    ui->tableWidget->setSortingEnabled(false);      //更改列表时，禁止排序，以防数据显示错误
    //界面列表中添加一行，并填写添加进程的名称和ID
    int rowcount = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(rowcount + 1);

    QTableWidgetItem *processName = new QTableWidgetItem();
    processName->setText(watchList[rowcount].comm.c_str());
    ui->tableWidget->setItem(rowcount, 0, processName);

    QTableWidgetItem *processID = new QTableWidgetItem();
    processID->setData(Qt::EditRole, QVariant(watchList[rowcount].pid));
    ui->tableWidget->setItem(rowcount, 1, processID);
    ui->tableWidget->item(rowcount,1)->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setSortingEnabled(true);       //列表更改完毕，设置为可排序

    refreshWatchList();
}

bool List::eventFilter(QObject *obj, QEvent *event)
{
    //事件过滤器，当点击LineEdit时，清除列表的选中状态
    if(obj == ui->pidEdit)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            ui->tableWidget->clearSelection();
        }
    }
    return QWidget::eventFilter(obj,event);
}
