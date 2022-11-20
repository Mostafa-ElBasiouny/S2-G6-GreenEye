/*
 * backend_wrapper.h
 * A simple backend wrapper for imgui and glfw.
 *
 * Mostafa Elbasiouny - Nov 04, 2022
 */

#pragma once

#include "../ImGui/GLFW/glfw3.h"
#include "../ImGui/imgui.h"
#include "../ImGui/implot.h"

class Wrapper {
public:
    GLFWwindow* window;

    Wrapper(const char* title);

    void NewFrame();
    void RenderFrame();

    void Terminate();
};
