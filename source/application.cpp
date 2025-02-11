#include "application.hpp"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imGui/imgui.h>
#include <imGui/imgui_impl_glfw.h>
#include <imGui/imgui_impl_opengl3.h>

void deltaTime::measure()
{
    currFrame = static_cast<float>(glfwGetTime());
    dt = currFrame - lastFrame;
    lastFrame = currFrame;
}

static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

/* application methods */

application::application()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        exit(-1);

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.3 + GLSL 150
    glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.3 + GLSL 130
    window.glslVersion = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
}

void application::run()
{
    // Create window with graphics context
    window.handle = glfwCreateWindow(window.width, window.height, window.name.c_str(), nullptr, nullptr);
    if (window.handle == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window.handle);
    glfwSwapInterval(1); // Enable vsync

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        exit(-1);
    }
    glEnable(GL_DEPTH_TEST);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    IMGUI_CHECKVERSION();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.handle, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init(window.glslVersion);


    /* ------------------------------------------------------------------- */
    /* done with init */

    while(!glfwWindowShouldClose(window.handle))
    {
        /* delta time */
        deltaTime.measure();

        /* handle current view inputs and other events */
        handleInput();
        glfwPollEvents();
        if (glfwGetWindowAttrib(window.handle, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        update(deltaTime());

        /* imGui new frame + view ui */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ui();

        /* clear etc. */
        ImGui::Render();
        glfwGetFramebufferSize(window.handle, &window.width, &window.height);
        glViewport(0, 0, window.width, window.height);
        glClearColor(window.color.x * window.color.w, 
            window.color.y * window.color.w,
            window.color.z * window.color.w,
            window.color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        /* render etc */
        render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window.handle);
    }
}

application::~application()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window.handle);
    glfwTerminate();
}
