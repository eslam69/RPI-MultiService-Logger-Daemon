#!/bin/bash
 g++ -c -fPIC sender.cpp -o sender.o
 g++ -shared -Wl,-soname,libsender.so -o libsender.so sender.o
 g++ -c -fPIC receiver.cpp -o receiver.o
 g++ -shared -Wl,-soname,libreceiver.so -o libreceiver.so receiver.o -lrt -pthread

sudo cp libsender.so /usr/local/lib
sudo cp libreceiver.so /usr/local/lib

ldconfig

echo "Done"
