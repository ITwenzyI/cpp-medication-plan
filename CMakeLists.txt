cmake_minimum_required(VERSION 3.20)
project(medication_plan LANGUAGES C CXX)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/sql/schema.sql
    ${CMAKE_CURRENT_BINARY_DIR}/schema.sql
    COPYONLY
)

add_subdirectory(external/sqlite)

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

list(REMOVE_ITEM CORE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

add_library(medication_core ${CORE_SOURCES})

target_include_directories(medication_core PUBLIC src)

target_compile_features(medication_core PUBLIC cxx_std_20)

target_link_libraries(medication_core PUBLIC sqlite3)

add_executable(medication_plan src/main.cpp)

target_link_libraries(medication_plan PRIVATE medication_core)


# =========== TESTS =============

add_executable(smoke_patients tests/smoke_patient_repository.cpp)

target_link_libraries(smoke_patients PRIVATE medication_core)

add_executable(smoke_medications tests/smoke_medication_repository.cpp)

target_link_libraries(smoke_medications PRIVATE medication_core)

add_executable(smoke_intake_plans tests/smoke_intake_plan_repository.cpp)

target_link_libraries(smoke_intake_plans PRIVATE medication_core)
