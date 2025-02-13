//
// Created on 13.02.2025.
//

#ifndef IMAGECONVERTER_HPP
#define IMAGECONVERTER_HPP

#include <tinyfiledialogs/tinyfiledialogs.h>
#include "window.hpp"
#include "image.hpp"

class application;
class imageConverter : public window<application> {
public:
    explicit imageConverter(application& app) : window(app) { }
    ~imageConverter() override = default;

    void prepareEnvironmentOnce() override;
    void prepareEnvironment() override;

    void ui() override;
    void handleInput() override;
    void update(float dt) override;
    void render() override;

    void cleanEnvironment() override;
    void cleanEnvironmentOnce() override;

    const char * filename = nullptr;
    int selectedType = 0;
    image imgSource;
    image imgConverted;
};

///////////////////////////////////////////////////////////////////
/// implementation
//////////////////////////////////////////////////////////////////

#include "../application.hpp"

inline void imageConverter::prepareEnvironmentOnce() { }

inline void imageConverter::prepareEnvironment()
{
    imgSource.init();
    imgConverted.init();
}

inline void imageConverter::ui()
{
    ImGui::Begin("Image Converter");
    if (ImGui::Button("clear")) {
        imgSource.clear();
        imgConverted.clear();
    }

    if (ImGui::Button("select")) {
        filename = tinyfd_openFileDialog("Select image",
            "",
            image::supportedFormatsNum,
            image::supportedFormats,
            "",
            0);
        if (filename != nullptr)
            imgSource.load(filename);
    }
    ImGui::Text((std::string("Selected image: ") + (filename != nullptr ? filename : "<none>")).c_str());

    if (ImGui::Combo("type", &selectedType, image::types, image::typesNum)) {
        auto t = static_cast<image::type>(selectedType);
        imgSource.changeType(t);
        imgConverted.changeType(t);
    }

    if (ImGui::Button("Convert")) {
        // TODO add convertion
        imgConverted.copy(imgSource);
        //imgSource.copy(imgConverted);
    }

    ImGui::Columns(2);
    imgSource.display();
    ImGui::NextColumn();
    imgConverted.display();

    ImGui::End();
}

inline void imageConverter::handleInput() { }

inline void imageConverter::update(float dt) { }

inline void imageConverter::render()
{
    imgSource.render();
    imgConverted.render();
}

inline void imageConverter::cleanEnvironment()
{
    imgSource.destroy();
    imgConverted.destroy();
}

inline void imageConverter::cleanEnvironmentOnce() { }

#endif //IMAGECONVERTER_HPP
