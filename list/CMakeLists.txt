target_sources(${PROJECT_NAME}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include/list.h"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/list.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
