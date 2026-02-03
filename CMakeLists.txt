cmake_minimum_required(VERSION 3.20)
project(medication_plan LANGUAGES C CXX)

add_subdirectory(external/sqlite)

add_executable(medication_plan
        src/main.cpp
)

target_compile_features(medication_plan PRIVATE cxx_std_20)

target_link_libraries(medication_plan PRIVATE sqlite3)
