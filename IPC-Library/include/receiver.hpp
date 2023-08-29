#pragma once
#include <iostream>
#include <mqueue.h>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <chrono>

#define MAX_MESSAGE_SIZE 1024
#define MAX_MESSAGES 10

class MessageQueueReceiver
{
private:
    mqd_t message_q_descriptor;
    struct mq_attr configuration;
    char *message_queue_name;
    void _async_receive(void (*callback)(std::string));
    // std::thread receiverThread;

public:
    MessageQueueReceiver(std::string message_queue_name);
    MessageQueueReceiver(std::string message_queue_name, int max_messages, int max_message_size);

    ~MessageQueueReceiver();
    std::string receiveMessageSynchronous();
    void receiveMessageAsynchronous(void (*callback)(std::string), std::thread &thread);
};

