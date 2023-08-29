// compile app: g++ -std=c++11 -pthread -lrt -o receiver receiver.cpp
//compile lib:
// g++ -c -fPIC receiver.cpp -o receiver.o
// g++ -shared -Wl,-soname,libreceiver.so -o libreceiver.so receiver.o -lrt -pthread

#include "../lib/receiver.hpp"

MessageQueueReceiver::MessageQueueReceiver(std::string message_queue_name, int max_messages, int max_message_size)
{

    configuration.mq_flags = 0;
    configuration.mq_curmsgs = 0;
    configuration.mq_maxmsg = max_messages;
    configuration.mq_msgsize = max_message_size;
    // Open the receiver's message queue
    message_q_descriptor = mq_open(message_queue_name.c_str(), O_CREAT | O_RDONLY, 0644, &configuration);
    if (message_q_descriptor == -1)
    {
        perror("mq_open");
        exit(1);
    }
}

MessageQueueReceiver::MessageQueueReceiver(std::string message_queue_name)
{

    configuration.mq_flags = 0;
    configuration.mq_curmsgs = 0;
    configuration.mq_maxmsg = MAX_MESSAGES;
    configuration.mq_msgsize = MAX_MESSAGE_SIZE;
    // Open the receiver's message queue
    message_q_descriptor = mq_open(message_queue_name.c_str(), O_CREAT | O_RDONLY, 0644, &configuration);
    if (message_q_descriptor == -1)
    {
        perror("mq_open");
        exit(1);
    }
}

MessageQueueReceiver::~MessageQueueReceiver()
{
    mq_close(message_q_descriptor);
    mq_unlink(this->message_queue_name);
    // while (AsyncActive != false)
    // {
    //     receiverThread.join();
    // }
}

std::string MessageQueueReceiver::receiveMessageSynchronous()
{
    char buffer[this->configuration.mq_msgsize];
    unsigned int prio;
    ssize_t bytes_received = mq_receive(this->message_q_descriptor, buffer, sizeof(buffer), &prio);
    if (bytes_received == -1)
    {
        perror("mq_receive");
        return std::string("Could not receive message");
    }
    else
    {
        buffer[bytes_received] = '\0';
        return std::string(buffer);
    }
}

void MessageQueueReceiver::_async_receive(void (*callback)(std::string))
{
    // TODO : DRY
    while (true)
    {
        // Receive a message from the sender
        char buffer[1024];
        unsigned int prio;
        std::cout << "Waiting for message" << std::endl;
        ssize_t bytes_received = mq_receive(this->message_q_descriptor, buffer, sizeof(buffer), &prio);
        if (bytes_received == -1)
        {
            perror("mq_receive async");
            return; // Terminate the thread on receive error
        }

        buffer[bytes_received] = '\0';
        callback(std::string(buffer));
        // std::cout << "Received message: " << buffer << std::endl;
    }
}

void MessageQueueReceiver::receiveMessageAsynchronous(void (*callback)(std::string), std::thread &receiverThread)
{
    receiverThread = std::thread(&MessageQueueReceiver::_async_receive, this, callback);
    // receiverThread.join();
}



// void printstring(std::string message)
// {
//     std::cout << "Received message: " << message << std::endl;
// }

// int main()
// {
//     // MessageQueueReceiver receiver("/my_message_queue");
//     MessageQueueReceiver receiver2("/my_message_queue", 10, 1024);
//     // std::string message = receiver2.receiveMessageSynchronous();
//     // std::cout << message << std::endl;

//     MessageQueueReceiver receiver("/my_message_queue");
//     std::thread receiverThread;
//     receiver.receiveMessageAsynchronous(printstring, receiverThread);
//     // receiver.receiveMessageAsynchronous([](std::string message) {
//     //     std::cout << "Received message: " << message << std::endl;
//     // }, receiverThread);

//     // receiver.receiveMessageAsynchronous([](std::string message) {
//     //     std::cout << "Received message: " << message << std::endl;
//     // });
//     std::cout<< "it really async"<<std::endl;
//     receiverThread.join();


//     return 0;
// }
















// int main()
// {
//     // Define message queue attributes
//     struct mq_attr attr;
//     attr.mq_flags = 0;
//     attr.mq_maxmsg = 10;
//     attr.mq_msgsize = 1024;
//     attr.mq_curmsgs = 0;

//     // Create a message queue
//     mqd_t mq = mq_open("/my_message_queue", O_CREAT | O_RDONLY, 0644, &attr);
//     if (mq == -1)
//     {
//         perror("mq_open");
//         return 1;
//     }

//     if (true)
//     {
//         // Receive a message from the sender
//         char buffer[1024];
//         unsigned int prio;
//         ssize_t bytes_received = mq_receive(mq, buffer, sizeof(buffer), &prio);
//         if (bytes_received == -1)
//         {
//             perror("mq_receive");
//             return 1;
//         }

//         buffer[bytes_received] = '\0';
//         std::cout << "Received message: " << buffer << std::endl;
//     }

//     // Close and unlink the message queue (this won't be reached in the loop)
//     mq_close(mq);
//     mq_unlink("/my_message_queue");

//     return 0;
// }

// // #include <iostream>
// // #include <mqueue.h>
// // #include <thread>
// // #include <chrono>

// void messageReceiver(mqd_t mq)
// {
//     while (true)
//     {
//         // Receive a message from the sender
//         char buffer[1024];
//         unsigned int prio;
//         ssize_t bytes_received = mq_receive(mq, buffer, sizeof(buffer), &prio);
//         if (bytes_received == -1)
//         {
//             perror("mq_receive");
//             return; // Terminate the thread on receive error
//         }

//         buffer[bytes_received] = '\0';
//         std::cout << "Received message: " << buffer << std::endl;
//     }
// }

// int main()
// {
//     // Define message queue attributes
//     struct mq_attr attr;
//     attr.mq_flags = 0;
//     attr.mq_maxmsg = 10;
//     attr.mq_msgsize = 1024;
//     attr.mq_curmsgs = 0;

//     // Create a message queue
//     mqd_t mq = mq_open("/my_message_queue", O_CREAT | O_RDONLY, 0644, &attr);
//     if (mq == -1)
//     {
//         perror("mq_open");
//         return 1;
//     }

//     // Create a separate thread for the message receiver
//     std::thread receiverThread(messageReceiver, mq);

//     // Perform other tasks in the main thread (you can replace this with any meaningful work)
//     for (int i = 0; i < 100; ++i)
//     {
//         std::cout << "Main thread working..." << std::endl;
//         std::this_thread::sleep_for(std::chrono::seconds(10));
//     }

//     // Close and unlink the message queue
//     mq_close(mq);
//     mq_unlink("/my_message_queue");

//     // Wait for the receiver thread to finish
//     receiverThread.join();

//     return 0;
// }
