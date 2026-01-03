#ifndef HTTP_SERVER_HTTPSERVER_H
#define HTTP_SERVER_HTTPSERVER_H

#include "utils.h"

class HttpServer {
    int server = -1;
    int port;
    std::string path;

public:
    HttpServer(int port_, std::string path_);

    int start();

    int finalize() const;

    int run() const;

private:
    bool handleRequest(int client) const;

    HttpRequest readSocket(int client) const;

    static HttpRequest processRequest(const std::string &request);

    void handle200(HttpRequest &request, int client) const;

    void handle301(const HttpRequest &request, int client) const;

    void handle403(int client) const;

    void handle404(int client) const;

    void handle501(int client) const;

    static void extractMethod(std::string line, HttpRequest &request);

    static void extractHost(std::string line, HttpRequest &request);

    static void extractConnection(std::string line, HttpRequest &request);

    static void setHeader(HttpRequest &request, std::string &header, const std::string &filePath);

    void sendBinaryFile(const std::string &filePath, int client) const;

    void handleClient(int client_fd) const;

    static std::string getContentType(const std::string &filename);
};


#endif //HTTP_SERVER_HTTPSERVER_H
