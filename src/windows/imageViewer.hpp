//
// Created on 11.02.2025.
//

#ifndef IMAGEVIEWER_HPP
#define IMAGEVIEWER_HPP

#include <memory>
#include <tinyfiledialogs/tinyfiledialogs.h>
#include "image.hpp"

#include "window.hpp"

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

    int selectedType = 0;
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
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 100);
    ImGui::SetNextItemWidth(80);
    if(ImGui::Button("select", ImVec2(80, 20))) {
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
    ImGui::NextColumn();
    ImGui::SetNextItemWidth(120);
    if (ImGui::Combo("Type", &selectedType, image::types, image::typesNum)) {
        img.changeType(static_cast<image::type>(selectedType));
        glXCheckError();
    }

    // image
    ImGui::Columns(1);
    ImGui::Text((std::string("Selected image: ") + (filename != nullptr ? filename : "<none>")).c_str());
    glXCheckError();
    img.display();
    glXCheckError();

    ImGui::End();
    glXCheckError();
}

inline void imageViewer::handleInput() { }
inline void imageViewer::update(float dt) { }
inline void imageViewer::render()
{
    img.render();
}

inline void imageViewer::cleanEnvironmentOnce() { }

inline void imageViewer::cleanEnvironment()
{
    img.destroy();
}

#endif //IMAGEVIEWER_HPP
