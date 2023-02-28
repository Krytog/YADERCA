cmake_minimum_required(VERSION 3.16)
project(YADERCA C)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(YADERCA main.c
        UI/UI.c UI/BufferLine/UI_bufferl.c UI/LegendLine/UI_Legendl.c)

target_link_libraries(YADERCA ${CURSES_LIBRARIES})
