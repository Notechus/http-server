#include "HttpServer.h"

#include <utility>

void custom_trim(std::string &text);

HttpServer::HttpServer(const int port_, std::string path_)
    : port(port_), path(std::move(path_)) {
}

int HttpServer::start() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "There was an error while initializing the socket\n";
        return -1;
    }

    struct sockaddr_in serv;

    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(this->port);

    if (bind(sock, (const sockaddr *) &serv, sizeof(serv)) < 0) {
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
    return close(server) || close(client);
}

int HttpServer::run() {
    struct sockaddr_in client_name{};
    socklen_t client_name_len = sizeof(client_name);

    while (true) {
        client = accept(server, (struct sockaddr *) &client_name, &client_name_len);
        if (client == -1) {
            std::cerr << "There was error while accepting the connection\n";
            std::cout << strerror(errno) << std::endl;
            finalize();
            return -1;
        }
        handleRequest();
        close(client);
    }
}

bool HttpServer::handleRequest() {
    HttpRequest request = readSocket();
    HttpResponse response;
    std::cout << "Got request for: " << request.path << " " << request.file << std::endl;
    if (request.method != MethodType::GET) {
        handle501();
        std::cout << "501 Method Not Implemented\n";
    } else if (request.file == "" || request.file == "/") {
        handle301(request);
        std::cout << "301 Moved Permanently\n";
    } else if (request.file.find_last_of(".") > request.file.length()) {
        handle403();
        std::cout << "403 Forbidden\n";
    } else {
        handle200(request);
    }

    return request.keepAlive;
}

HttpRequest HttpServer::readSocket() {
    char buffer[BUFFER_SIZE];

    ssize_t received = recv(client, buffer, (size_t) BUFFER_SIZE, 0);

    if (received < 0) {
        std::cerr << "There was an error while receiving data\n";
        std::terminate();
    }

    return processRequest(std::string(buffer));
}

HttpRequest HttpServer::processRequest(std::string request) {
    HttpRequest req;

    std::istringstream stream(request);
    std::string method, host, connection;
    getline(stream, method);
    getline(stream, host);
    getline(stream, connection);

    std::cout << "Processing the request "
            << method << ", "
            << host << ", "
            << connection << ", "
            << std::endl;

    extractMethod(method, req);
    extractHost(host, req);
    extractConnection(connection, req);

    return req;
}

void HttpServer::extractMethod(std::string line, HttpRequest &request) {
    custom_trim(line);
    unsigned long space = line.find_first_of(' ');
    unsigned long http = line.find_last_of(' ');

    if (space > 0 && http > 0) {
        std::string method = line.substr(0, space);
        std::string url = line.substr(space + 1, http - (space + 1));
        custom_trim(url);
        request.file = url.substr(1, url.length());

        if (method == "GET") {
            request.method = MethodType::GET;
        } else {
            request.method = MethodType::NOT_IMPLEMENTED;
        }
    }
}

void HttpServer::extractHost(std::string line, HttpRequest &request) {
    line = line.substr(6, line.length() - 6);
    custom_trim(line);
    unsigned long dot = line.find_last_of(":");
    if (dot > 0) {
        unsigned long found = line.find_first_of("http://");
        if (found < 8) {
            request.path = line.substr(7, dot - 7);
        } else {
            request.path = line.substr(0, dot);
        }
        try {
            std::string port = line.substr(dot + 1, line.length() - dot);
            request.port = std::stoi(port);
        } catch (...) {
        }
    }
}

void HttpServer::extractConnection(std::string line, HttpRequest &request) {
    custom_trim(line);
    unsigned long dot = line.find_first_of(":");
    if (dot > 0 && dot < line.length() - 1) {
        std::string connection = line.substr(dot + 1, line.length());
        custom_trim(connection);
        request.keepAlive = connection == "keep-alive";
    }
}

void HttpServer::handle200(HttpRequest &request) {
    request.path = "dom1.abc.pl";
    std::string filePath = this->path + "/" + request.path + "/" + request.file;
    char buff[RESPONSE_BUFFER];
    std::string header;
    std::ifstream file;
    file.open(filePath);
    if (!file.is_open()) {
        handle404();
        std::cout << "404 Not Found\n";
    } else {
        file.close();
        setHeader(request, header);
        sprintf(buff, "HTTP/1.1 200 OK\r\n");
        send(client, buff, strlen(buff), 0);
        sprintf(buff, "%s", header.c_str());
        send(client, buff, strlen(buff), 0);
        strcpy(buff, "\r\n");
        send(client, buff, strlen(buff), 0);
        if (request.content == "html" || request.content == "css") {
            sendTextFile(filePath);
        } else {
            sendBinaryFile(filePath);
        }
        std::cout << "200 OK\n";
    }
}

void HttpServer::handle301(const HttpRequest &request) {
    char buff[RESPONSE_BUFFER];

    sprintf(buff, "HTTP/1.1 301 Moved Permanently\r\n");
    send(client, buff, strlen(buff), 0);
    sprintf(buff, "Location: http://%s:%d/index.html\r\n", request.path.c_str(), request.port);
    send(client, buff, strlen(buff), 0);
    sprintf(buff, "\r\n");
    send(client, buff, strlen(buff), 0);
}

void HttpServer::handle403() {
    char buff[RESPONSE_BUFFER];

    sprintf(buff, "HTTP/1.1 403 Forbidden\r\n");
    send(client, buff, strlen(buff), 0);
    sprintf(buff, "Content-Type: text/html\r\n");
    send(client, buff, strlen(buff), 0);
    sprintf(buff, "\r\n");
    send(client, buff, strlen(buff), 0);
    sprintf(buff, "<html><head><title>Forbidden</title></head>\r\n");
    send(client, buff, strlen(buff), 0);
    sprintf(buff, "<body><h3>Forbidden</h3></body></html>\r\n");
    send(client, buff, strlen(buff), 0);
}

void HttpServer::handle404() {
    char buff[RESPONSE_BUFFER];

    sprintf(buff, "HTTP/1.1 404 Not Found\r\n");
    send(client, buff, strlen(buff), 0);
    sprintf(buff, "Content-Type: text/html\r\n");
    send(client, buff, strlen(buff), 0);
    sprintf(buff, "\r\n");
    send(client, buff, strlen(buff), 0);
    sprintf(buff, "<html><head><title>Not Found</title></head>\r\n");
    send(client, buff, strlen(buff), 0);
    sprintf(buff, "<body><h3>404 Not found</h3></body></html>\r\n");
    send(client, buff, strlen(buff), 0);
}

void HttpServer::handle501() {
    char buff[RESPONSE_BUFFER];

    sprintf(buff, "HTTP/1.1 501 Method Not Implemented\r\n");
    send(client, buff, strlen(buff), 0);
    sprintf(buff, "Content-Type: text/html\r\n");
    send(client, buff, strlen(buff), 0);
    sprintf(buff, "\r\n");
    send(client, buff, strlen(buff), 0);
    sprintf(buff, "<html><head><title>Method Not Implemented</title></head>\r\n");
    send(client, buff, strlen(buff), 0);
    sprintf(buff, "<body><p>Chosen HTTP method is not supported yet.</body></html>\r\n");
    send(client, buff, strlen(buff), 0);
}

void HttpServer::setHeader(HttpRequest &request, std::string &header) {
    unsigned long dot = request.file.find_last_of('.');
    std::string extension = request.file.substr(dot + 1, request.file.length() - dot);

    if (extension == "html") {
        header = "Content-Type: text/html\r\n";
        request.content = "html";
    } else if (extension == "css") {
        header = "Content-Type: text/css\r\n";
        request.content = "css";
    } else if (extension == "jpg") {
        header = "Content-Type: image/jpg\r\n";
        request.content = "jpg";
    } else if (extension == "png") {
        header = "Content-Type: image/png\r\n";
        request.content = "png";
    } else if (extension == "jpeg") {
        header = "Content-Type: image/jpeg\r\n";
        request.content = "jpeg";
    } else if (extension == "pdf") {
        header = "Content-Type: application/pdf\r\n";
        request.content = "pdf";
    } else {
        header = "Content-Type: application/octet-stream\r\n";
        request.content = "octet-stream";
    }
}

void HttpServer::keepAlive() {
    fd_set descriptors;
    FD_ZERO(&descriptors);
    FD_SET(server, &descriptors);
    struct timeval tv;
    tv.tv_sec = TIMEOUT_SECONDS;
    tv.tv_usec = 0;

    int ready = select(server + 1, &descriptors, nullptr, nullptr, &tv);
    if (ready > 0) {
        readSocket();
    }
}

void HttpServer::sendTextFile(const std::string &filePath) const {
    std::string line;
    std::ifstream file;
    file.open(filePath);
    if (file.is_open()) {
        while (getline(file, line)) {
            send(client, line.c_str(), line.length(), 0);
        }
        file.close();
    }
}

void HttpServer::sendBinaryFile(const std::string &filePath) const {
    std::ifstream file;
    char buff[RESPONSE_BUFFER];
    file.open(filePath, std::ios::binary | std::ios::in);
    file.seekg(0, std::ios::end);
    ssize_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    while (size != 0 && !file.eof()) {
        if (size > RESPONSE_BUFFER) {
            file.read(buff, RESPONSE_BUFFER);
            send(client, buff, RESPONSE_BUFFER, 0);
            size -= RESPONSE_BUFFER;
        } else {
            file.read(buff, size);
            send(client, buff, size, 0);
            size -= size;
        }
    }

    file.close();
}

void custom_trim(std::string &text) {
    text.erase(std::find_if(text.rbegin(), text.rend(),
                            [](const char ch) { return !std::isspace(static_cast<unsigned char>(ch)); }).base(),
               text.end());
}
