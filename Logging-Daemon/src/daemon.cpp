// rm ../logs/*.log ; g++ -o daemon daemon.cpp -I../../IPC-Library/include -lMsgQConnect -lrt -lpthread -lboost_log -lboost_log_setup -lboost_system  -lboost_thread -DBOOST_ALL_DYN_LINK  && ./daemon

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <iomanip>
#include <chrono>
#include <ctime>
#include <sstream>
#include <thread>

#include <boost/log/attributes.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/lock_guard.hpp>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "../../IPC-Library/include/MsgQConnect.hpp"

#define MAX_FILE_LINES 1000
#define ASYNC 1

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;

const char *serverAddress ;

enum class SeverityLevel
{
    trace,
    debug,
    info,
    warning,
    error,
    fatal
};

void init_logging();
void parseConfigFile(const std::string &filename, std::map<std::string, std::string> &configMap);
void printConfigMap(const std::map<std::string, std::string> &configMap);
void log_message(SeverityLevel severity, std::string message);
void test_logging();
void log_async();
void log_sync();

inline std::string get_timestamp();
inline std::string format_message(std::string const &s, std::string const &app_name);
void parse_message(std::string received_message, SeverityLevel &severityLevel, std::string &literal_message);

static std::atomic<int> log_counter(1);

class Client
{
public:
    Client(const char *serverAddress, int port) : serverAddress(serverAddress), port(port), clientSocket(-1) {}

    bool connectToServer()
    {
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1)
        {
            std::cerr << "Error creating socket" << std::endl;
            return false;
        }

        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = inet_addr(serverAddress);

        if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
        {
            std::cerr << "Error connecting" << std::endl;
            return false;
        }
        return true;
    }

    bool sendData(const char *data)
    {
        ssize_t bytesSent = send(clientSocket, data, strlen(data), 0);
        if (bytesSent == -1)
        {
            std::cerr << "Error sending data" << std::endl;
            return false;
        }
        return true;
    }

    void closeConnection()
    {
        close(clientSocket);
    }

private:
    const char *serverAddress;
    int port;
    int clientSocket;
};

// function to return the current time as a string
inline std::string get_timestamp()
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    auto now_tm = std::localtime(&now_c);

    std::ostringstream timestamp;
    timestamp << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");

    return timestamp.str() + "." + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000);
}
// function to return the formatted log message
inline std::string format_message(std::string const &s, std::string const &app_name)
{

    std::ostringstream oss;

    oss << "[" << std::setfill('0') << std::setw(4) << log_counter << "] "
        << "[" << std::setfill(' ') << std::setw(15) << std::left << app_name << "] "
        << "[" << std::setfill('0') << std::setw(23) << std::left << get_timestamp() << "] " << s;

    return oss.str();
}

void init_logging()
{
    // Create a file sink with rotation based on line count
    typedef sinks::text_file_backend file_backend_t;
    typedef sinks::synchronous_sink<file_backend_t> sink_t;
    boost::shared_ptr<file_backend_t> file_backend = boost::make_shared<file_backend_t>(
        keywords::file_name = "../logs/%Y-%m-%d_%H-%M-%S.log",
        // keywords::rotation_size = 10,  // Rotation based on line count, not file size
        keywords::rotation_size = 1024,
        keywords::auto_flush = true);
    boost::shared_ptr<sink_t> file_sink = boost::make_shared<sink_t>(file_backend);

    // Add a formatter to include a timestamp and severity in each log message
    logging::formatter formatter = expr::stream
                                   // << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
                                   //    << "%H-%M-%S"
                                   << "[" << std::setfill(' ') << std::setw(7) << std::left << logging::trivial::severity << "] "
                                   << expr::smessage;
    file_sink->set_formatter(formatter);

    // Add the sink to the logging core
    logging::core::get()->add_sink(file_sink);
}

void parseConfigFile(const std::string &filename, std::map<std::string, std::string> &configMap)
{
    std::ifstream configFile(filename);
    if (!configFile.is_open())
    {
        std::cerr << "Error opening configuration file." << "can't find file in "<< filename<<std::endl;
        exit(1);
        return;
    }

    std::string line;
    while (std::getline(configFile, line))
    {
        if (!line.empty() && line[0] != '#')
        { // Ignore empty lines and comments
            size_t delimiterPos = line.find('=');
            if (delimiterPos != std::string::npos)
            {
                std::string key = line.substr(0, delimiterPos);
                std::string value = line.substr(delimiterPos + 1);
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);
                configMap[key] = value;
            }
        }
    }

    configFile.close();
}

void printConfigMap(const std::map<std::string, std::string> &configMap)
{
    for (const auto &pair : configMap)
    {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }
}
void send_log_to_server(std::string message)
{
    Client client(serverAddress, 12345);
    
    if (!client.connectToServer())
    {
        // exit(1);
        return ;
    }

    if (client.sendData(message.c_str()))
    {
        std::cout << "Socket Sent: " << message << std::endl;
    }
    client.closeConnection();
}
// prpend severity like [info] message
std::string prepend_severity(const std::string &message, SeverityLevel severity)
{
    std::string new_message;
    switch (severity)
    {
    case SeverityLevel::trace:
        new_message = "[trace] " + message;
        break;
    case SeverityLevel::debug:
        new_message = "[debug] " + message;
        break;
    case SeverityLevel::info:
        new_message = "[info] " + message;
        break;
    case SeverityLevel::warning:
        new_message = "[warning] " + message;
        break;
    case SeverityLevel::error:
        new_message = "[error] " + message;
        break;
    case SeverityLevel::fatal:
        new_message = "[fatal] " + message;
        break;
    default:
        new_message = "[info] " + message;
        break;
    }
    return new_message;
}

void log_message(SeverityLevel severity, std::string message)
{

    if (log_counter > MAX_FILE_LINES)
    {

        logging::core::get()->remove_all_sinks();
        init_logging();
        log_counter = 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(900));
    }
    log_counter++;
    // try to send the message to the server
    while (true)
    {
        try
        {
            send_log_to_server(prepend_severity(message, severity));
            break;
        }
        catch (const std::exception &e)
        {
            std::cerr <<"TCP Client is not Connected" <<e.what() << '\n';
            sleep(3);
        }
    }

    switch (severity)
    {
    case SeverityLevel::trace:
        BOOST_LOG_TRIVIAL(trace) << message;
        break;
    case SeverityLevel::debug:
        BOOST_LOG_TRIVIAL(debug) << message;
        break;
    case SeverityLevel::info:
        BOOST_LOG_TRIVIAL(info) << message;
        break;
    case SeverityLevel::warning:
        BOOST_LOG_TRIVIAL(warning) << message;
        break;
    case SeverityLevel::error:
        BOOST_LOG_TRIVIAL(error) << message;
        break;
    case SeverityLevel::fatal:
        BOOST_LOG_TRIVIAL(fatal) << message;
        break;
    default:
        BOOST_LOG_TRIVIAL(info) << message;
        break;
    }
}

void test_logging()
{
    for (int i = 1; i <= MAX_FILE_LINES * 3 + 4; i++)
    {
        // BOOST_LOG_TRIVIAL(trace) << message(std::to_string(i));
        log_message(SeverityLevel::info, format_message(std::to_string(i), "test"));
        // sleep(1);
    }
}
// create thread for each receiver
std::map<std::string, std::thread *> receiver_threads;
// create receiver for each app
std::map<std::string, MessageQueueReceiver *> receivers;

void parse_message(std::string received_message, SeverityLevel &severityLevel, std::string &literal_message)
{

    // message is int the form severty level | message
    // e.g. Error| this is a message
    // so we need to split the message into severity level and message and strip them
    // and then log it
    std::string severity = received_message.substr(0, received_message.find("|"));
    // strip the severity level from extra spaces
    severity.erase(0, severity.find_first_not_of(" \t"));
    severity.erase(severity.find_last_not_of(" \t") + 1);

    literal_message = received_message.substr(received_message.find("|") + 1);
    // std::cout << "severity:" << severity
    //           << "message:" << literal_message << std::endl;
    // check corresponding enum value
    if (severity == "Trace")
    {
        severityLevel = SeverityLevel::trace;
    }
    else if (severity == "Debug")
    {
        severityLevel = SeverityLevel::debug;
    }
    else if (severity == "Info")
    {
        severityLevel = SeverityLevel::info;
    }
    else if (severity == "Warning")
    {
        severityLevel = SeverityLevel::warning;
    }
    else if (severity == "Error")
    {
        severityLevel = SeverityLevel::error;
    }
    else if (severity == "Fatal")
    {
        severityLevel = SeverityLevel::fatal;
    }
    else
    {
        severityLevel = SeverityLevel::info;
    }
}

void callback(std::string received_message)
{
    // log_message(SeverityLevel::info, message(received_message, "testSendd"));
    // get current thread id
    std::thread::id this_id = std::this_thread::get_id();
    // get the name of the receiver app whose thread is the current thread
    std::string app_name;
    for (auto &pair : receiver_threads)
    {
        if (pair.second->get_id() == this_id)
        {
            app_name = pair.first;
            break;
        }
    }
    SeverityLevel severityLevel;
    std::string literal_message;

    parse_message(received_message, severityLevel, literal_message);

    log_message(severityLevel, format_message(literal_message, app_name));
}

void log_async()
{
    // MessageQueueReceiver receiver("/my_queue");

    // receive message from each app asynchronously

    // Receive a message from the queue synchronously
    for (auto &pair : receivers)
    {
        std::thread *receiver_thread = new std::thread();
        receiver_threads[pair.first] = receiver_thread;
    }
    while (true)
    {
        // receive message from each app
        for (auto &pair : receivers)
        {
            pair.second->receiveMessageAsynchronous(callback, *receiver_threads[pair.first]);
        }
        // join all threaads
        for (auto &pair : receiver_threads)
        {
            pair.second->join();
        }
    }
    // free memory

    for (auto &pair : receiver_threads)
    {
        delete pair.second;
    }
}

void log_sync()
{

    std::cout << "log sync\n";
    while (true)
    {
        // receive message from each app
        for (auto &pair : receivers)
        {
            std::string received_message = pair.second->receiveMessageSynchronous();
            // std::cout << "from app: " << pair.first << "\t"
            //           << "Received message: " << received_message << std::endl;
            // log_message(SeverityLevel::info, format_message(received_message, pair.first));

            SeverityLevel severityLevel;
            std::string literal_message;

            parse_message(received_message, severityLevel, literal_message);

            log_message(severityLevel, format_message(literal_message, pair.first));
        }
    }
}

int main(int argc, char **argv)
{   
    //get server address
    serverAddress = argv[1];  // "192.168.1.20" or "127.0.0.1"
    std::cout << "server address: " << serverAddress << std::endl;

    init_logging();


    std::map<std::string, std::string> configMap;
    parseConfigFile("/etc/config.txt", configMap);
    std::cout << "listenning.....\n";
    // test_logging();

    // create receivers for each app in the config file
    for (const auto &pair : configMap)
    {
        std::cout << "App: " << pair.first << ", Msg Q name: " << pair.second << std::endl;
        receivers[pair.first] = new MessageQueueReceiver(pair.second);
    }

#if (ASYNC == 1)
    log_async();
#else
    log_sync();
#endif

    for (auto &pair : receivers)
    {
        delete pair.second;
    }

    return 0;
}
