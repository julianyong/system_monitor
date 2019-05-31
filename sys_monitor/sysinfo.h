#ifndef SYSINFO_H
#define SYSINFO_H

#include <QWidget>
#include <QFile>
#include <QMessageBox>
namespace Ui {
class sysInfo;
}

class sysInfo : public QWidget
{
    Q_OBJECT

public:
    explicit sysInfo(QWidget *parent = 0);
    ~sysInfo();

private:
    Ui::sysInfo *ui;
private slots:
    void showSysInfo();
};

#endif // SYSINFO_H
