set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.13)


set(COMMON_FLAGS "-ffunction-sections -fdata-sections -g -gdwarf-2 -fno-common -fmessage-length=0 ")

set(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -std=c++17")

set(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS} -std=c11")

# SET(CMAKE_EXE_LINKER_FLAGS_INIT "-Wl,-gc-sections,--print-memory-usage")


project(Host C CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(PARENT_DIR ${PROJECT_SOURCE_DIR}/../..)

set(TOPICS_DIR ${PARENT_DIR}/examples_rpx/topics)
set(MRPX_DIR ${PARENT_DIR}/machinerpx/include)
set(APPS_DIR ${PARENT_DIR}/examples_rpx/apps)

file(GLOB_RECURSE PROJECT_SOURCES "src/*.c" "src/*.cpp" "${APPS_DIR}/*.cpp")

include_directories(
    include
    ${APPS_DIR}
    ${TOPICS_DIR}
    ${MRPX_DIR}
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} 
pthread 
m
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND size --format=berkeley ${PROJECT_NAME})