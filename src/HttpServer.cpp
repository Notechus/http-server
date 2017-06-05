#include "HttpServer.h"

HttpServer::HttpServer(int port_, std::string address_, std::string path_)
        : port(port_), address(address_), path(path_) {

}

int HttpServer::start() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "There was an error while initializing the socket\n";
        return -1;
    }

    struct sockaddr_in server;

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(this->port);

    if (bind(sock, (const sockaddr *) &server, sizeof(server)) < 0) {
        std::cerr << "There was an error while binding the socket\n";
        std::cerr << strerror(errno) << std::endl;
        return -1;
    }
    if (listen(sock, CONNECTION_LIMIT) == -1) {
        std::cerr << "There was an error in listen call\n";
        std::cerr << strerror(errno) << std::endl;
        return -1;
    }
    this->server = sock;

    std::cout << "Server is running on port " << port << std::endl;

    return sock;
}

int HttpServer::finalize() {
    return close(server);
}

int HttpServer::run() {
    struct sockaddr_in client_name;
    socklen_t client_name_len = sizeof(client_name);

    while (1) {
        client = accept(server, (struct sockaddr *) &client_name, &client_name_len);
        if (client == -1) {
            std::cerr << "There was error while accepting the connection\n";
            std::cout << strerror(errno) << std::endl;
            finalize();
            return -1;
        }
        handleRequest();
    }

    return 1;
}

void HttpServer::handleRequest() {
    char buffer[BUFFER_SIZE];
    char method_buff[100];
    char url_buff[256];
    char host_buff[256];
    char path_buff[1024];

    int bytesRead = readSocket(buffer, BUFFER_SIZE);
    if (bytesRead > 0) {
        std::cout << buffer << std::endl;
    }
}

int HttpServer::readSocket(char *buffer, int length) {

    char c = 0;
    int i = 0;
    while (i < length - 1 && c != '\n') {
        ssize_t received = recv(this->server, &c, 1, 0);
        std::cerr << strerror(errno) << std::endl;
        i++;
    }
//    if (received)
//        std::cout << "received: " << received << std::endl;
    return 0;
}
