#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "list.h"
#include "processwindow.h"
#include <QMainWindow>
#include <QFile>
#include <QDir>
#include <QTimer>
#include <QMessageBox>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
   Ui::MainWindow *ui;
   QTimer *timer; //计时器
   List *WatchList;
   processwindow *ProcessInfo;
   private slots:
   void closeEvent(QCloseEvent *event);
   void on_pushButton_kill_clicked();
   void on_pushButton_refresh_clicked();
   void on_pushButton_reboot_clicked();
   void on_pushButton_halt_clicked();
   void on_tabWidget_currentChanged(int index);
   void timer_update_currentTabInfo();
   //显示tab中的内容
   void show_tabWidgetInfo(int index);
//   void on_tabWidget_currentChanged(int index);
};

#endif // MAINWINDOW_H
