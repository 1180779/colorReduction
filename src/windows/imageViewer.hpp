//
// Created on 11.02.2025.
//

#ifndef IMAGEVIEWER_HPP
#define IMAGEVIEWER_HPP

#include <memory>
#include <tinyfiledialogs/tinyfiledialogs.h>
#include "image.hpp"
#include "ui.hpp"

#include "window.hpp"
#include "../../deps/imGui/src/imgui_internal.h"

class application;

class imageViewer : public window<application>
{
public:
    explicit imageViewer(application& app) : window(app) { }
    ~imageViewer() override = default;

    void prepareEnvironmentOnce() override;
    void prepareEnvironment() override;

    void ui() override;
    void handleInput() override;
    void update(float dt) override;
    void render() override;

    void cleanEnvironment() override;
    void cleanEnvironmentOnce() override;

    int selectedType = static_cast<int>(image::defaultType);
    image img;
    const char * filename = nullptr;
};

///////////////////////////////////////////////////////////////////
/// implementation
//////////////////////////////////////////////////////////////////

#include "../application.hpp"

inline void imageViewer::prepareEnvironment()
{
    img.init();
}

inline void imageViewer::prepareEnvironmentOnce() { }

inline void imageViewer::ui()
{
    glXCheckError();
    ImGui::Begin("Image Viewer");

    // some buttons in two column layout
    if (ImGui::BeginTable("imageViewer-controls-1", 2)) {
        ImGui::TableNextRow();

        ImGui::TableNextColumn();
        if (ui::ComboFill("Type", &selectedType, image::types, image::typesNum)) {
            img.changeType(static_cast<image::type>(selectedType));
            glXCheckError();
        }

        ImGui::TableNextColumn();
        if(ui::ButtonFill("select")) {
            filename = tinyfd_openFileDialog("Select image",
                "",
                image::supportedFormatsNum,
                image::supportedFormats,
                "",
                0);
            if (filename != nullptr)
                img.load(filename);
            glXCheckError();
        }

        ImGui::EndTable();
    }

    // image
    ImGui::Text((std::string("Selected image: ") + (filename != nullptr ? filename : "<none>")).c_str());
    img.display();
    glXCheckError();

    ImGui::End();
}

inline void imageViewer::handleInput() { }
inline void imageViewer::update(float dt) { }
inline void imageViewer::render()
{
    img.render();
    glXCheckError();
}

inline void imageViewer::cleanEnvironmentOnce() { }

inline void imageViewer::cleanEnvironment()
{
    img.destroy();
}

#endif //IMAGEVIEWER_HPP
