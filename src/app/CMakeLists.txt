# src/app/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


# 소스 및 UI 파일 정의
set(APP_SOURCES
    main.cpp
    mainwindow.cpp
)

set(APP_UI
    mainwindow.ui
)

# app 라이브러리 생성
add_library(app ${APP_SOURCES} ${APP_UI})

# include 디렉터리 추가
target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include/app
    ${CMAKE_BINARY_DIR}/src/app
)

# Qt 모듈과 core 라이브러리 연결
target_link_libraries(app PRIVATE core Qt6::Core Qt6::Widgets)

# UI 파일 처리
qt6_wrap_ui(UI_HEADERS ${APP_UI})
target_sources(app PRIVATE ${UI_HEADERS})