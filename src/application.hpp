
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imGui/imgui.h>

#include <memory>
#include <vector>
#include <string>
#include "vec4.hpp"

#include "windows/window.hpp"
#include "windows/testWindow.hpp"


struct appData
{
    GLFWwindow* handle;
    const char* glslVersion;
    std::string name;
    int width = 1280;
    int height = 720;
    vec4<float> color = vec4<float>(0.0f, 0.0f, 0.0f, 1.0f);
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

class testWindow;

class application 
{
public:
    application();
    void run();
    ~application();

    std::vector<std::unique_ptr<::window<application>>> windows;
    void addWindow(std::unique_ptr<::window<application>>&& window);
    void addWindows();

    void prepareEnvironment();
    void prepareEnvironmentOnce();
    void handleInput();
    void update(float dt);
    void ui();
    void render();
    void cleanEnvironmentOnce();
    void cleanEnvironment();

    appData window;
    deltaTime deltaTime;
};

#endif