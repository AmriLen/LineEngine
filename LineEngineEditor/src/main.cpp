#include <iostream>
#include <memory>
#include <LineEngineCore/Application.hpp>

class MyApp : public LineEngine::Application {
	virtual void on_update() override {
	//	std::cout << "Update frame: " << frame++ << std::endl;
	}

	int frame = 0;
};

int main() {
	auto myApp = std::make_unique<MyApp>();

	int returnCode = myApp->start(1280, 720, "My first app");

	std::cin.get();

	return returnCode;
}