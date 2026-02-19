cmake_minimum_required(VERSION 3.20)
project(medication_plan LANGUAGES C CXX)

add_subdirectory(external/sqlite)

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

add_executable(medication_plan ${APP_SOURCES})

target_include_directories(medication_plan PRIVATE src)

target_compile_features(medication_plan PRIVATE cxx_std_20)

target_link_libraries(medication_plan PRIVATE sqlite3)
