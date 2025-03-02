//
// Created on 13.02.2025.
//

#ifndef IMAGECONVERTER_HPP
#define IMAGECONVERTER_HPP

#include <map>
#include <memory>
#include <tinyfiledialogs/tinyfiledialogs.h>

#include "../debug.hpp"
#include "window.hpp"
#include "image.hpp"
#include "computeShader.hpp"
#include "conMatSSBO.hpp"
#include "matProviderPopup.hpp"

#include "ui.hpp"

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

    std::string filename;
    int selectedType = static_cast<int>(image::defaultType);
    image imgSource;
    image imgConverted;

    conMatSSBO conMatSSBO = {};
    matProviderPopup matProviderPopup;
private:
    static std::unique_ptr<computeShader> computeShader_s;
    static std::unique_ptr<computeShader> computeShaderInvert_s;

    void uiButtons();
    void uiButtonClear();
    void uiButtonSetMatrix();
    void uiButtonSelectImage();
    void uiButtonConvertImage();
    void uiButtonInvertImage();
    void uiButtonSetResultAsSource();
    void uiButtonChangeImageDisplayType();
    void uiButtonSaveImage() const;
    void uiImages();
};

//////////////////////////////////////////////////////////////////
/// implementation
//////////////////////////////////////////////////////////////////

#include "../application.hpp"

std::unique_ptr<computeShader> imageConverter::computeShader_s;
std::unique_ptr<computeShader> imageConverter::computeShaderInvert_s;

inline void imageConverter::prepareEnvironmentOnce()
{
    computeShader_s = std::make_unique<computeShader>("GL_RGBA8.comp");
    computeShaderInvert_s = std::make_unique<computeShader>("invert.comp");
}

inline void imageConverter::prepareEnvironment()
{
    imgSource.init();
    imgConverted.init();
    conMatSSBO.init();
}

inline void imageConverter::ui()
{
    ImGui::Begin("Image Converter");
    uiButtons();
    ImGui::Text((std::string("Selected image: ") + (filename.empty() ? "<none>" : filename)).c_str());
    uiImages();
    ImGui::End();

    /* show the popup if need be */
    matProviderPopup.show(conMatSSBO.mat);
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
    conMatSSBO.destroy();
}

inline void imageConverter::cleanEnvironmentOnce()
{
    computeShader_s.reset();
    computeShaderInvert_s.reset();
}

inline void imageConverter::uiButtons()
{
    if (ImGui::BeginTable("controls-1", 4)) {
        ImGui::TableNextRow();

        ImGui::TableNextColumn();
        uiButtonChangeImageDisplayType();

        ImGui::TableNextColumn();
        uiButtonSelectImage();

        ImGui::TableNextColumn();
        uiButtonSetMatrix();

        ImGui::TableNextColumn();
        uiButtonSaveImage();

        ImGui::EndTable();
    }

    if (ImGui::BeginTable("controls-2", 4)) {
        ImGui::TableNextRow();

        ImGui::TableNextColumn();
        uiButtonClear();

        ImGui::TableNextColumn();
        uiButtonSetResultAsSource();

        ImGui::TableNextColumn();
        uiButtonConvertImage();

        ImGui::TableNextColumn();
        uiButtonInvertImage();

        ImGui::EndTable();
    }
}

inline void imageConverter::uiButtonClear()
{
    if (ui::ButtonFill("clear")) {
        imgSource.clear();
        imgConverted.clear();
    }
}

inline void imageConverter::uiButtonSetMatrix()
{
    if (ui::ButtonFill("set matrix")) {
        matProviderPopup.schedule();
    }
}

inline void imageConverter::uiButtonSelectImage()
{
    if (ui::ButtonFill("select")) {
        const char * path = tinyfd_openFileDialog("Select image",
            "",
            image::supportedFormatsNum,
            image::supportedFormats,
            "",
            0);
        filename = path != nullptr ? std::string(path) : std::string("");
        if (!filename.empty())
            imgSource.load(filename.c_str());
    }
}

inline void imageConverter::uiButtonConvertImage()
{
    if (ui::ButtonFill("convert")) {
        glXCheckError();
        conMatSSBO.set(conMatSSBO.mat.getN(), conMatSSBO.mat.getM(), conMatSSBO.mat.getData());
        glXCheckError();

        imgConverted.copy(imgSource);
        computeShader_s->use();
        imgSource.bindImageTexture(0, GL_READ_ONLY);
        glXCheckError();
        imgConverted.bindImageTexture(1, GL_WRITE_ONLY);
        glXCheckError();

        conMatSSBO.use(*computeShader_s, 0, 2);
        glXCheckError();
        computeShader_s->setl1i(4, imgSource.getHeight());
        glXCheckError();
        computeShader_s->setl1i(5, imgSource.getWidth());
        glXCheckError();

        glDispatchCompute(
            static_cast<GLuint>(imgSource.getWidth()),
            static_cast<GLuint>(imgSource.getHeight()),
            1);
        // make sure writing to image has finished before read
        glXCheckError();
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        glXCheckError();

        glBindTexture(GL_TEXTURE_2D, 0);
        //imgSource.copy(imgConverted);
    }
}

inline void imageConverter::uiButtonInvertImage()
{
    if (ui::ButtonFill("invert")) {
        glXCheckError();
        imgConverted.copy(imgSource);
        computeShaderInvert_s->use();
        imgSource.bindImageTexture(0, GL_READ_ONLY);
        imgConverted.bindImageTexture(1, GL_WRITE_ONLY);

        glDispatchCompute(
            static_cast<GLuint>(imgSource.getWidth()),
            static_cast<GLuint>(imgSource.getHeight()),
            1);
        // make sure writing to image has finished before read
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        glBindTexture(GL_TEXTURE_2D, 0);
        glXCheckError();
    }
}

inline void imageConverter::uiButtonSetResultAsSource()
{
    if (ui::ButtonFill("swap")) {
        glXCheckError();
        imgSource.copy(imgConverted);
        glXCheckError();
    }
}

inline void imageConverter::uiButtonChangeImageDisplayType()
{
    if (ui::ComboFill("type", &selectedType, image::types, image::typesNum)) {
        auto t = static_cast<image::type>(selectedType);
        imgSource.changeType(t);
        imgConverted.changeType(t);
    }
}

inline void imageConverter::uiButtonSaveImage() const
{
    if (ui::ButtonFill("save")) {
        const char * formats[] = { "*.png" };
        const char* path = tinyfd_saveFileDialog("save result", "converted", 1, formats, "");
        imgConverted.writePng(path);
    }
}

inline void imageConverter::uiImages()
{
    if (ImGui::BeginTable("controls-2", 2)) {
        ImGui::TableNextRow();

        ImGui::TableNextColumn();
        imgSource.display();
        glXCheckError();

        ImGui::TableNextColumn();
        imgConverted.display();
        glXCheckError();

        ImGui::EndTable();
    }
}

#endif //IMAGECONVERTER_HPP
