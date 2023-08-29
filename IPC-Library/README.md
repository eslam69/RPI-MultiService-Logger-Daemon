# MsgQConnect

MsgQConnect is a C++ library for inter-process communication using POSIX message queues. It provides a simple interface for sending and receiving messages between processes. With MsgQConnect, you can create message queue receivers and senders, and use them to send and receive messages between processes. MsgQConnect supports both synchronous and asynchronous receiving of messages, allowing you to choose the best approach for your use case.

cout


## Requirements

- C++11 compiler
- pthread library
- real-time library

## Installation

To install `MsgQConnect`, follow these steps:

1. Clone the repository.


2. Build the library:

```
cd IPC-Library
mkdir build
cd build
cmake ..
make
```

3. Install the library:

```
sudo make install
```
## API

### `MessageQueueReceiver`

#### `MessageQueueReceiver(const char* queue_name)`

Creates a new `MessageQueueReceiver` object with the specified message queue name.

##### Parameters

- `queue_name`: The name of the message queue to receive messages from.

#### `std::string receiveMessageSynchronous()`

Receives a message from the message queue synchronously.

##### Returns

The received message as a `std::string`.

#### `void receiveMessageAsynchronous(std::function<void(std::string)> callback, std::thread& receiver_thread)`

Receives messages from the message queue asynchronously.

##### Parameters

- `callback`: A function to be called for each received message. The function should take a single `std::string` parameter representing the received message.
- `receiver_thread`: A `std::thread` object that will be used to run the receiver loop. This object should be joined later to wait for the receiver loop to finish.

### `MessageQueueSender`

#### `MessageQueueSender(const char* queue_name)`

Creates a new `MessageQueueSender` object with the specified message queue name.

##### Parameters

- `queue_name`: The name of the message queue to send messages to.

#### `void send_message(const char* message)`

Sends a message to the message queue.

##### Parameters

- `message`: The message to send, as a null-terminated C string.


## Usage

To use `MsgQConnect` in your project, you need to link your project with the `MsgQConnect` library and the pthread and real-time libraries. Here's an example of how you can compile a program that uses `MsgQConnect`:

```
g++ main.cpp -o main -L. -lMsgQConnect -lrt -pthread
```

Alternatively, if you are using CMake, you can link your project with `MsgQConnect` using the `target_link_libraries` command:

```cmake
target_link_libraries(myapp MsgQConnect pthread rt)
```

## Example

Here's an example of how you can use `MsgQConnect` to send and receive messages:

```c++
#include <iostream>
#include <string>
#include <thread>
#include "MsgQConnect.hpp"

void callback(std::string message)
{
    std::cout << "Received message: " << message << std::endl;
}

int main()
{
    // Create a message queue receiver with the name "my_queue"
    MessageQueueReceiver receiver("my_queue");

    // Receive a message from the queue synchronously
    std::string received_message = receiver.receiveMessageSynchronous();
    std::cout << "Received message: " << received_message << std::endl;

    // Receive messages from the queue asynchronously
    std::thread receiver_thread;
    receiver.receiveMessageAsynchronous(callback, receiver_thread);

    // Create a message queue sender with the name "my_queue"
    MessageQueueSender sender("my_queue");

    // Send a message to the queue
    std::string message = "Hello, world!";
    sender.send_message(message.c_str());

    // Wait for the receiver thread to finish
    receiver_thread.join();

    return 0;
}
```
