#ifndef LIST_H
#define LIST_H

#include <QWidget>
#include <QTimer>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "watchList.h"
#define sz_type unsigned int

namespace Ui {
class List;
}

class List : public QWidget
{
    Q_OBJECT

public:
    explicit List(QWidget *parent = nullptr);

    //第一次打开程序，获取监视列表记录并显示到界面上
    void firstShowWatchList();

    //第一次打开程序时，获取之前的监视列表的记录，
    //并加入当前的监视列表中
    void getWatchList();

    //第一次打开程序，获取到监视列表后显示到界面上
    void showWatchList();


    /*
     * 函数说明：
     *      输出列表信息到界面
     * 输入参数：
     *      listInfo：输出到界面的列表信息
     */
    void showListInfo(QString listInfo);

    /*
     * 函数说明：
     *      获取当前监视列表的信息，
     *      信息包括监视的程序个数以及当前选中的程序个数
     * 输入参数：
     *      listInfo：用于存储列表信息
     */
    void getListInfo(QString &listInfo);

    /*
     * 函数说明：
     *      更新列表中的数据，
     *      当更新完存储监视程序的结构体watchList后调用，
     *      以更新列表中的状态、CPU、内存等进程属性的值
     */
    void updateListData();

    /*
     * 函数说明：
     *      更新列表中的每一行的数据，
     *      更新内容：状态、CPU、内存等进程属性的值
     * 输入参数：
     *      row：列表中的行号
     *      pid：进程的唯一标识符
     */
    void updateRowData(int row,int pid);

    // 删除界面中选中的进程
    void deleteSeleted();

    /*
     * 函数说明：
     *      从列表中删除第row行
     * 输入参数：
     *      row：列表中的行号
     * 返 回 值：
     *      0：删除成功
     *      1：没有选中的行，删除失败
     */
    int  deleteRow(int row);

    /*
     * 函数说明：
     *      从监视列表结构watchList中删除列表中第row行所对应的信息
     *
     * 输入参数：
     *      row：列表中的行号
     */
    void deleteFromWatchList(int row);

    /*
     * 函数说明：
     *      用以删除从LineEdit输入框中输入的进程ID未pid的进程，
     *      从watchList结构中删除该进程对应的信息，
     *      从界面中删除该进程对应的行
     */
    void deleteInputPid();
    ~List();
protected:
    /*
     * 函数说明：
     *      时间过滤器，用以实现点击LineEdit进行相应的操作
     * 输入参数：
     *      obj：界面中发生事件的对象
     *      event：obj所发生的事件
     */
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::List *ui;                                   //创建的界面对象
    vector<struct Process> watchList;               //存储所有监视进程的信息
    QTimer *timer;                                  //定时器
    double cpulevel1=70,cpulevel2=100,cpulevel3=150;//根据该值将cpu值划分为不同等级，进而上色
    double memlevel1=70,memlevel2=100,memlevel3=150;//根据该值将内存值划分为不同等级，进而上色

private slots:
    void refreshWatchList();                        //刷新监视列表，包括watchList和界面
    void refreshListInfo();                         //刷新列表信息，包括监视的程序个数以及当前选中的程序个数
    void on_deleteBtn_clicked();                    //点击deleteBtn按钮触发的操作
    void on_addBtn_clicked();                       //点击addBtn按钮触发的操作
};

#endif // LIST_H
