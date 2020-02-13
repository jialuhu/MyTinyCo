//
// Created by Jialu  Hu on 2020-01-18.
//

#ifndef UNTITLED_CONFIG_H
#define UNTITLED_CONFIG_H
__const char* CONFIG_PATH {"httped.conf"};
__const char* SERVERROOT {"/etc/TinyHtp/WWW"};
__const int LISTEN{8888};
__const char* DOCUMENTPATH {"/etc/TinyHtp/WWW/Documents"};
__const char* CGIPATH {"/etc/TinyHtp/WWW/CGI"};
__const char* ADDRESS {"127.0.0.1"};
__const bool CGI{false};
__const int ThreadNumber{0};

__const char* HEAD_SERVERROOT {"ServerRoot:"};
__const char* HEAD_LISTEN {"Listen:"};
__const char* HEAD_DOCUMENTPATH {"DocumentPath:"};
__const char* HEAD_CGIPATH {"CGIPath:"};
__const char* HEAD_CGI {"CGI:"};
__const char* HEAD_ADDR {"ADDR:"};
__const char* HEAD_ThreadNumber{"ThreadNumber:"};

__const char* CONFIG_START {"<Start>"};
__const char* CONFIG_END {"</End>"};

__const char* YESCGI {"YES"};
__const char* NOCGI {"NO"};
#endif //UNTITLED_CONFIG_H
