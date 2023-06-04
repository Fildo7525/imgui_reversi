#pragma once

#include "imgui.h"
#include "Layer.h"
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <memory>
#include <vector>

class Application
{
public:
	struct Config
	{
		const std::string &title;
		int width;
		int height;
		bool resizable;
		bool fullscreen;
		const std::string &font;
	};

	enum class RenderBackend
	{
		Polling,
		WaitEvents,
	};

	explicit Application(const Application::Config &config, Application::RenderBackend renderBackend = Application::RenderBackend::Polling);
	~Application();

	Application &addLayer(const std::shared_ptr<Layer> &layer);
	void run();

private:
	void Init();
	void Cleanup();

private:
	/// OpenGL3 window data.
	GLFWwindow* m_window;

	/// Configuration of the GLFW window.
	Config m_config;

	/// @c RenderBackend enum variable identifying the rendering method.
	RenderBackend m_renderBackend;

	/// Flag to show the ImGui demo window.
	bool m_showDemoWindow = true;

	/// Flag to show the ImGui metrics window.
	ImVec4 m_clearColor;

	/// All the windows displayed in the application.
	std::vector<std::shared_ptr<Layer>> m_layers;
};

