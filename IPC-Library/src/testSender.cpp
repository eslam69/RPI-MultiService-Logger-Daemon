#include <iostream>
#include <string>
#include <thread>
#include "MsgQConnect.hpp"
//g++ testSender.cpp -o testSend -I../include -lMsgQConnect  -lrt -pthread  


int main()
{
    
    // Create a message queue sender with the name "my_queue"
    MessageQueueSender sender("/my_queue");

    // Send a message to the queue
    std::string message = "Hello, world!";
    sender.send_message(message.c_str());

    // Wait for the receiver thread to finish
    // receiver_thread.join();

    return 0;
}