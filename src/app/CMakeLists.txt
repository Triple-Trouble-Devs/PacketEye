cmake_minimum_required(VERSION 3.16)

project(packet-eye VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_OSX_ARCHITECTURES "arm64")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 모듈 찾기
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# 실행 파일 정의
qt_add_executable(packet-eye
    mainwindow.ui
    mainwindow.cpp
    main.cpp
)

# Include 디렉토리 추가 (libpcap 헤더 파일 경로)
target_include_directories(packet-eye PRIVATE
    ${PE_INCLUDE_DIR}   # 필요한 경우에만 사용
    ${PE_SOURCE_DIR}    # 필요한 경우에만 사용
    /opt/homebrew/opt/libpcap/include
)

# libpcap 찾기
find_library(PCAP_LIB pcap REQUIRED PATHS
    /opt/homebrew/opt/libpcap/lib
    /usr/lib
    /usr/local/lib
)

# 라이브러리 링크
target_link_libraries(packet-eye PRIVATE
    ${PCAP_LIB}         # libpcap 추가
    Qt6::Widgets        
)

# 플랫폼별 속성 설정
set_target_properties(packet-eye PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)