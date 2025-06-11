cmake_minimum_required(VERSION 3.10)
project(Angela)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# PCA9685
find_package(PiPCA9685 REQUIRED)

add_executable(${PROJECT_NAME} 
    apps/hello_world.cpp
)

target_link_libraries(${PROJECT_NAME} pigpio pthread PiPCA9685::PiPCA9685)
