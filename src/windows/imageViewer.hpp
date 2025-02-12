//
// Created on 11.02.2025.
//

#ifndef IMAGEVIEWER_HPP
#define IMAGEVIEWER_HPP

#include <memory>
#include <tinyfiledialogs/tinyfiledialogs.h>
#include <stb/stb_image.h>
#include <glad/glad.h>

#include "window.hpp"
#include "shader.hpp"

class application;

class imageViewer : public window<application>
{
public:
    static const char * supportedFormats[5];
    static const int supportedFormatsNum;

    static const char * types[2];
    enum type
    {
        stretch,
        scale,
    };

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

    void loadImage();
    void rescaleVBO() const;
    void rescale_framebuffer(float width, float height);
    void changeType(type newType);

private:
    void typeRescaleScaledA(float newVertices[]) const;
    void typeRescaleScaledB(float newVertices[]) const;

    static std::unique_ptr<shader> shader_s;
    const char* fileName_ = nullptr;
    int width_ = 0,
        height_ = 0,
        nrChannels_ = 0;
    float aspectRatio_ = 1.0f;
    GLenum format_ = GL_RGB;

    GLuint texture_ = 0,
        textureDisplayed_ = 0,
        FBO_ = 0;
    int widthFB_ = 0,
        heightFB_ = 0;

    GLuint VAO_ = 0,
        VBO_ = 0;
    static float vertices_s[6 * 5];

    int typeInt_ = 0;
    type type_ = stretch;
};

///////////////////////////////////////////////////////////////////
/// implementation
//////////////////////////////////////////////////////////////////

#include "../application.hpp"

const char * imageViewer::supportedFormats[5] = { "*.png", "*.jpg", "*.jpeg", "*.bmp", "*.gif" };
const int imageViewer::supportedFormatsNum = sizeof(supportedFormats) / sizeof(char *);
const char * imageViewer::types[2] = { "stretch", "scale" };

std::unique_ptr<shader> imageViewer::shader_s;

float imageViewer::vertices_s[6 * 5] = {
    -1.0f, -1.0f, 0.0f,   0.0f,  0.0f,
    -1.0f,  1.0f, 0.0f,   0.0f,  1.0f,
     1.0f, -1.0f, 0.0f,   1.0f,  0.0f,

     1.0f, -1.0f, 0.0f,   1.0f,  0.0f,
     1.0f,  1.0f, 0.0f,   1.0f,  1.0f,
    -1.0f,  1.0f, 0.0f,   0.0f,  1.0f,
};

inline void imageViewer::prepareEnvironment()
{
    // texture
    glGenTextures(1, &texture_);

    // VAO + VBO
    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);

    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_s), vertices_s, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // FBO + textureDisplayed
    glGenFramebuffers(1, &FBO_);
    glGenTextures(1, &textureDisplayed_);
}

inline void imageViewer::prepareEnvironmentOnce()
{
    stbi_set_flip_vertically_on_load(true);

    shader_s = std::make_unique<shader>("sh.vert", "sh.frag");
}

inline void imageViewer::ui()
{
    ImGui::Begin("Image Viewer");

    // some buttons in two column layout
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 100);
    ImGui::SetNextItemWidth(80);
    if(ImGui::Button("select", ImVec2(80, 20))) {
        fileName_ = tinyfd_openFileDialog("Select image",
            "",
            supportedFormatsNum,
            supportedFormats,
            "",
            0);
        if (fileName_ != nullptr)
            loadImage();
    }
    ImGui::NextColumn();
    ImGui::SetNextItemWidth(120);
    if (ImGui::Combo("Type", &typeInt_, types, 2)) {
        changeType(static_cast<type>(typeInt_));
    }

    // image
    ImGui::Columns(1);
    ImGui::Text((std::string("Selected image: ") + (fileName_ != nullptr ? fileName_ : "<none>")).c_str());

    // we access the ImGui window size
    const float width = ImGui::GetContentRegionAvail().x;
    const float height = ImGui::GetContentRegionAvail().y;

    // we rescale the framebuffer to the actual window size here and reset the glViewport
    rescale_framebuffer(width, height);
    rescaleVBO();


    // we get the screen position of the window
    const ImVec2 pos = ImGui::GetCursorScreenPos();

    // add texture as image to ImGui
    ImGui::GetWindowDrawList()->AddImage(
        textureDisplayed_,
        ImVec2(pos.x, pos.y),
        ImVec2(pos.x + width, pos.y + height),
        ImVec2(0, 1),
        ImVec2(1, 0)
    );
    ImGui::End();
}

inline void imageViewer::handleInput() { }
inline void imageViewer::update(float dt) { }
inline void imageViewer::render()
{
    if (fileName_ == nullptr)
        return;

    // set viewport for the displayed image
    glViewport(0, 0, static_cast<GLsizei>(widthFB_), static_cast<GLsizei>(heightFB_));
    // clear before redrawing
    glBindFramebuffer(GL_FRAMEBUFFER, FBO_);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, texture_);
    shader_s->use();
    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glUseProgram(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

inline void imageViewer::cleanEnvironmentOnce()
{
    shader_s.reset();

    glDeleteBuffers(1, &VBO_);
    glDeleteVertexArrays(1, &VAO_);
}

inline void imageViewer::cleanEnvironment()
{
    glDeleteTextures(1, &texture_);
    texture_ = 0;

    glDeleteFramebuffers(1, &FBO_);
    FBO_ = 0;
    glDeleteTextures(1, &textureDisplayed_);
    textureDisplayed_ = 0;
}

inline void imageViewer::loadImage()
{
    // loaded image texture
    unsigned char* data = stbi_load(fileName_, &width_, &height_, &nrChannels_, 0);
    aspectRatio_ = static_cast<float>(width_) / static_cast<float>(height_);

    int unpackAlignment = 1;
    if (width_ & 4 == 0 || height_ & 4 == 0) {
        unpackAlignment = 4;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, unpackAlignment);

    format_ = GL_RGB;
    if (nrChannels_ == 1)
        format_ = GL_RED;
    else if (nrChannels_ == 3)
        format_ = GL_RGB;
    else if (nrChannels_ == 4)
        format_ = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0,
        static_cast<int>(format_),
        static_cast<GLsizei>(width_),
        static_cast<GLsizei>(height_), 0, format_, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    // create all other data needed for rendering
    glBindFramebuffer(GL_FRAMEBUFFER, FBO_);

    glBindTexture(GL_TEXTURE_2D, textureDisplayed_);
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format_), width_, height_, 0, format_, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureDisplayed_, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
        throw std::runtime_error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

inline void imageViewer::rescaleVBO() const
{
    switch (type_) {
        case type::stretch:
            break;
        case type::scale:
            float newVertices[6 * 5];
            if (const float windowAspectRatio = static_cast<float>(widthFB_) / static_cast<float>(heightFB_);
                aspectRatio_ > windowAspectRatio) {
                typeRescaleScaledA(newVertices);
            }
            else {
                typeRescaleScaledB(newVertices);
            }

            // remap texture coords to make image scaled
            glBindVertexArray(VAO_);

            glBindBuffer(GL_ARRAY_BUFFER, VBO_);
            glBufferData(GL_ARRAY_BUFFER, sizeof(newVertices), newVertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            break;
    }

}

// rescale the buffer, so we're able to resize the window
inline void imageViewer::rescale_framebuffer(const float width, const float height)
{
    widthFB_ = static_cast<int>(width);
    heightFB_ = static_cast<int>(height);

    glBindTexture(GL_TEXTURE_2D, textureDisplayed_);
    glTexImage2D(GL_TEXTURE_2D, 0,  static_cast<GLint>(format_),
        static_cast<GLsizei>(widthFB_),
        static_cast<GLsizei>(heightFB_),
        0, format_, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_, 0);
}

inline void imageViewer::changeType(const type newType)
{
    if (type_ == newType)
        return;
    type_ = newType;

    float newVertices[6 * 5];
    switch (type_) {
        case type::stretch:
            memcpy(newVertices, vertices_s, 6 * 5 * sizeof(float));
            break;
        case type::scale:

            if (const float windowAspectRatio = static_cast<float>(widthFB_) / static_cast<float>(heightFB_);
                aspectRatio_ > windowAspectRatio) {
                typeRescaleScaledA(newVertices);
            }
            else {
                typeRescaleScaledB(newVertices);
            }
            break;
    }

    // remap texture coords to make image scaled
    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(newVertices), newVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

inline void imageViewer::typeRescaleScaledA(float newVertices[]) const
{
    const float scale = static_cast<float>(widthFB_)  / static_cast<float>(width_);
    const float height = scale * static_cast<float>(height_);
    for (int i = 0; i < 6; i++) {
        newVertices[5 * i + 0] = vertices_s[5 * i + 0];
        newVertices[5 * i + 1] = vertices_s[5 * i + 1];
        newVertices[5 * i + 2] = vertices_s[5 * i + 2];
        newVertices[5 * i + 3] = vertices_s[5 * i + 3];
        newVertices[5 * i + 4] = vertices_s[5 * i + 4];
    }

    const float newY = height / static_cast<float>(heightFB_);
    newVertices[5 * 0 + 1] =  -newY;
    newVertices[5 * 1 + 1] =  newY;
    newVertices[5 * 2 + 1] =  -newY;
    newVertices[5 * 3 + 1] =  -newY;
    newVertices[5 * 4 + 1] =  newY;
    newVertices[5 * 5 + 1] =  newY;
}

inline void imageViewer::typeRescaleScaledB(float newVertices[]) const
{
    const float scale = static_cast<float>(heightFB_)  / static_cast<float>(height_);
    const float width = scale * static_cast<float>(width_);
    for (int i = 0; i < 6; i++) {
        newVertices[5 * i + 0] = vertices_s[5 * i + 0];
        newVertices[5 * i + 1] = vertices_s[5 * i + 1];
        newVertices[5 * i + 2] = vertices_s[5 * i + 2];
        newVertices[5 * i + 3] = vertices_s[5 * i + 3];
        newVertices[5 * i + 4] = vertices_s[5 * i + 4];
    }

    const float newX = width / static_cast<float>(widthFB_);
    newVertices[5 * 0 + 0] =  -newX;
    newVertices[5 * 1 + 0] =  -newX;
    newVertices[5 * 2 + 0] =  newX;
    newVertices[5 * 3 + 0] =  newX;
    newVertices[5 * 4 + 0] =  newX;
    newVertices[5 * 5 + 0] =  -newX;
}

#endif //IMAGEVIEWER_HPP
