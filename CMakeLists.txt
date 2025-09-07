# Минимальная требуемая версия CMake
cmake_minimum_required(VERSION 3.18)

# Определение проекта (название, языки)
project(VelikieProgera C)

# Установка стандарта языка C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# --- Управление зависимостями через vcpkg ---
# Находим пакет 'fmt', который был объявлен в vcpkg.json
# CONFIG REQUIRED - означает, что если пакет не найден, CMake выдаст ошибку.
find_package(fmt CONFIG REQUIRED)

# --- Определение целей сборки (исполняемых файлов) ---

# Добавляем исполняемый файл для лабораторной работы по основам программирования
add_executable(
    prog_basics_lab1_opt19 
    discipline/programmingBasics/laba1/option_19.c
)

# "Прилинковываем" (подключаем) библиотеку fmt к нашей программе.
# Теперь внутри option_19.c можно использовать #include <fmt/core.h>
target_link_libraries(prog_basics_lab1_opt19 PRIVATE fmt::fmt)

# Вы можете добавлять другие исполняемые файлы по аналогии:
# add_executable(
#     graphics_lab1_opt1
#     discipline/computerGraphics/laba1/option_1.c
# )
# target_link_libraries(graphics_lab1_opt1 PRIVATE fmt::fmt)