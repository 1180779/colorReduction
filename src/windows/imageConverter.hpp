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

    conMatSSBO conMatSSBO = {};

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
        static float data[] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        glXCheckError();
        conMatSSBO.set(1, 2, data);
        glXCheckError();

        imgConverted.copy(imgSource);
        glXCheckError();
        computeShader_s->use();
        glXCheckError();
        imgSource.bindImageTexture(0, GL_READ_ONLY);
        glXCheckError();
        imgConverted.bindImageTexture(1, GL_WRITE_ONLY);
        glXCheckError();

        conMatSSBO.use(*computeShader_s, 0, 1);
        glXCheckError();
        computeShader_s->setl1i(2, imgSource.getHeight());
        glXCheckError();
        computeShader_s->setl1i(3, imgSource.getWidth());
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

    ImGui::Columns(2);
    imgSource.display();
    ImGui::NextColumn();
    imgConverted.display();

    ImGui::End();
    glXCheckError();
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
