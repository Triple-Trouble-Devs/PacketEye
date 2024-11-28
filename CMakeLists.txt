# 최소 요구 CMake 버전
cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름과 C++ 표준 설정
project(packet-eye)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 찾기
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
qt_standard_project_setup()

add_subdirectory(src/app)

# 디렉토리 경로 설정
set(PE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(PE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(PE_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")


# 소스 파일 자동 수집
file(GLOB_RECURSE PE_SOURCES
    "${PE_SOURCE_DIR}/*.cpp"
    "${PE_SOURCE_DIR}/*.cxx"
    "${PE_SOURCE_DIR}/*.cc"
)

# 헤더 파일 자동 수집
file(GLOB_RECURSE PE_HEADERS
    "${PE_INCLUDE_DIR}/*.h"
    "${PE_INCLUDE_DIR}/*.hpp"
    "${PE_INCLUDE_DIR}/*.hxx"
)
