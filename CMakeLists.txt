cmake_minimum_required(VERSION 3.10)

# Set some basic project attributes
project(vcard
    VERSION 1.0
    DESCRIPTION "vCard 3.0 Generator – DHBW TI25 Programming Exam")

# C++ standard
set(CMAKE_CXX_STANDARD 17)

# Compiler flags – same style as dhbwcppexamples
set(CMAKE_CXX_FLAGS "-Wall -Wfatal-errors -pipe")
set(CMAKE_CXX_FLAGS_DEBUG          "-Wextra -Wno-ignored-qualifiers -ggdb -Og -Werror")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -s -DNDEBUG")

if(CMAKE_BUILD_TYPE)
    message("Building with configuration '${CMAKE_BUILD_TYPE}'")
else(CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "No valid build configuration set (CMAKE_BUILD_TYPE=...). Valid options: Debug, RelWithDebInfo, Release, MinSizeRel")
endif(CMAKE_BUILD_TYPE)

# Doxygen (optional documentation)
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(doc_doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generating documentation with Doxygen"
        VERBATIM)
else(DOXYGEN_FOUND)
    message("Doxygen not found – documentation target will not be available")
endif(DOXYGEN_FOUND)

# Output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# Main executable
add_executable(${PROJECT_NAME} main.cpp)

# Subdirectories add their sources and include paths to the target
add_subdirectory(list)
add_subdirectory(vcard)
