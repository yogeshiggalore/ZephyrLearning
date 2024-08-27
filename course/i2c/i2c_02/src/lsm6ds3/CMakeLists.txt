cmake_minimum_required(VERSION 3.13.1)

target_sources(app PRIVATE
    "lsm6ds3.c"
)

target_include_directories(app PUBLIC "./")


