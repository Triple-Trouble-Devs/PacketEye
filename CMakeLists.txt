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

# 헤더 파일 검색 (include/core 및 include/app에서 모든 .h 파일 가져오기)
file(GLOB CORE_HEADERS include/core/*.h)
file(GLOB APP_HEADERS include/app/*.h)

# 모든 헤더 파일 합치기
set(HEADERS ${CORE_HEADERS} ${APP_HEADERS})

# Qt의 moc 처리
qt_wrap_cpp(MOC_SOURCES ${HEADERS})

# 하위 디렉터리 추가
add_subdirectory(src/app)
add_subdirectory(src/core)

# 실행 파일 생성
add_executable(packet-eye src/app/main.cpp ${MOC_SOURCES})

# 실행 파일에 하위 라이브러리와 Qt 모듈 연결
target_link_libraries(packet-eye PRIVATE app core Qt6::Core Qt6::Widgets)