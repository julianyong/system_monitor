#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "list.h"
#include "processwindow.h"
#include "memwindow.h"
#include "sysinfo.h"
#include <QMainWindow>
#include <QDir>
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
   List *WatchList;
   processwindow *ProcessInfo;
   memWindow *MemWindow;
   sysInfo *SysInfo;
   private slots:
   void closeEvent(QCloseEvent *event);
   void on_pushButton_reboot_clicked();
   void on_pushButton_halt_clicked();
};

#endif // MAINWINDOW_H
