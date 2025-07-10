#include "LineEngineCore/Application.hpp"
#include "LineEngineCore/Window.hpp"

#include <alplogger/alplogger.h>
#include <iostream>

namespace LineEngine {

	Application::Application() {
		LOGGER::INFO("Starting Application...");
	}
	Application::~Application() {
		LOGGER::INFO("Closing Application");
	}

	int Application::start(unsigned int window_width, unsigned int window_height, const char* window_title) {

		m_pWindow = std::make_unique<Window>(window_title, window_width, window_height);

		m_event_dispatcher.add_event_listener<EventMouseMoved>([](EventMouseMoved& event) {
			//LOGGER::INFO("[MouseMoved] Mouse moved to " + std::to_string(event.x) + "x" + std::to_string(event.y));
		});

		m_event_dispatcher.add_event_listener<EventWindowResize>([](EventWindowResize& event) {
			LOGGER::INFO("[Resized] Changed size to " + std::to_string(event.width) + "x" + std::to_string(event.height));
		});

		m_event_dispatcher.add_event_listener<EventWindowClose>([&](EventWindowClose& event) {
			LOGGER::INFO("[WindowClose]");
			m_bCloseWindow = true;
		});

		m_pWindow->set_event_callback([&](BaseEvent& event) {
			m_event_dispatcher.dispatch(event);
		});

		while (!m_bCloseWindow) {
			m_pWindow->on_update();
			on_update();
		}

		m_pWindow = nullptr;

		return 0;
	}
}