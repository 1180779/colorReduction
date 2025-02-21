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
    int selectedType = 0;
    image imgSource;
    image imgConverted;

    conMatSSBO conMatSSBO = {};
    matProviderPopup matProviderPopup;
private:
    static std::unique_ptr<computeShader> computeShader_s;
};

//////////////////////////////////////////////////////////////////
/// implementation
//////////////////////////////////////////////////////////////////

#include "../application.hpp"

std::unique_ptr<computeShader> imageConverter::computeShader_s;

inline void imageConverter::prepareEnvironmentOnce()
{
    computeShader_s = std::make_unique<computeShader>("GL_RGBA8.comp");
}

inline void imageConverter::prepareEnvironment()
{
    imgSource.init();
    imgConverted.init();
    conMatSSBO.init();
}

inline void imageConverter::ui()
{
    glXCheckError();
    ImGui::Begin("Image Converter");
    ImGui::Columns(6);
    if (ImGui::Button("clear")) {
        imgSource.clear();
        imgConverted.clear();
    }

    ImGui::NextColumn();
    if (ImGui::Button("set matrix")) {
        matProviderPopup.schedule();
    }
    ImGui::NextColumn();
    if (ImGui::Button("select")) {
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
    ImGui::NextColumn();
    if (ImGui::Combo("type", &selectedType, image::types, image::typesNum)) {
        auto t = static_cast<image::type>(selectedType);
        imgSource.changeType(t);
        imgConverted.changeType(t);
    }
    ImGui::NextColumn();
    if (ImGui::Button("Convert")) {
        glXCheckError();
        conMatSSBO.set(conMatSSBO.mat.getN(), conMatSSBO.mat.getM(), conMatSSBO.mat.getData());
        glXCheckError();

        imgConverted.copy(imgSource);
        glXCheckError();
        computeShader_s->use();
        glXCheckError();
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
        glXCheckError();
        //imgSource.copy(imgConverted);
    }
    ImGui::NextColumn();
    if (ImGui::Button("save")) {
        const char * formats[] = { "*.png" };
        const char* path = tinyfd_saveFileDialog("save result", "converted", 1, formats, "");
        imgConverted.writePng(path);
    }

    ImGui::Columns(1);
    ImGui::Text((std::string("Selected image: ") + (filename.empty() ? "<none>" : filename)).c_str());

    ImGui::Columns(2);
    imgSource.display();
    ImGui::NextColumn();
    imgConverted.display();

    ImGui::End();
    glXCheckError();

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
}

#endif //IMAGECONVERTER_HPP
