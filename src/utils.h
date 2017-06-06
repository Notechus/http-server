// Sebastian Paulus 266446
#ifndef HTTP_SERVER_UTILS_H
#define HTTP_SERVER_UTILS_H


#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 65535
#define RESPONSE_BUFFER 1024
#define CONNECTION_LIMIT 5
#define TIMEOUT_SECONDS 1

enum class MethodType {
    GET, NOT_IMPLEMENTED
};

struct HttpRequest {
    MethodType method;
    std::string path;
    std::string file;
    bool keepAlive;
    int port;
};

struct HttpResponse {
    std::string status;
    std::string headers;
};

#endif //HTTP_SERVER_UTILS_H
