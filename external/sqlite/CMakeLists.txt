cmake_minimum_required(VERSION 3.20)

add_library(sqlite3 STATIC sqlite3.c)

target_include_directories(sqlite3 PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_compile_definitions(sqlite3 PUBLIC SQLITE_THREADSAFE=1)