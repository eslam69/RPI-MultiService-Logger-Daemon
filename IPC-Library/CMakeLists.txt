cmake_minimum_required(VERSION 3.10)

project(MsgQConnect)

set(CMAKE_CXX_STANDARD 11)


# Set the compiler to /home/eslam/x-tools/aarch64-rpi4-linux-gnu/bin/aarch64-rpi4-linux-gnu-g++
 set(CMAKE_CXX_COMPILER "/home/eslam/x-tools/aarch64-rpi4-linux-gnu/bin/aarch64-rpi4-linux-gnu-g++")
# or use : cmake -DCMAKE_CXX_COMPILER=path-to/aarch64-rpi4-linux-gnu-g++ ..


# Add the MsgQConnect library
add_library(MsgQConnect SHARED src/MsgQConnect.cpp)

# add_library(MsgQConnect STATIC src/MsgQConnect.cpp)

# Link the MsgQConnect library with the pthread and real-time libraries
target_link_libraries(MsgQConnect pthread rt)

# Install the MsgQConnect library to /usr/local/lib
install(TARGETS MsgQConnect DESTINATION /usr/local/lib)

#add headers to the system
target_include_directories(MsgQConnect PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#usage
# mkdir build
# cd build
# cmake -DCMAKE_CXX_COMPILER=/usr/bin/clang++ ..
# make
# sudo make install

#to use in any app 
#gcc
# g++ main.cpp -o main -L. -lMsgQConnect -lrt -pthread

# or in CMakeLists.txt
# target_link_libraries(main MsgQConnect pthread rt)




#to use in RPI 
# scp /path/to/libMsgQConnect.so pi@<ip_address>:/home/pi/
# ssh pi@<ip_address>
# sudo mv libMsgQConnect.so /usr/local/lib/
# sudo ldconfig
