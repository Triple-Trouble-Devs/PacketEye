# src/core/CMakeLists.txt

# 명시적으로 소스 파일 나열
set(CORE_SOURCES
    ether_header.cpp
    ip_header.cpp
    packet_capture.cpp
    tcp_header.cpp
)

# core 라이브러리 생성
add_library(core STATIC ${CORE_SOURCES})

# libpcap 헤더 경로 및 라이브러리 경로 설정 (brew로 설치된 경로 사용)
set(PCAP_INCLUDE_DIR /opt/homebrew/Cellar/libpcap/1.10.5/include)
set(PCAP_LIBRARY /opt/homebrew/Cellar/libpcap/1.10.5/lib/libpcap.dylib)

if (NOT PCAP_INCLUDE_DIR OR NOT PCAP_LIBRARY)
    message(FATAL_ERROR "libpcap not found. Please install it.")
endif()

# include 디렉터리 추가 (PUBLIC으로 설정해 다른 타겟에서 참조 가능)
target_include_directories(core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PCAP_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include/core
)

# libpcap 및 Qt Core 모듈 연결
target_link_libraries(core PUBLIC ${PCAP_LIBRARY} Qt6::Core)