#ifndef HTTP_SERVER_HTTPSERVER_H
#define HTTP_SERVER_HTTPSERVER_H

#include "utils.h"

class HttpServer {
private:
    int server = -1;
    int client = -1;
    int port;
    std::string address;
    std::string path;

public:
    HttpServer(int port_, std::string address_, std::string path_);

    int start();

    int finalize();

    int run();

private:
    void handleRequest();

    int readSocket(char *buffer, int length);

    void handle200();

    void handle301();

    void handle403();

    void handle404();

    void handle501();
};


#endif //HTTP_SERVER_HTTPSERVER_H
