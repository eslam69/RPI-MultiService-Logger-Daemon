#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class Client {
public:
    Client(const char* serverAddress, int port) : serverAddress(serverAddress), port(port), clientSocket(-1) {}

    bool connectToServer() {
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1) {
            std::cerr << "Error creating socket" << std::endl;
            return false;
        }

        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = inet_addr(serverAddress);

        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
            std::cerr << "Error connecting" << std::endl;
            return false;
        }
        return true;
    }

    bool sendData(const char* data) {
        ssize_t bytesSent = send(clientSocket, data, strlen(data), 0);
        if (bytesSent == -1) {
            std::cerr << "Error sending data" << std::endl;
            return false;
        }
        return true;
    }

    void closeConnection() {
        close(clientSocket);
    }

private:
    const char* serverAddress;
    int port;
    int clientSocket;
};

int main() {
    Client client("127.0.0.1", 12345);

    // if (!client.connectToServer()) {
    //     return 1;
    // }
    //try to connect to the server
    while (true)
    {
        if (client.connectToServer()){
            break;
        }
        else{
            std::cout << "Trying to connect to the Daemon server..." << std::endl;
            sleep(1);
        }
    }
    

    const char* message = "Hello, world!";
    if (client.sendData(message)) {
        std::cout << "Sent: " << message << std::endl;
    }

    client.closeConnection();

    return 0;
}
