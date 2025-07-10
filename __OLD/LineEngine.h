// LineEngine.h : включаемый файл для стандартных системных включаемых файлов
// или включаемые файлы для конкретного проекта.

#pragma once

// Инициализация и завершение работы движка (без окон и ImGui)
void LineEngine_Init();
void LineEngine_Shutdown();

// Обновление логики сцены (вызывается каждый кадр)
void LineEngine_Update();

// Рендер сцены в текущий активный фреймбуфер
void LineEngine_RenderScene(bool drawGrid);