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
    bool handleRequest();

    HttpRequest readSocket();

    HttpRequest processRequest(std::string request);

    void handle200(HttpRequest &request);

    void handle301(HttpRequest &request);

    void handle403();

    void handle404();

    void handle501();

    void keepAlive();

    void extractMethod(std::string line, HttpRequest &request);

    void extractHost(std::string line, HttpRequest &request);

    void extractConnection(std::string line, HttpRequest &request);

    void setHeader(HttpRequest request, std::string &header);
};


#endif //HTTP_SERVER_HTTPSERVER_H
