cmake_minimum_required(VERSION 3.23)
project(lab3 C)

set(CMAKE_C_STANDARD 99)

set(SOURCE_EXE labs/lab3/main.c)
set(SOURCE_LIB labs/lab3/lab3.c)


add_library(lab3 STATIC ${SOURCE_LIB})
add_executable(
        main ${SOURCE_EXE}
)

target_link_libraries(main lab3)
