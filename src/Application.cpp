#include "Application.h"




Application::Application():
	m_Window(nullptr, glfwDestroyWindow), m_Running(false)
{
	const auto result = Init()
	.and_then([this](bool _)
	{
		return InitImGui();
	})
	.or_else([](std::string_view error)
	{
		std::print("Error: {}\n", error);
		return std::expected<bool, std::string_view>{false};
	});

	if (result)
	{
		m_Running = *result;
	}
}


std::expected<bool, std::string_view> Application::Init()
{
	if (!glfwInit())
	{
		return std::unexpected("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	m_Window.reset(glfwCreateWindow(1080, 720, "CppChat", nullptr, nullptr));

	if (!m_Window)
	{
		glfwTerminate();
		return std::unexpected("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(m_Window.get());
	glfwSetWindowUserPointer(m_Window.get(), this);
	glfwSwapInterval(1);

	glfwSetWindowCloseCallback(m_Window.get(), [](GLFWwindow* window) -> void
	{
		if (auto const app = static_cast<Application*>(glfwGetWindowUserPointer(window)))
		{
			app->m_Running = false;
		}
	});

	return true;
}

std::expected<bool, std::string_view> Application::InitImGui() const
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	if (!ImGui_ImplGlfw_InitForOpenGL(m_Window.get(), true))
	{
		ImGui::DestroyContext();
		return std::unexpected("Failed to initialize ImGui for GLFW");
	}

	if (!ImGui_ImplOpenGL3_Init("#version 450"))
	{
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		return std::unexpected("Failed to initialize ImGui for OpenGL3");
	}

	return true;
}

void Application::Run()
{
	while (m_Running)
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		CreateDockSpace();

		ImGui::Begin("Main Window");

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		glfwSwapBuffers(m_Window.get());
	}
}

void Application::CreateDockSpace()
{
	static bool dockSpaceOpen = true;
	static bool optFullScreenPersistant = true;
	const bool optFullScreen = optFullScreenPersistant;

	static ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	if (optFullScreen)
	{
		const ImGuiViewport* const viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	if (dockSpaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
		windowFlags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", &dockSpaceOpen, windowFlags);
	ImGui::PopStyleVar();

	if (optFullScreen)
		ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		const ImGuiID dockSpaceId = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), dockSpaceFlags);
	}
	ImGui::End();
}


Application::~Application()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(m_Window.get());
	glfwTerminate();
}