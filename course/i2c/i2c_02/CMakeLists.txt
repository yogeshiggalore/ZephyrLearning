cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(i2c_02)

target_sources(app PRIVATE src/main.c)

add_subdirectory("src/lsm6ds3")