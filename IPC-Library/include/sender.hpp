#pragma once
#include <iostream>
#include <mqueue.h>
#include <cstring>
#include <cstdlib>

class MessageQueueSender
{
public:
    MessageQueueSender(const char* message_queue_name);

    ~MessageQueueSender();

   bool send_message(const char* message);

private:
    mqd_t message_q_descriptor;
};
