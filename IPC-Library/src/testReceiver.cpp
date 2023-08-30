#include <iostream>
#include <string>
#include <thread>
#include "MsgQConnect.hpp"
//g++ testReceiver.cpp -o testReceive -I../include -lMsgQConnect  -lrt -pthread

void callback(std::string message)
{
    std::cout << "Received message: " << message << std::endl;
}

int main()
{
    // Create a message queue receiver with the name "my_queue"
    MessageQueueReceiver receiver("/my_queue");

    // Receive a message from the queue synchronously
    // std::string received_message = receiver.receiveMessageSynchronous();
    // std::cout << "Received message: " << received_message << std::endl;

    // Receive messages from the queue asynchronously
    std::thread receiver_thread;
    receiver.receiveMessageAsynchronous(callback, receiver_thread);

    // // Create a message queue sender with the name "my_queue"
    // MessageQueueSender sender("my_queue");

    // // Send a message to the queue
    // std::string message = "Hello, world!";
    // sender.send_message(message.c_str());

    // // Wait for the receiver thread to finish
    receiver_thread.join();

    return 0;
}