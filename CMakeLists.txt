# BASIC PROJECT SETTINGS
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(ACE4Sim VERSION 0.1 LANGUAGES CXX)

# SET C++ STANDARD (REQUIRED BY ROOT)
set(CMAKE_CXX_STANDARD 17)

# FIND SOURCE FILES
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# CREATE EXECUTABLE
add_executable(ACE4Sim src/ACE4Sim.cpp ${SOURCES})

# LOCAL HEADER FILES
target_include_directories(ACE4Sim PUBLIC ${PROJECT_SOURCE_DIR}/include)

# FIND GEANT4
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
target_include_directories(ACE4Sim PRIVATE ${Geant4_INCLUDE_DIRS})
target_link_libraries(ACE4Sim ${Geant4_LIBRARIES})

# FIND ROOT
find_package(ROOT REQUIRED)
target_include_directories(ACE4Sim PRIVATE  ${ROOT_INCLUDE_DIRS})
target_link_libraries(ACE4Sim ${ROOT_LIBRARIES})

# specify our C++ compile flags
set(COMPILE_OPTIONS -Wall -Wextra -Wdisabled-optimization -fconcepts
  -pedantic -Wshadow -Wno-sign-conversion -Wno-sign-compare -Wold-style-cast
  -Wno-strict-overflow -Wundef -Wno-unused -Wstack-protector -Wcast-align
  -Wpacked -Wformat-y2k -Warray-bounds -Wreorder -Wmissing-braces
  -Wstrict-overflow=4 -Wwrite-strings -Wpointer-arith
  -Wcast-qual -Wswitch-enum -Wswitch-default -Wunreachable-code
  -Wmaybe-uninitialized -march=native -fmax-errors=5 -O2 -flto -g)
target_compile_options(ACE4Sim PUBLIC ${COMPILE_OPTIONS})
   
# and install the binary to the bin directory
install(TARGETS ACE4Sim DESTINATION ${PROJECT_SOURCE_DIR}/bin
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
