#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class Server {
public:
    Server(int port) : serverSocket(-1), port(port) {}

    bool start() {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1) {
            std::cerr << "Error creating socket" << std::endl;
            return false;
        }

        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
            std::cerr << "Error binding socket" << std::endl;
            return false;
        }

        if (listen(serverSocket, 5) == -1) {
            std::cerr << "Error listening" << std::endl;
            return false;
        }

        std::cout << "Server listening on port " << port << "..." << std::endl;

        return true;
    }

    void run() {
        while (true) {
            if (!acceptConnection()) {
                break;
            }

            char buffer[2000];
            ssize_t bytesRead = receiveData(buffer, sizeof(buffer));
            if (bytesRead > 0) {
                std::cout << buffer << std::endl;
            }

            closeConnection();
        }
    }

    bool acceptConnection() {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection" << std::endl;
            return false;
        }
        return true;
    }

    ssize_t receiveData(char* buffer, size_t bufferSize) {
        ssize_t bytesRead = recv(clientSocket, buffer, bufferSize - 1, 0);
        if (bytesRead <= 0) {
            std::cerr << "Error receiving data" << std::endl;
            return -1;
        }
        buffer[bytesRead] = '\0';
        return bytesRead;
    }

    void closeConnection() {
        close(clientSocket);
    }

    void stop() {
        close(serverSocket);
    }

private:
    int serverSocket;
    int clientSocket;
    int port;
};

int main() {
    Server server(12345);

    if (!server.start()) {
        return 1;
    }

    server.run();

    server.stop();

    return 0;
}
