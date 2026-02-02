cmake_minimum_required(VERSION 3.20)
project(medication_plan LANGUAGES C CXX)

add_executable(medication_plan
        src/main.cpp
        external/sqlite/sqlite3.c
)

target_compile_features(medication_plan PRIVATE cxx_std_20)

target_include_directories(medication_plan PRIVATE
        external/sqlite
)
