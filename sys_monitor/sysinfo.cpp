#include "sysinfo.h"
#include "ui_sysinfo.h"

sysInfo::sysInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sysInfo)
{
    ui->setupUi(this);
    showSysInfo();
}

sysInfo::~sysInfo()
{
    delete ui;
}

void sysInfo::showSysInfo(){
    QString tempStr;    //读取文件信息字符串
    QFile tempFile;     //用于打开系统文件
    int pos;            //读取文件的位置

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
