#ifndef EMAIL_H
#define EMAIL_H
#include <string>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <watchList.h>
using namespace std;

/*
 * 函数说明：
 *      填写邮件的内容
 * 输入参数：
 *      emailList：保存有需要发送邮件的进程的信息
 * 返 回 值：
 *      0：邮件内容填写成功
 *      -1：打开文件失败
 *      other：邮件内容填写失败
 */
int saveEmailContent(vector<struct Process> emailList);

/*
 * 函数说明:
 *      发送邮件。将监视列表中状态发生变化的进程的信息发送给指定邮箱
 * 输入参数：
 *      emailList：保存有需要发送邮件的进程的信息
 * 返 回 值：
 *      0：发送成功
 *      other：发送失败
 */
int sendEmail(vector<struct Process> emailList);




#endif // EMAIL_H
