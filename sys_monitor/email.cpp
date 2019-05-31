#ifndef EMAIL_CPP
#define EMAIL_CPP
#include <email.h>

int saveEmailContent(vector<struct Process> emailList){
    FILE * file;
    file = fopen("emailIn.txt","w");
    if(file == NULL){
        printf("Open file failed!\n");
        return -1;
    }
    size_t count = emailList.size();
    for(size_t i = 0;i < count ; i++){
        string info;
        info = "Process name is " + emailList[i].comm + ", ";
        info = info + "Process pid is " + to_string(emailList[i].pid)+ ", ";
        info = info + "Process state is " + emailList[i].task_state + "\n";
        fputs(info.c_str(),file);
    }
    fclose(file);
    return 0;
}

int sendEmail(vector<struct Process> emailList){
    saveEmailContent(emailList);
    system("python3 emailSend.py");
    return 0;
}

#endif // EMAIL_CPP
