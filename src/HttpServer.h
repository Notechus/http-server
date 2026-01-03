#ifndef HTTP_SERVER_HTTPSERVER_H
#define HTTP_SERVER_HTTPSERVER_H

#include "utils.h"

class HttpServer {
private:
    int server = -1;
    int client = -1;
    int port;
    std::string path;

public:
    HttpServer(int port_, std::string path_);

    int start();

    int finalize() const;

    int run();

    bool handleRequest() const;

    HttpRequest readSocket() const;

    static HttpRequest processRequest(const std::string &request);

    void handle200(HttpRequest &request) const;

    void handle301(const HttpRequest &request) const;

    void handle403() const;

    void handle404() const;

    void handle501() const;

    void keepAlive() const;

    static void extractMethod(std::string line, HttpRequest &request);

    static void extractHost(std::string line, HttpRequest &request);

    static void extractConnection(std::string line, HttpRequest &request);

    static void setHeader(HttpRequest &request, std::string &header);

    void sendTextFile(const std::string &filePath) const;

    void sendBinaryFile(const std::string &filePath) const;
};


#endif //HTTP_SERVER_HTTPSERVER_H
