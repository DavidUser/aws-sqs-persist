cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Release)
list(APPEND CMAKE_PREFIX_PATH "/home/david/tests/aws/sdk/c++/aws-sdk-cpp/build/out")
set(CMAKE_CXX_STANDARD 20)
project(aws-sqs-persist LANGUAGES CXX)

find_package(AWSSDK COMPONENTS sqs dynamodb)

add_executable(${PROJECT_NAME} "./main.cc")
target_link_libraries(${PROJECT_NAME} PUBLIC ${AWSSDK_LINK_LIBRARIES})
