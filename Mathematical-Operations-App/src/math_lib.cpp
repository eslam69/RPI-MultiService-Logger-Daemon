// #include "../include/math_lib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include "../../IPC-Library/include/MsgQConnect.hpp"

bool is_number(const std::string &str)
{
    try
    {
        double num = std::stod(str);
        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}

// read calculation from the user commands, where app arg1 op arg2
int main(int argc, char *argv[])
{

    // Create a message queue sender with the name "/math_queue"
    MessageQueueSender logClientsender("/math_queue");
    // printf("N args: %d\n", argc);
    // log app started
    logClientsender.send_message("Info | App started, Connected to the logger!");

    // define --help arg
    // ops are  + - * /
    if (argc == 2 && strcmp(argv[1], "--help") == 0)
    {
        logClientsender.send_message("Info | User requested help");
        printf("Usage: %s arg1 op arg2\n", argv[0]);
        printf("arg1: float number\n");
        printf("arg2: float number\n");
        printf("op: +, -, x, s\n");
        printf("+: addition\n");
        printf("-: subtraction\n");
        printf("x: multiplication\n");
        printf("/: division\n");

        return 0;
    }
    if (argc != 4)
    {
        logClientsender.send_message("Error | Invalid number of arguments");
        printf("Invalid number of arguments\n");
        // print see help
        printf("see help with %s --help\n", argv[0]);
        return 1;
    }

    // check if the args are numbers
    if (!is_number(argv[1]) || !is_number(argv[3]))
    {
        logClientsender.send_message("Error | Invalid arguments");
        printf("Invalid arguments\n");
        printf("see help with %s --help\n", argv[0]);
        return 1;
    }

    double arg1 = atof(argv[1]);
    double arg2 = atof(argv[3]);
    char op = argv[2][0];
    std::string operand = std::string(1, op);
    // send inputs to the logger
    // logClientsender.send_message((std::string("Info | Inputs: ") + std::to_string(arg1) + std::string(" ") + operand + std::string(" ") + std::to_string(arg2)).c_str());
    logClientsender.send_message((std::string("Info | Inputs: ") + std::string(argv[1]) + std::string(" ") + operand + std::string(" ") + std::string(argv[3])).c_str());

    if (op != '+' && op != '-' && op != 'x' && op != '/')
    {
        logClientsender.send_message((std::string("Error | Invalid operation: ") + std::to_string(op)).c_str());
        printf("Invalid operation\n");
        printf("see help with %s --help\n", argv[0]);

        return 1;
    }

    // check if arg1 and arg2 are not numbers

    double result = 0;
    switch (op)
    {
    case '+':
        logClientsender.send_message("Info | Addition operation");
        result = arg1 + arg2;
        break;
    case '-':
        logClientsender.send_message("Info | Subtraction operation");
        result = arg1 - arg2;
        break;
    case 'x':
        logClientsender.send_message("Info | Multiplication operation");
        result = arg1 * arg2;
        break;
    case '/':
        logClientsender.send_message("Info | Division operation");
        if (arg2 == 0)
        {
            logClientsender.send_message("Fatal | Division by zero");
            printf("Division by zero\n");
            return 1;
        }
        result = arg1 / arg2;
        break;

    default:
        auto message = std::string("Error | Invalid operation") + std::to_string(op);
        logClientsender.send_message((std::string("Error | Invalid operation") + std::to_string(op)).c_str());
        printf("Invalid operation\n");
        printf("see help with %s --help\n", argv[0]);
        return 1;
    }
    // log the total operation arg1 op arg2 = result
    logClientsender.send_message((std::string("Info | Operation: ") + std::string(argv[1]) + std::string(" ") + operand + std::string(" ") + std::string(argv[3]) + std::string(" = ") + std::to_string(result)).c_str());
    std::cout << (std::string("Info | Result: ") + std::string(argv[1]) + std::string(" ") + operand + std::string(" ") + std::string(argv[3]) + std::string(" = ") + std::to_string(result)).c_str() << std::endl;
    // log app termination
    logClientsender.send_message("Info | App terminated Successfully!");
    return 0;
}