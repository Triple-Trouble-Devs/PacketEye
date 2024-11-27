# 최소 요구 CMake 버전
cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름과 C++ 표준 설정
project(PacketEye)
set(CMAKE_CXX_STANDARD 14)

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

# 실행 파일 생성
add_executable(main ${PE_SOURCES} ${PE_HEADERS})

# Include 디렉토리 추가 (libpcap 헤더 파일 경로)
target_include_directories(main PRIVATE ${PE_INCLUDE_DIR} ${PE_SOURCE_DIR} /opt/homebrew/opt/libpcap/include)


# 라이브러리 링크 (libpcap 라이브러리)
target_link_libraries(main /opt/homebrew/opt/libpcap/lib/libpcap.dylib)
