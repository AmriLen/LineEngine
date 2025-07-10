# CMakeList.txt: проект CMake для LineEngineEditor; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.

cmake_minimum_required(VERSION 3.16)

set(EDITOR_PROJECT_NAME LineEngineEditor)

add_executable(${EDITOR_PROJECT_NAME}
  src/main.cpp
)

target_link_libraries(${EDITOR_PROJECT_NAME} LineEngineCore)
target_compile_features(${EDITOR_PROJECT_NAME} PUBLIC cxx_std_17)

# Установка исходного билда
set_target_properties(${EDITOR_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
