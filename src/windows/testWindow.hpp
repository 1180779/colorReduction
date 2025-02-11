//
// Created on 11.02.2025.
//

#ifndef TESTWINDOW_HPP
#define TESTWINDOW_HPP

#include "window.hpp"

class application;

class testWindow : public window<application> {
public:
    testWindow(application& app, const char* name) : window(app), name(name) { }
    ~testWindow() override = default;

    void prepareEnvironmentOnce() override { }
    void prepareEnvironment() override { }

    void ui() override;
    void handleInput() override;
    void update(float dt) override;
    void render() override;

    void cleanEnvironment() override { }
    void cleanEnvironmentOnce() override { }


    const char* name;
};

///////////////////////////////////////////////////////////////////
/// implementation
//////////////////////////////////////////////////////////////////

#include "../application.hpp"
#include <imGui/imgui.h>

inline void testWindow::ui()
{
    ImGui::Begin(name);
    ImGui::Text("Test window");
    ImGui::End();
}

inline void testWindow::handleInput()
{

}

inline void testWindow::update(float dt)
{

}

inline void testWindow::render()
{

}

#endif //TESTWINDOW_HPP
