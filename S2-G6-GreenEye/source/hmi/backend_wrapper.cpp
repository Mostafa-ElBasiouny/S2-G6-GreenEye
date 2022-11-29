/*
 * backend_wrapper.cpp
 * A simple backend wrapper for imgui and glfw.
 *
 * Mostafa Elbasiouny - Nov 04, 2022
 */

#include "backend_wrapper.h"

#include <iostream>

#include "../../imgui/imgui_impl_glfw.h"
#include "../../imgui/imgui_impl_opengl3.h"
#include "inter_regular.h"

static void glfwErrorCallback(int error, const char* description)
{
	std::fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

Wrapper::Wrapper(const char* title)
{
	glfwSetErrorCallback(glfwErrorCallback);
	if (!glfwInit())
		std::exit(1);

	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	this->window = glfwCreateWindow(1900, 1200, title, NULL, NULL);
	if (this->window == NULL)
		std::exit(1);

	glfwMakeContextCurrent(this->window);
	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.IniFilename = "";

	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(InterRegularCompressedData, InterRegularCompressedSize, 26.0f);
	ImGui::StyleColorsLight();

	ImGuiStyle& style = ImGui::GetStyle();

	{
		style.GrabMinSize = 20.0f;
		style.ScrollbarSize = 20.0f;
		style.PopupBorderSize = 1.0f;
		style.ChildBorderSize = 1.0f;
		style.WindowBorderSize = 1.0f;

		style.TabRounding = 12.0f;
		style.GrabRounding = 12.0f;
		style.FrameRounding = 12.0f;
		style.ChildRounding = 12.0f;
		style.FrameRounding = 12.0f;
		style.PopupRounding = 12.0f;
		style.ScrollbarRounding = 12.0f;

		style.LogSliderDeadzone = 12.0f;

		style.ItemSpacing = ImVec2(20.0f, 10.0f);
		style.ItemInnerSpacing = ImVec2(20.0f, 4.0f);

		style.FramePadding = ImVec2(20.0f, 16.0f);
		style.WindowPadding = ImVec2(16.0f, 16.0f);

		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.98f, 0.40f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.98f, 0.27f, 0.67f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.07f, 0.70f, 0.10f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.07f, 0.50f, 0.15f, 0.78f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.46f, 0.80f, 0.57f, 0.60f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.49f, 0.71f, 0.3f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.17f, 0.47f, 0.18f, 0.27f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.24f, 0.74f, 0.05f, 0.72f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.72f, 0.36f, 0.31f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.10f, 0.67f, 0.34f, 0.80f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.36f, 0.93f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.65f, 0.18f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.80f, 0.16f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.28f, 0.98f, 0.26f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.98f, 0.32f, 0.95f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.82f, 0.23f, 0.81f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.98f, 0.26f, 0.80f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.09f, 0.65f, 0.24f, 1.00f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.92f, 0.93f, 0.94f, 0.99f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.74f, 0.91f, 0.77f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.78f, 0.98f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.57f, 0.57f, 0.64f, 1.00f);
		style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.68f, 0.68f, 0.74f, 1.00f);
		style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.98f, 0.30f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.98f, 0.42f, 0.95f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.98f, 0.36f, 0.80f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	}

	ImGui_ImplGlfw_InitForOpenGL(this->window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void Wrapper::NewFrame()
{
	glfwPollEvents();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Wrapper::RenderFrame()
{
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(this->window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(this->window);
}

void Wrapper::Terminate()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();

	glfwDestroyWindow(this->window);
	glfwTerminate();
}
