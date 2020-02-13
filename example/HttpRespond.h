//
// Created by Jialu  Hu on 2019-08-06.
//

#ifndef UNTITLED_HTTPRESPOND_H
#define UNTITLED_HTTPRESPOND_H

#include "HttpContent.h"
#include "Tcpconn.h"
#include "Buffer.h"
#include "ResPond.h"
#include <sys/stat.h>
#include <sys/mman.h>
class GetConfig;
class HttpRespond{
public:
    HttpRespond();
    ~HttpRespond();
    void set_url(std::string url){
        url_ = url;
    }
      void set_version(std::string version){
        version_ = version;
    }

    /*void set_Respond_Header(std::string head){
        Respond_Header_ = head;
    }*/

    /*void set_ContentLength(std::string length){
        ContentLength_ = length;
    }*/
    void set_ContentLanguage(std::string language){
        ContentLanguage_ = language;
    }

    /*void set_ContentType(std::string type){
        ContentType_ = type;
    }*/

    void set_Connection(std::string Connction){
        Connection_ = Connction;
    }

    void set_method(std::string method){
        method_ = method;
    }


    void set_postcontent(std::string st){
        post_content = st;
    }


    void set_Host_(std::string host){
        Host_ = Host_;
    }

    void set_Document(std::string dp){
        DocumentPath_ = std::move(dp);
    }
    void set_CGIPath(std::string cgi, bool B_CGI){
        this->B_CGI = B_CGI;
        CGIPath_ = std::move(cgi);
    }

    void FillRespond_POST(const SiNet::TcpConnectionPtr &conn){
        if(B_CGI){
            method_ = CGIPath_ + method_;
            conn->Post_deal(method_.c_str(), post_content.c_str());
        }else{
            FillRespond_GET(conn);
        }

    }
    void FillRespond_GET(const SiNet::TcpConnectionPtr &conn){
            if(method_=="/"){
                method_ = DocumentPath_ + "/" + INDEX;
            }else{
                method_ =DocumentPath_ + method_;
            }
            int fd = open(method_.c_str(),O_RDWR);
            if(fd < 0){
                std::string header(HTTP_NOTFOUND);
                std::string content(HTTP_CONTENT);
                conn->set_HandleErrno(fd, header);

            }
            else{
                std::string header(HTTP_OK);
                struct stat tbuf;
                fstat(fd,&tbuf);
                char buf[100];
                sprintf(buf,"Content-Length: %d\r\n\r\n",tbuf.st_size);
                buf[strlen(buf)]='\0';
                header = header+buf;
                int save = 0;
                conn->set_Handlewrite(method_.c_str(),fd,header);
            }
            close(fd);
    }
private:
    std::string url_;
    std::string method_;
    std::string version_;

    std::string post_content;

    //std::string Respond_Header_;
    std::string Host_;
    std::string ContentLanguage_;
    //std::string ContentLength_;
    //std::string ContentType_;
    std::string Connection_;

    std::string DocumentPath_;
    std::string CGIPath_;
    bool B_CGI;

};

HttpRespond::HttpRespond() {

}
HttpRespond::~HttpRespond() {

}
#endif //UNTITLED_HTTPRESPOND_H
