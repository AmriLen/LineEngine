# CMakeList.txt: проект CMake для LineEngine; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.

cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME LineEngine)

# Парсинг дополнительных директорий
add_subdirectory(LineEngineCore)
add_subdirectory(LineEngineEditor)

# Установка для стартового проекта
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT LineEngineEditor)