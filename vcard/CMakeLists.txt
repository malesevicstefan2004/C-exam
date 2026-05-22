target_sources(${PROJECT_NAME}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include/vcard.h"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/vcard.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
