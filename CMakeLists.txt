cmake_minimum_required(VERSION 3.10)
project(mqtt)

set(CMAKE_CXX_STANDARD 11)
find_library(wiringPi_LIB wiringPi)
find_package(Threads)

add_executable(mqtt main.cpp UserMQTTClient.cpp UserMQTTClient.h)
TARGET_LINK_LIBRARIES(mqtt paho-mqtt3c jsoncpp ${wiringPi_LIB} ${CMAKE_THREAD_LIBS_INIT} rt crypt)