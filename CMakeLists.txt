cmake_minimum_required(VERSION 3.20)
project(medication_plan LANGUAGES C CXX)

add_subdirectory(external/sqlite)

add_executable(medication_plan
        src/main.cpp
        src/infrastructure/db/database.cpp
        src/infrastructure/db/statement.cpp
        src/infrastructure/db/init_db.cpp
        src/domain/patient.hpp
        src/application/repository/patient_repository.cpp
        src/ui/cli/patient_printer.cpp
)

target_include_directories(medication_plan PRIVATE src)

target_compile_features(medication_plan PRIVATE cxx_std_20)

target_link_libraries(medication_plan PRIVATE sqlite3)
