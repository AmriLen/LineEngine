# CMakeList.txt: проект CMake для LineEngineCore; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.

cmake_minimum_required(VERSION 3.16)

set(ENGINE_PROJECT_NAME LineEngineCore)

# Include публичное подключение
set(ENGINE_PUBLIC_INCLUDES
    includes/LineEngineCore/Application.hpp
    includes/LineEngineCore/Event.hpp
)

set(ENGINE_PRIVATE_INCLUDES
    src/LineEngineCore/Window.hpp
    src/LineEngineCore/Rendering/OpenGL/ShaderProgram.hpp
)

set(ENGINE_PRIVATE_SOURCES
    src/LineEngineCore/Application.cpp
    src/LineEngineCore/Window.cpp
    src/LineEngineCore/Rendering/OpenGL/ShaderProgram.cpp
)

set(ENGINE_ALL_SOURCES
    ${ENGINE_PUBLIC_INCLUDES}
    ${ENGINE_PRIVATE_INCLUDES}
    ${ENGINE_PRIVATE_SOURCES}
)

add_library(${ENGINE_PROJECT_NAME} STATIC
    ${ENGINE_ALL_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
    ${ENGINE_ALL_SOURCES}
)


target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
target_include_directories(${ENGINE_PROJECT_NAME} PRIVATE src)
target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)

# Подключение Subdirectory
# Связка библиотеки с LineEngineCore
add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glfw)

add_subdirectory(../external/alplogger ${CMAKE_CURRENT_BINARY_DIR}/alplogger)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE alplogger)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glad)


# Связка IMGUI с LineEngineCore
set(IMGUI_INCLUDES
    ../external/imgui/imgui.h
    ../external/imgui/backends/imgui_impl_glfw.h
    ../external/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
    ../external/imgui/imgui.cpp
    ../external/imgui/backends/imgui_impl_glfw.cpp
    ../external/imgui/backends/imgui_impl_opengl3.cpp
    ../external/imgui/imgui_demo.cpp
    ../external/imgui/imgui_draw.cpp
    ../external/imgui/imgui_tables.cpp
    ../external/imgui/imgui_widgets.cpp
)

add_library(ImGui STATIC
    ${IMGUI_INCLUDES}
    ${IMGUI_SOURCES}
)

target_include_directories(ImGui PUBLIC ../external)
target_include_directories(ImGui PUBLIC ../external/imgui)
target_link_libraries(ImGui PRIVATE glad glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE ImGui)