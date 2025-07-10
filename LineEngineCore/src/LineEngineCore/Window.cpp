#include "LineEngineCore/Window.hpp"
#include "LineEngineCore/Rendering/OpenGL/ShaderProgram.hpp"

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <alplogger/alplogger.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>


namespace LineEngine {

	static bool s_glfw_initialized = false;

	GLfloat points[] = {
		0.0f,  0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
	   -0.5f, -0.5f, 0.0f
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	const char* vertex_shader =
		"#version 460\n"
		"layout(location = 0) in vec3 vertex_position;"
		"layout(location = 1) in vec3 vertex_color;"
		"out vec3 color;"
		"void main() {"
		"	color = vertex_color;"
		"	gl_Position = vec4(vertex_position, 1.0);"
		"}";

	const char* fragment_shader = 
		"#version 460\n"
		"in vec3 color;"
		"out vec4 frag_color;"
		"void main() {"
		"	frag_color = vec4(color, 1.0);"
		"}";

	std::unique_ptr<ShaderProgram> p_shader_program;
	GLuint vao;

	Window::Window(std::string title, const unsigned int width, const unsigned int height)
		: m_data({std::move(title), width, height}) {
		int resultCode = init();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init();
		ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
	}

	Window::~Window() {
		shutdown();
	}

	int Window::init() {
		LOGGER::INFO("Creating window '" + std::string(m_data.title) + "' width size " + std::to_string(m_data.width) + "x" + std::to_string(m_data.height));

		//Инициализация библиотеки
		if (!s_glfw_initialized) {
			if (!glfwInit()) {
				LOGGER::CRITICAL("GLFW initialization failed");
				return -1;
			}
			s_glfw_initialized = true;
		}

		//Создаем окно в оконном режиме
		m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
		if (!m_pWindow) {
			LOGGER::CRITICAL("Create window failed -> " + std::string(m_data.title));
			glfwTerminate();
			return -2;
		}

		//Делаем контекст окна текущим
		glfwMakeContextCurrent(m_pWindow);

		//Инициализация GLAD
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			LOGGER::CRITICAL("GLAD initialization failed");
			return -3;
		}

		glfwSetWindowUserPointer(m_pWindow, &m_data);

		glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow* pWindow, int width, int height) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
			data.width = width;
			data.height = height;

			EventWindowResize event(width, height);
			data.eventCallbackFn(event);
		});

		glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* pWindow, double x, double y) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

			EventMouseMoved event(x, y);
			data.eventCallbackFn(event);
		});

		glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow* pWindow) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

			EventWindowClose event;
			data.eventCallbackFn(event);
		});

		glfwSetFramebufferSizeCallback(m_pWindow, [](GLFWwindow* pWindow, int width, int height) {
			glViewport(0, 0, width, height);
		});

		// Шейдерная программа
		/*GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertex_shader, nullptr);
		glCompileShader(vs);

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fragment_shader, nullptr);
		glCompileShader(fs);

		shader_program = glCreateProgram();
		glAttachShader(shader_program, vs);
		glAttachShader(shader_program, fs);
		glLinkProgram(shader_program);

		glDeleteShader(vs);
		glDeleteShader(fs);*/

		p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
		if (!p_shader_program->isCompiled()) {
			return false;
		}

		GLuint points_vbo = 0;
		glGenBuffers(1, &points_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

		GLuint colors_vbo = 0;
		glGenBuffers(1, &colors_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, nullptr);



		return 0;

	}

	void Window::shutdown() {
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}

	void Window::on_update() {
		glClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]); // Устанавливаем цвет очистки экрана
		glClear(GL_COLOR_BUFFER_BIT); // Очищаем экран цветом, установленным выше

		// Отрисовка
		p_shader_program->bind();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(get_width());
		io.DisplaySize.y = static_cast<float>(get_height());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow(); // DEMO окно

		ImGui::Begin("Background Color Window");
		ImGui::ColorEdit4("Background Color", m_background_color);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_pWindow); //Обновляем окно
		glfwPollEvents(); //Обрабатываем события
	}
}