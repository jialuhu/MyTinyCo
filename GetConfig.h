//
// Created by Jialu  Hu on 2020-01-18.
//

#ifndef UNTITLED_GETCONFIG_H
#define UNTITLED_GETCONFIG_H
#include <fstream>
#include <iostream>
#include <cstring>
#include "Config.h"
class GetConfig{
public:
    GetConfig():ConfigPath(){

    }
    GetConfig(const char *ConfigPath_):ConfigPath(ConfigPath_){

    }
    ~GetConfig(){

    }
    void ReadConfig(){
        std::ifstream Read(ConfigPath);
        char buf[1024];
        while(true){
            memset(buf,0,1024);
            Read.getline(buf,1024);
            buf[strlen(buf)]='\0';
            if(buf[0]=='#'||strcmp(buf,CONFIG_START)==0){
                continue;
            }
            std::string txt(buf);
            if(txt.compare(CONFIG_END)==0){
                break;
            }else{
                SetConfig(buf);
            }
        }
        Read.close();
    }
    void SetConfig(char *important){
        char *p = strstr(important," ");
        char *type = p+1;
        p[0] = '\0';
        if(strcmp(important,HEAD_SERVERROOT)==0){
            if(ServerRoot.compare(type)!=0){
                ServerRoot.clear();
                ServerRoot.insert(0,type);
            }
        }else if(strcmp(important,HEAD_CGI)==0){
            if(strcmp(type,YESCGI)==0){
                B_CGI = true;
            }else{
                B_CGI = false;
            }

        }else if(strcmp(important,HEAD_LISTEN)==0){
            int P = atoi(type);
            if(P != Listen_Port){
                Listen_Port = P;
            }
        }else if(strcmp(important,HEAD_DOCUMENTPATH)==0){
            if(DocumentPath.compare(type)!=0){
                DocumentPath.clear();
                DocumentPath.insert(0,type);
            }

        }else if(strcmp(important,HEAD_CGIPATH)==0){
            if(CGIPath.compare(type)!=0){
                CGIPath.clear();
                CGIPath.insert(0,type);
            }
        }
    }
    void display(){
        std::cout << ServerRoot << "\n";
        std::cout << Listen_Port << "\n";
        std::cout << DocumentPath << "\n";
        std::cout << CGIPath << "\n";
        std::cout << B_CGI << "\n";
    }
    const char* Return_ServerRoot(){
        return ServerRoot.c_str();
    }
    const char* Return_DocumentPath(){
        return DocumentPath.c_str();
    }
    const char* Return_CGIPath(){
        return CGIPath.c_str();
    }
    int Return_Listen(){
        return Listen_Port;
    }
    bool Return_CGI(){
        return B_CGI;
    }
private:
    const char *ConfigPath{CONFIG_PATH};
    std::string ServerRoot{SERVERROOT};
    int Listen_Port{LISTEN};
    std::string DocumentPath{DOCUMENTPATH};
    std::string CGIPath{CGIPATH};
    bool B_CGI{CGI};
};

#endif //UNTITLED_GETCONFIG_H
