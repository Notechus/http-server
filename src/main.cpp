#include "utils.h"
#include "HttpServer.h"

int serverPort;
std::string path;
DIR *hostingDir;

bool validateInput(int length, char **args) {
    if (length != 3) {
        std::cout << "Correct usage is  ./http-server <port> <folder>\n";
        return false;
    }
    try {
        serverPort = std::stoi(args[1]);
        path = args[2];
        hostingDir = opendir(args[2]);
        if (!hostingDir) {
            std::cerr << "The directory: " + path << " couldn't be opened\n";
            return false;
        }
        closedir(hostingDir);
        return true;
    } catch (...) {
        std::cerr << "There was an error while processing arguments\n";
        return false;
    }
}

int main(int argc, char **argv) {
    if (!validateInput(argc, argv)) {
        std::cerr << "The application will close.\n";
        return EXIT_FAILURE;
    }

    HttpServer server(serverPort, path);

    if (server.start()) {
        const int run = server.run();
        if (run < 0) {
            std::cerr << "There was an error during server work\n";
            std::cerr << "The application will close.\n";
            const int code = server.finalize();

            std::cout << "Exited with code " << code << std::endl;

            return EXIT_FAILURE;
        }
    }
    const int code = server.finalize();

    std::cout << "Exited with code " << code << std::endl;
    return 0;
}
