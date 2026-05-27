cmake_minimum_required(VERSION 3.14)
project(AutoInsuranceSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Gather Source Files ---
# Our application sources
file(GLOB APP_SOURCES "*.cpp")

# --- Create Executable ---
add_executable(AutoInsuranceSystem ${APP_SOURCES})

# --- Include Directories ---
target_include_directories(AutoInsuranceSystem PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

