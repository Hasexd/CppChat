#pragma once
#include <memory>
#include <expected>
#include <string>
#include <print>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


class Application
{
public:
	Application();
	~Application();

	void Run();

private:
	std::expected<bool, std::string_view> Init();
	std::expected<bool, std::string_view> InitImGui() const;
	static void CreateDockSpace();

private:
	std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> m_Window;

	bool m_Running;
};