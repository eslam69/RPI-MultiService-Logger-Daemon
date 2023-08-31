# cmake_minimum_required(VERSION 3.10)

# project(LoggingDaemon)

# set(CMAKE_CXX_STANDARD 11)

# set(Boost_USE_STATIC_LIBS OFF)
# set(Boost_USE_MULTITHREADED ON)
# set(Boost_USE_STATIC_RUNTIME OFF)

# # Add the IPC-Library include directory
# include_directories(../../IPC-Library/include)

# # Add the IPC-Library library
# link_directories(../../IPC-Library/lib)

# # Add the Boost libraries
# # find_package(Boost REQUIRED COMPONENTS log log_setup system thread)
# find_package(Boost 1.80.0 REQUIRED COMPONENTS log log_setup system thread)
# set(Boost_LIBRARIES Boost::log Boost::log_setup Boost::system Boost::thread)
# # Add the daemon executable
# add_executable(daemon src/daemon.cpp)

# # Link against the MsgQConnect and Boost libraries
# # target_link_libraries(daemon MsgQConnect rt pthread Boost::log Boost::log_setup Boost::system Boost::thread)



# target_link_libraries(daemon MsgQConnect rt pthread ${Boost_LIBRARIES})



# cmake_minimum_required(VERSION 3.10)

# project(LoggingDaemon)

# set(CMAKE_CXX_STANDARD 11)

# # Specify the path to your cross-compiler toolchain file
# set(CMAKE_C_COMPILER /home/eslam/x-tools/aarch64-rpi4-linux-gnu/bin/aarch64-rpi4-linux-gnu-gcc)
# set(CMAKE_CXX_COMPILER /home/eslam/x-tools/aarch64-rpi4-linux-gnu/bin/aarch64-rpi4-linux-gnu-g++)

# # Specify the path to the cross-compiled Boost installation
# # set(BOOST_ROOT /mnt/Windows/boost_1_80_0/stage)
# set(Boost_USE_STATIC_LIBS OFF)
# set(Boost_USE_MULTITHREADED ON)
# set(Boost_USE_STATIC_RUNTIME OFF)

# # Add the IPC-Library include directory
# include_directories(../../IPC-Library/include)
# include_directories(/usr/include/boost)
# # Add the IPC-Library library

# # Add the Boost include and library directories
# # include_directories(/mnt/Windows/boost_1_80_0)
# # link_directories(/mnt/Windows/boost_1_80_0/stage/lib)

# # # Add the daemon executable
# add_executable(daemon src/daemon.cpp)

# # add_executable(daemon src/daemon.cpp)
# # find_package(Boost  REQUIRED COMPONENTS log log_setup system thread)
# find_package(Boost 1.80.0 REQUIRED COMPONENTS log log_setup system thread)
# # set(Boost_LIBRARIES Boost::log Boost::log_setup Boost::system Boost::thread)
# # # Link against the MsgQConnect and Boost libraries
# # target_link_libraries(daemon MsgQConnect rt pthread boost_log boost_log_setup boost_system boost_thread)

# find_library(MSGQCONNECT_LIB MsgQConnect PATHS ../../IPC-Library/build)

# # Add the daemon executable
# link_directories(../../IPC-Library/build)

# # Link against the MsgQConnect and Boost libraries
# # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath=/mnt/Windows/boost_1_80_0/stage/lib")
# target_link_libraries(daemon ${MSGQCONNECT_LIB} rt pthread boost_log boost_log_setup boost_system boost_thread)
# # set_target_properties(daemon PROPERTIES LINK_FLAGS "-Wl,-rpath=/mnt/Windows/boost_1_80_0/stage/lib")
# # target_link_libraries(daemon ${MSGQCONNECT_LIB} rt pthread Boost::log Boost::log_setup Boost::system Boost::thread)



cmake_minimum_required(VERSION 3.10)

project(LoggingDaemon)

set(CMAKE_CXX_STANDARD 11)

# Specify the path to your cross-compiler toolchain file
set(CMAKE_C_COMPILER /home/eslam/x-tools/aarch64-rpi4-linux-gnu/bin/aarch64-rpi4-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /home/eslam/x-tools/aarch64-rpi4-linux-gnu/bin/aarch64-rpi4-linux-gnu-g++)

# Add the IPC-Library include directory
include_directories(../../IPC-Library/include)

# Add the Boost include directory
include_directories(/usr/include/boost)

# Add the daemon executable
add_executable(daemon src/daemon.cpp)
set(CMAKE_PREFIX_PATH "/home/eslam/cross_compiled_boost/lib")
# Find the Boost libraries
find_package(Boost 1.80.0 REQUIRED COMPONENTS log log_setup system thread)
# set(Boost_LIBRARIES /usr/lib/libboost_log.so /usr/lib/libboost_log_setup.so /usr/lib/libboost_system.so /usr/lib/libboost_thread.so)
#print Boost_INCLUDE_DIRS
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
# Link against the MsgQConnect, Boost, and other required libraries
find_library(MSGQCONNECT_LIB MsgQConnect PATHS ../../IPC-Library/build)

# target_link_libraries(daemon ${MSGQCONNECT_LIB} rt pthread Boost::log Boost::log_setup Boost::system Boost::thread)
target_link_libraries(daemonge rt pthread ${Boost_LIBRARIES})
