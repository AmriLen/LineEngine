// LineEngine.cpp: определяет точку входа для приложения.
//

#include "LineEngine.h"
#include <glad/glad.h>
#include <gl/gl.h>
#pragma comment(lib, "opengl32.lib")

#include "Logger.h"

#include <iostream>

// Версия движка
#define LINEENGINE_NAME				"LineEngine"
#define LINEENGINE_VERSION			"v0.0.1"
#define LINEENGINE_COMPANY_NAME     "AFTERLINE Production"
#define THIS_PROG					"[LineEngine] "

float theta = 0.0f;

void LineEngine_Init() {
    Logger::Init();
    std::cerr << "==============================" << std::endl;
    std::cerr << LINEENGINE_NAME << " " << LINEENGINE_VERSION << std::endl;
    std::cerr << LINEENGINE_COMPANY_NAME << std::endl;
    std::cerr << "------------------------------" << std::endl;
    Logger::Info(std::string(THIS_PROG) + "Starting initialization...");
    
    // Инициализация данных сцены, объектов и т.д.


    Logger::Info(std::string(THIS_PROG) + "Initialization complete");
}

void LineEngine_RenderScene(bool drawGrid) {

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    glRotatef(theta, 0.0f, 0.0f, 1.0f);

    glBegin(GL_TRIANGLES);

    glColor3f(1.0f, 0.0f, 0.0f);   glVertex2f(0.0f, 1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);   glVertex2f(0.87f, -0.5f);
    glColor3f(0.0f, 0.0f, 1.0f);   glVertex2f(-0.87f, -0.5f);

    glEnd();

    glPopMatrix();

    //SwapBuffers(hDC);

    theta += 1.0f;
    //Sleep(1);
    // TODO: здесь будет рендеринг объектов сцены
}

void LineEngine_Update() {
    // Обновление логики сцены, анимаций и т.д.
}

void LineEngine_Shutdown() {
    // Очистка ресурсов сцены
}