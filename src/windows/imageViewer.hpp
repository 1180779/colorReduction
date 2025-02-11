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
    explicit imageViewer(application& app) : window<application>(app) { }
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
    void rescale_framebuffer(float width, float height);

private:
    static std::unique_ptr<shader> shader_s;
    const char* fileName_ = nullptr;
    int width_ = 0,
        height_ = 0,
        nrChannels_ = 0;

    GLuint texture_ = 0;
    GLuint FBO_ = 0;
    GLuint RBO_ = 0;

    static GLuint VAO_s;
    static GLuint VBO_s;
    static float vertices_s[6 * 5];
};

///////////////////////////////////////////////////////////////////
/// implementation
//////////////////////////////////////////////////////////////////

#include "../application.hpp"

std::unique_ptr<shader> imageViewer::shader_s;

GLuint imageViewer::VAO_s = 0;
GLuint imageViewer::VBO_s = 0;

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

    // FBO + RBO
    glGenFramebuffers(1, &FBO_);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO_);

    glGenRenderbuffers(1, &RBO_);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, parent_.window.width, parent_.window.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO_);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";

    // reset bindings
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

inline void imageViewer::prepareEnvironmentOnce()
{
    stbi_set_flip_vertically_on_load(true);

    shader_s = std::make_unique<shader>("sh.vert", "sh.frag");

    // VAO + VBO
    glGenVertexArrays(1, &VAO_s);
    glBindVertexArray(VAO_s);

    glGenBuffers(1, &VBO_s);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_s);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_s), vertices_s, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

inline void imageViewer::ui()
{
    ImGui::Begin("Image Viewer");
    ImGui::Text("Select an image");
    if(ImGui::Button("OK")) {
        fileName_ = tinyfd_openFileDialog("select image",
            "",
            0,
            static_cast<const char * const * const>(nullptr),
            "",
            0);
        loadImage();
    }

    // we access the ImGui window size
    const float window_width = ImGui::GetContentRegionAvail().x;
    const float window_height = ImGui::GetContentRegionAvail().y;

    // we rescale the framebuffer to the actual window size here and reset the glViewport
    rescale_framebuffer(window_width, window_height);
    glViewport(0, 0, static_cast<GLsizei>(window_width), static_cast<GLsizei>(window_height));

    // we get the screen position of the window
    ImVec2 pos = ImGui::GetCursorScreenPos();

    // and here we can add our created texture as image to ImGui
    // unfortunately we need to use the cast to void* or I didn't find another way tbh
    ImGui::GetWindowDrawList()->AddImage(
        texture_,
        ImVec2(pos.x, pos.y),
        ImVec2(pos.x + window_width, pos.y + window_height),
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    ImGui::Text((std::string("Selected image: ") + (fileName_ != nullptr ? fileName_ : "")).c_str());
    ImGui::End();
}

inline void imageViewer::handleInput() { }
inline void imageViewer::update(float dt) { }
inline void imageViewer::render()
{
    if (fileName_ == nullptr)
        return;

    glBindFramebuffer(GL_FRAMEBUFFER, FBO_);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO_);
    glBindTexture(GL_TEXTURE_2D, texture_);

    shader_s->use();
    glBindVertexArray(VAO_s);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

inline void imageViewer::cleanEnvironmentOnce()
{
    shader_s.reset();

    glDeleteBuffers(1, &VBO_s);
    glDeleteVertexArrays(1, &VAO_s);
}

inline void imageViewer::cleanEnvironment()
{
    glDeleteTextures(1, &texture_);

    glDeleteFramebuffers(1, &FBO_);
    glDeleteRenderbuffers(1, &RBO_);
}

inline void imageViewer::loadImage()
{
    // texture
    unsigned char* data = stbi_load(fileName_, &width_, &height_, &nrChannels_, 0);

    GLenum format = GL_RGB; // Default
    if (nrChannels_ == 1)
        format = GL_RED;
    else if (nrChannels_ == 3)
        format = GL_RGB;
    else if (nrChannels_ == 4)
        format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, texture_);

    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(format), width_, height_, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

// and we rescale the buffer, so we're able to resize the window
inline void imageViewer::rescale_framebuffer(float width, float height)
{
    //glBindTexture(GL_TEXTURE_2D, texture_);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, RBO_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO_);
}

#endif //IMAGEVIEWER_HPP
