
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imGui/imgui.h>

#include <string>
#include "vec4.hpp"

struct window
{
    GLFWwindow* handle;
    const char* glslVersion;
    std::string name;
    int width = 1280;
    int height = 720;
    vec4<> color;
};

struct deltaTime
{
    deltaTime() = default;
    float currFrame = 0.0f;
    float lastFrame = 0.0f;
    float dt = 0.0f;

    float operator()() const { return dt; }
    void measure();
};

class application 
{
public:
    application();
    void run();
    ~application();

    void handleInput() { }
    void update(float dt) { }
    void ui()
    {
        ImGui::Begin("Settings");
        ImGui::Text("Test window");
        ImGui::End();
    }
    void render() { }

    window window;
    deltaTime deltaTime;
};

#endif