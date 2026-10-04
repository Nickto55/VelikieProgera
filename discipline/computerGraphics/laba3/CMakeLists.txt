cmake_minimum_required(VERSION 3.10)
project(ComputerGraphics_Laba3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Найти X11
find_package(X11 REQUIRED)

if(NOT X11_FOUND)
    message(FATAL_ERROR "X11 not found. Please install libx11-dev")
endif()

# Добавить исполняемый файл
add_executable(option_2 option_2.cpp)

# Подключить заголовочные файлы X11
target_include_directories(option_2 PRIVATE ${X11_INCLUDE_DIR})

# Подключить библиотеки X11
target_link_libraries(option_2 ${X11_LIBRARIES})

# Опции компиляции
target_compile_options(option_2 PRIVATE -Wall -Wextra)
