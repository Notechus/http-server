// Sebastian Paulus 266446
#ifndef HTTP_SERVER_UTILS_H
#define HTTP_SERVER_UTILS_H


#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 65535
#define CONNECTION_LIMIT 5

enum class MethodType {
    GET, POST, UNKNOWN
};

struct HttpRequest {
    MethodType method;
    std::string url;
    std::string host;
    std::string path;
    int port;
};

#endif //HTTP_SERVER_UTILS_H
