#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <imgui_internal.h>

#include "Logger.h"
#include "../engine/LineEngine.h"

#include <iostream>

// Версия редактора
#define LINEENGINE_EDITOR_NAME      "LineEngine Editor"
#define LINEENGINE_EDITOR_VERSION   "v0.0.1"
#define LINEENGINE_COMPANY_NAME     "AFTERLINE Production"
#define THIS_PROG                   "[LineEngineEditor] "

static bool show_viewport = true;
static bool show_grid = true;
static bool show_outliner = true;
static bool show_properties = true;


struct ViewportRenderTarget {
    GLuint framebuffer = 0;
    GLuint colorTexture = 0;
    GLuint depthRenderbuffer = 0;
    int width = 0;
    int height = 0;
};

void CreateRenderTarget(ViewportRenderTarget& target, int width, int height) {
    target.width = width;
    target.height = height;

    glGenTextures(1, &target.colorTexture);
    glBindTexture(GL_TEXTURE_2D, target.colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenRenderbuffers(1, &target.depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, target.depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    glGenFramebuffers(1, &target.framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, target.framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, target.colorTexture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, target.depthRenderbuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShowViewportWindow(ViewportRenderTarget& target, bool* isOpen) {
    if (!*isOpen) return;

    ImGui::Begin("Viewport", isOpen, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImVec2 size = ImGui::GetContentRegionAvail();
    int newWidth = static_cast<int>(size.x);
    int newHeight = static_cast<int>(size.y);

    if (newWidth > 0 && newHeight > 0 && (newWidth != target.width || newHeight != target.height)) {
        glDeleteTextures(1, &target.colorTexture);
        glDeleteRenderbuffers(1, &target.depthRenderbuffer);
        glDeleteFramebuffers(1, &target.framebuffer);
        CreateRenderTarget(target, newWidth, newHeight);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ImGui::Image((ImTextureID)(intptr_t)target.colorTexture, size, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMax().x - 60, 5));
    if (ImGui::Button("Grid")) {
        show_grid = !show_grid;
    }

    ImGui::End();
}

void ShowAboutWindow() {
    GLFWwindow* window = glfwCreateWindow(720, 400, "About LineEngine Editor", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("TEST");
        ImGui::TextUnformatted(u8"ТЕСТ ТЕСТ");
        ImGui::End();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window);
    }
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void ShowMainMenuBar(GLFWwindow* window) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {}
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) { glfwSetWindowShouldClose(window, GLFW_TRUE); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Shift+Ctrl+Y")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
            if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Settings")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Viewport", NULL, &show_viewport);
            ImGui::MenuItem("Outliner", NULL, &show_outliner);
            ImGui::MenuItem("Properties", NULL, &show_properties);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Info")) {
            if (ImGui::MenuItem("About editor...")) { ShowAboutWindow(); }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void ShowDockSpace() {
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("DockSpace Demo", nullptr, window_flags);

    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    ImGui::End();
}

int main() {
    Logger::Init();
    std::cerr << "==============================" << std::endl;
    std::cerr << LINEENGINE_EDITOR_NAME << " " << LINEENGINE_EDITOR_VERSION << std::endl;
    std::cerr << LINEENGINE_COMPANY_NAME << std::endl;
    std::cerr << "------------------------------" << std::endl;
    Logger::Info(std::string(THIS_PROG) + "Starting initialization...");

    // --- GLFW инициализация ---
    if (!glfwInit()) {
        Logger::Error(std::string(THIS_PROG) + "GLFW initialization failed");
        return -1;
    };
    Logger::Info(std::string(THIS_PROG) + "GLFW version: " + std::string(glfwGetVersionString()));

    // --- GLAD инициализация ---
    GLFWwindow* window = glfwCreateWindow(1280, 720, "LineEngine Editor", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Logger::Error(std::string(THIS_PROG) + "GLAD initialization failed");
        return -1;
    };
    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    Logger::Info(std::string(THIS_PROG) + "GLAD version: major: " + std::to_string(major) + " minor: " + std::to_string(minor));

    // --- ImGui инициализация ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Regular.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    ImGui::StyleColorsDark();

    if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
        Logger::Error(std::string(THIS_PROG) + "IMGUI.GLFW initialization failed");
        return -1;
    };
    if (!ImGui_ImplOpenGL3_Init("#version 330")) {
        Logger::Error(std::string(THIS_PROG) + "IMGUI.OpenGL3 initialization failed");
        return -1;
    };
    Logger::Info(std::string(THIS_PROG) + "IMGUI version: " + IMGUI_VERSION);

    Logger::Info(std::string(THIS_PROG) + "Initialization complete. Launching LineEngine...\n");

    // --- Движок ---
    LineEngine_Init();

    ViewportRenderTarget viewportTarget;

    // --- Главный цикл ---
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glBindFramebuffer(GL_FRAMEBUFFER, viewportTarget.framebuffer);
        glViewport(0, 0, viewportTarget.width, viewportTarget.height);

        LineEngine_Update();
        LineEngine_RenderScene(show_grid);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // --- GUI ---
        ShowMainMenuBar(window);
        ShowDockSpace();



        if (show_viewport) {
            ShowViewportWindow(viewportTarget, &show_viewport);
        }

        if (show_outliner) {
            ImGui::Begin("Outliner", &show_outliner);
            ImGui::TextUnformatted(u8"Список объектов сцены");
            ImGui::End();
        }
        if (show_properties) {
            ImGui::Begin("Properties", &show_properties);
            ImGui::TextUnformatted(u8"Свойства выбранного объекта");
            ImGui::End();
        }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // --- Очистка ---
    LineEngine_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    Logger::Shutdown();
    return 0;
}