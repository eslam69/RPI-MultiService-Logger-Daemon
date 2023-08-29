#!/bin/bash

#build shared library
g++ -c -fPIC MsgQConnect.cpp -o MsgQConnect.o

#link shared library
g++ -shared -Wl,-soname,libMsgQConnect.so -o libMsgQConnect.so MsgQConnect.o -lrt -pthread
sudo cp libMsgQConnect.so /usr/local/lib

ldconfig

echo "Done"
