#include "../include/MsgQConnect.hpp"
//uses pthread and rt libs

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
