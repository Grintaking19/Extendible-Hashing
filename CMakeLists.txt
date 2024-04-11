cmake_minimum_required(VERSION 3.5.0)   # Selects the minimum version of CMake required to run this file
project(Extendiable-Hashing VERSION 0.1.0)          # Here we select the project name and version

# Here we select C++17 with all the standards required and all compiler-specific extensions disabled
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(main.cpp)        # Here we add the executable file to be compiled
target_sources(main.cpp PRIVATE main.cpp ExtendiableHashing.cpp ExtendiableHashing.h)   # Here we add the source files to be compiled
install(TARGETS main.cpp DESTINATION bin)   # Here we install the executable file to the bin directory