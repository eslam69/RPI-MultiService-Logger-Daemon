#include <iostream>
#include <mqueue.h>
#include <cstdlib>
#include <cstring>

int main() {
    // Define message queue attributes
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 1024;
    attr.mq_curmsgs = 0;

    // Create a message queue
    mqd_t mq = mq_open("/my_message_queue", O_CREAT | O_RDONLY, 0644, &attr);
    if (mq == -1) {
        perror("mq_open");
        return 1;
    }

    while (true) {
        // Receive a message from the sender
        char buffer[1024];
        unsigned int prio;
        ssize_t bytes_received = mq_receive(mq, buffer, sizeof(buffer), &prio);
        if (bytes_received == -1) {
            perror("mq_receive");
            return 1;
        }

        buffer[bytes_received] = '\0';
        std::cout << "Received message: " << buffer << std::endl;
    }

    // Close and unlink the message queue (this won't be reached in the loop)
    mq_close(mq);
    mq_unlink("/my_message_queue");

    return 0;
}
