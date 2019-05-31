#ifndef MEMWINDOW_H
#define MEMWINDOW_H

#include <QWidget>
#include <QTimer>
#include "processList.h"
#include "swapinfo.h"

namespace Ui {
class memWindow;
}

class memWindow : public QWidget
{
    Q_OBJECT

public:
    explicit memWindow(QWidget *parent = 0);
    ~memWindow();

private:
    Ui::memWindow *ui;
    QTimer *timer;
private slots:
    void showMemWindow();
};

#endif // MEMWINDOW_H
