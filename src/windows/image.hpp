//
// Created on 13.02.2025.
//

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <glad/glad.h>
#include <imGui/imgui.h>
#include <stb/stb_image.h>

#include <memory>
#include "shader.hpp"

class image {
public:
    static const char * supportedFormats[5];
    static const int supportedFormatsNum;

    static const char * types[2];
    static const int typesNum;
    enum type
    {
        stretch,
        scale,
    };

    static void initOnce();
    void init();

    // place inside ImGui window
    void display();

    // do the rendering on the displayed texture
    void render() const;

    // change texture to all black
    void clear(int width = 8, int height = 8);

    // copy texture contents from other image
    void copy(const image& other);

    void destroy();
    static void destroyOnce();

    void load(const char * path);
    void changeType(type newType);

    [[nodiscard]] int getWidth() const { return width_; }
    [[nodiscard]] int getHeight() const { return height_; }
    [[nodiscard]] int getNrChannels() const { return nrChannels_; }
    [[nodiscard]] float getAspectRatio() const { return aspectRatio_; }
    [[nodiscard]] GLenum getFormat() const { return format_; }
    [[nodiscard]] GLenum getType() const { return type_; }

private:
    static std::unique_ptr<shader> shader_s;

    void rescaleVBO() const;
    void rescaleFramebuffer(float width, float height);

    void typeRescaleScaledA(float newVertices[]) const;
    void typeRescaleScaledB(float newVertices[]) const;

    int width_ = 0,
        height_ = 0,
        nrChannels_ = 0;
    float aspectRatio_ = 1.0f;
    GLenum format_ = GL_RGBA;

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

const char * image::supportedFormats[5] = { "*.png", "*.jpg", "*.jpeg", "*.bmp", "*.gif" };
const int image::supportedFormatsNum = sizeof(supportedFormats) / sizeof(char *);
const char * image::types[2] = { "stretch", "scale" };
const int image::typesNum = 2;

std::unique_ptr<shader> image::shader_s;

float image::vertices_s[6 * 5] = {
    -1.0f, -1.0f, 0.0f,   0.0f,  0.0f,
    -1.0f,  1.0f, 0.0f,   0.0f,  1.0f,
     1.0f, -1.0f, 0.0f,   1.0f,  0.0f,

     1.0f, -1.0f, 0.0f,   1.0f,  0.0f,
     1.0f,  1.0f, 0.0f,   1.0f,  1.0f,
    -1.0f,  1.0f, 0.0f,   0.0f,  1.0f,
};

inline void image::initOnce()
{
    stbi_set_flip_vertically_on_load(true);

    shader_s = std::make_unique<shader>("sh.vert", "sh.frag");
}

inline void image::init()
{
    // texture
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

    glBindFramebuffer(GL_FRAMEBUFFER, FBO_);
    glBindTexture(GL_TEXTURE_2D, textureDisplayed_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureDisplayed_, 0);

    clear();

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

inline void image::display()
{
    // we access the ImGui window size
    const float width = ImGui::GetContentRegionAvail().x;
    const float height = ImGui::GetContentRegionAvail().y;

    // we rescale the framebuffer to the actual window size here and reset the glViewport
    rescaleFramebuffer(width, height);
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
}

inline void image::clear(int width, int height)
{
    width_ = width;
    height_ = height;
    aspectRatio_ = 1.0f;
    nrChannels_ = 4;
    format_ = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format_), width, height, 0, format_, GL_UNSIGNED_BYTE, nullptr);
    glClearTexImage(texture_, 0, format_, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

inline void image::copy(const image& other)
{
    width_ = other.width_;
    height_ = other.height_;
    nrChannels_ = other.nrChannels_;
    aspectRatio_ = other.aspectRatio_;
    format_ = other.format_;

    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format_), width_, height_, 0, format_, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint readFBO = 0, drawFBO = 0;
    glGenFramebuffers(1, &readFBO);
    glGenFramebuffers(1, &drawFBO);

    // Setup READ FBO with source texture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, readFBO);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, other.texture_, 0);
    GLenum status = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Read framebuffer incomplete");
    }

    // Setup DRAW FBO with destination texture
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFBO);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, texture_, 0);
    status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Draw framebuffer incomplete");
    }

    glBlitFramebuffer(0, 0, width_, height_,
                  0, 0, width_, height_,
                  GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &readFBO);
    glDeleteFramebuffers(1, &drawFBO);
}

inline void image::render() const
{
    // set viewport for the displayed image
    glViewport(0, 0, static_cast<GLsizei>(widthFB_), static_cast<GLsizei>(heightFB_));
    // clear before redrawing
    glBindFramebuffer(GL_FRAMEBUFFER, FBO_);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, texture_);
    shader_s->use();
    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glUseProgram(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

inline void image::destroy()
{
    glDeleteTextures(1, &texture_);
    texture_ = 0;

    glDeleteFramebuffers(1, &FBO_);
    FBO_ = 0;
    glDeleteTextures(1, &textureDisplayed_);
    textureDisplayed_ = 0;

    glDeleteBuffers(1, &VBO_);
    glDeleteVertexArrays(1, &VAO_);
}

inline void image::destroyOnce()
{
    shader_s.reset();
}

inline void image::load(const char * path)
{
    // loaded image texture
    unsigned char* data = stbi_load(path, &width_, &height_, &nrChannels_, 0);
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

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
        throw std::runtime_error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

inline void image::rescaleVBO() const
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
inline void image::rescaleFramebuffer(float width, float height)
{
    widthFB_ = static_cast<int>(width);
    heightFB_ = static_cast<int>(height);
    glBindTexture(GL_TEXTURE_2D, textureDisplayed_);
    glTexImage2D(GL_TEXTURE_2D, 0,  static_cast<GLint>(format_),
        static_cast<GLsizei>(widthFB_),
        static_cast<GLsizei>(heightFB_),
        0, format_, GL_UNSIGNED_BYTE, nullptr);
}

inline void image::changeType(type newType)
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

inline void image::typeRescaleScaledA(float newVertices[]) const
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

inline void image::typeRescaleScaledB(float newVertices[]) const
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


#endif //IMAGE_HPP
