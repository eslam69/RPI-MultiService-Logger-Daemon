#include <iostream>
#include <string>
#include <thread>
#include "MsgQConnect.hpp"
//g++ testSender.cpp -o testSend -I../include -lMsgQConnect  -lrt -pthread  


int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <message>" << std::endl;
        return 1;
    }

    const char *q = argv[1];
    const char *message = argv[2];


    // Create a message queue sender with the name "my_queue"
    MessageQueueSender sender(q);

    // Send a message to the queue
    // std::string message = "Hello, world!";
    sender.send_message(message);

    // Wait for the receiver thread to finish
    // receiver_thread.join();

    return 0;
}