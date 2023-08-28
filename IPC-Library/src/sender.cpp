#include <iostream>
#include <mqueue.h>
#include <cstring>
#include <cstdlib>

int main(int argc, const char *argv[]) {
    // Open the receiver's message queue
    mqd_t mq = mq_open("/my_message_queue", O_WRONLY);
    if (mq == -1) {
        perror("mq_open");
        return 1;
    }
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <message>" << std::endl;
        return 1;
    }

    // Send a message to the receiver
    // const char *message = ;
    // send first argement as message
    const char *message = argv[1];
    if (mq_send(mq, message, strlen(message), 0) == -1) {
        perror("mq_send");
        return 1;
    }

    std::cout << "Message sent: " << message << std::endl;

    // Close the message queue
    mq_close(mq);

    return 0;
}
