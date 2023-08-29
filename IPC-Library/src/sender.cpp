

#include "../include/sender.hpp"
//compile app: g++ sender.cpp -o sender -lrt

//compile lib:
// g++ -c -fPIC sender.cpp -o sender.o
// g++ -shared -Wl,-soname,libsender.so -o libsender.so sender.o

MessageQueueSender::MessageQueueSender(const char *message_queue_name)
{
    // Open the receiver's message queue
    message_q_descriptor = mq_open(message_queue_name, O_WRONLY);
    if (message_q_descriptor == -1)
    {
        perror("mq_open");
        exit(1);
    }
}

MessageQueueSender::~MessageQueueSender()
{
    // Close the message queue
    mq_close(message_q_descriptor);
}

bool MessageQueueSender::send_message(const char *message)
{
    // Send a message to the receiver
    if (mq_send(message_q_descriptor, message, strlen(message), 0) == -1)
    {
        perror("mq_send");
        // exit(1);
        return false;
    }
    else
    {
        return true;
    }
}

// compile with:
// g++ sender.cpp -o sender -lrt
#include <iostream>
#include <mqueue.h>
#include <cstring>
#include <cstdlib>

// int main(int argc, const char *argv[])
// {
//     // // Open the receiver's message queue
//     // mqd_t mq = mq_open("/my_message_queue", O_WRONLY);
//     // if (mq == -1)
//     // {
//     //     perror("mq_open");
//     //     return 1;
//     // }
//     // if (argc < 2)
//     // {
//     //     std::cerr << "Usage: " << argv[0] << " <message>" << std::endl;
//     //     return 1;
//     // }

//     // // Send a message to the receiver
//     // // const char *message = ;
//     // // send first argement as message
//     // const char *message = argv[1];
//     // if (mq_send(mq, message, strlen(message), 0) == -1)
//     // {
//     //     perror("mq_send");
//     //     return 1;
//     // }

//     // std::cout << "Message sent: " << message << std::endl;

//     // // Close the message queue
//     // mq_close(mq);

//     // return 0;
//     MessageQueueSender sender("/my_message_queue");
//     if (argc < 2)
//     {
//         std::cerr << "Usage: " << argv[0] << " <message>" << std::endl;
//         return 1;
//     }
//     const char *message = argv[1];
//     if (sender.send_message(message))
//     {
//         std::cout << "Message sent: " << message << std::endl;
//     }
//     else
//     {
//         std::cerr << "Message not sent: " << message << std::endl;
//     }
//     return 0;
    
// }
