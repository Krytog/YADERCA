cmake_minimum_required(VERSION 3.16)
project(YADERCA C)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_compile_options(-fsanitize=address)
add_compile_options(-ldl)
add_link_options(-fsanitize=address)

add_executable(YADERCA main.c
        UI/UI.c UI/BufferLine/UI_bufferl.c UI/LegendLine/UI_Legendl.c UI/Body/UI_body.c
        Manager/InputHandler/InputHandler.c Manager/Traveler/traveler.c Manager/InfoHolder/InfoHolder.c
        Manager/PathHolder/PathHolder.c Manager/InfoUtilities/InfoUtilities.c
        Manager/OpenFilesUtilities/OpenFilesUtilities.c)

target_link_libraries(YADERCA ${CURSES_LIBRARIES})
