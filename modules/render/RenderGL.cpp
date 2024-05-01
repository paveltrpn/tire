
#include <GL/glx.h>
#include <memory>
#include <string>
#include <iostream>
#include <format>
#include <print>

#include "config/Config.h"
#include "Render.h"
#include "RenderGL.h"

#include "render/GLFunctions.h"
#include "spdlog/spdlog.h"

#include <glm/gtc/matrix_transform.hpp>

namespace tire {

RenderGL::RenderGL(const tire::Config& config) : Render{ config } {
    configureGl();
    // setup VSYNC
    setSwapInterval(1);

    gl = std::make_shared<GLFunctions>();
    gl->initGLFunctions();

    linkProgram();
}

RenderGL::~RenderGL() {
    glXDestroyContext(display_, glContext_);
}

void RenderGL::configureGl() {
    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(&MessageCallback, nullptr);
    
    __detail_tire::ctxErrorOccurred = false;
    int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&__detail_tire::ctxErrorHandler);

    // Check for the GLX_ARB_create_context extension string and the function.
    // If either is not present, use GLX 1.3 context creation method.
    std::array<int, 5> context_attribs;
    context_attribs[0] = GLX_CONTEXT_MAJOR_VERSION_ARB;
    context_attribs[2] = GLX_CONTEXT_MINOR_VERSION_ARB;
    if (config_.get<bool>("use_maximum_context_version", true)) {
        // this parameters force X11 to use higher context among the possible
        context_attribs[1] = 3;
        context_attribs[3] = 0;
    } else {
        // or use user defined context version with 3.3 by default
        context_attribs[1] = config_.get<int>("use_context_version_major", 3);
        context_attribs[3] = config_.get<int>("use_context_version_minor", 3);
    }
    context_attribs[4] = GLX_CONTEXT_PROFILE_MASK_ARB;
    context_attribs[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
    context_attribs[6] = None;

    glContext_
      = glXCreateContextAttribsARB(display_, bestFbc_, nullptr, True, context_attribs.data());

    // error ocured
    if (__detail_tire::ctxErrorOccurred && glContext_) {
        throw std::runtime_error("can't create modern OpenGL context!");
    }

    // Sync to ensure any errors generated are processed.
    XSync(display_, False);

    // Restore the original error handler
    XSetErrorHandler(oldHandler);

    if (__detail_tire::ctxErrorOccurred || !glContext_) {
        throw std::runtime_error("failed to create an OpenGL context");
    }

    // Verifying that context is a direct context
    if (!glXIsDirect(display_, glContext_)) {
        spdlog::info("indirect GLX rendering context obtained");
    } else {
        spdlog::info("direct GLX rendering context obtained");
    }

    glXMakeCurrent(display_, window_, glContext_);

    vendor_ = (const char*)glGetString(GL_VENDOR);
    renderer_ = (const char*)glGetString(GL_RENDERER);
    glVersion_ = (const char*)glGetString(GL_VERSION);
    glslVersion_ = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}

void RenderGL::displayRenderInfo() {
    spdlog::info(
      "OpenGL context info\nvendor - {}\nrenderer - {}\nOpenGL version - {}\nGLSL version - {}",
      vendor_,
      renderer_,
      glVersion_,
      glslVersion_);
}

void RenderGL::preFrame() {
    glViewport(0, 0, width_, height_);
    glClearColor(0, 0.5, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderGL::postFrame() {
}

void RenderGL::setupDebugMessages() {
}

void RenderGL::swapBuffers() {
    glXSwapBuffers(display_, window_);
};

void RenderGL::appendToRenderList(std::shared_ptr<tire::Node<point_scalar_type>> node) {
    renderList_.push_back(node);

    gl->GenBuffers(1, &bufferObject_);
    gl->GenVertexArrays(1, &vertexObject_);

    gl->BindBuffer(GL_ARRAY_BUFFER, bufferObject_);
    gl->BindVertexArray(vertexObject_);
    gl->BufferData(GL_ARRAY_BUFFER, node->getVerteciesArraySize(), nullptr, GL_DYNAMIC_DRAW);
    // position attribute
    gl->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    gl->EnableVertexAttribArray(0);

    gl->BindBuffer(GL_ARRAY_BUFFER, bufferObject_);
    gl->BindVertexArray(vertexObject_);
    gl->BufferSubData(GL_ARRAY_BUFFER, 0, node->getVerteciesArraySize(), node->getVerteciesData());
    // color attribute
    // gl->VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)(12 * sizeof(float)));
    // gl->EnableVertexAttribArray(1);
    // texture coordinates attribute
    // gl->VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *)(24 * sizeof(float)));
    // gl->EnableVertexAttribArray(2);
}

void loadShader(std::string fname) {
    std::string shader;
    std::ifstream File;
    File.open(fname, std::ios::in);

    if (File) {
        shader.assign(std::istreambuf_iterator<char>(File), std::istreambuf_iterator<char>());
    }

    std::cout << shader << "\n";
}

std::vector<GLuint> RenderGL::appendShader(std::vector<std::tuple<GLuint, std::string>> shaders) {
    if (shaders.size() < 2) {
        spdlog::critical(" RenderGL::appendShader(): ERROR! Wrong shader count!");
        std::exit(1);
    } else {
        std::vector<GLuint> rt;
        for (const auto &[type, source] : shaders) {
            // Читаем файл в одну строку за раз
            std::ifstream inFile;

            GLuint shHandle = gl->CreateShader(type);

            const char *c_str = source.c_str();

            gl->ShaderSource(shHandle, 1, &c_str, nullptr);
            gl->CompileShader(shHandle);

            GLint success;
            gl->GetShaderiv(shHandle, GL_COMPILE_STATUS, &success);

            if (success == GL_FALSE) {
                int32_t logLength;
                gl->GetShaderiv(shHandle, GL_INFO_LOG_LENGTH, &logLength);

                std::shared_ptr<GLchar[]> log(new GLchar[logLength]);
                gl->GetShaderInfoLog(shHandle, logLength, nullptr, log.get());

                spdlog::critical(
                  "COglProgram::appendShader(): ERROR! Can't compile shader with trace:\n{}",
                  log.get());
                inFile.close();
                std::exit(1);
            }

            rt.push_back(shHandle);
        }
        return rt;
    }
}

void RenderGL::linkProgram() {
    std::string vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 pos;\n"
                                     "out vec3 outColor;\n"
                                     "uniform mat4 matrix;\n"
                                     "void main() {\n"
                                     "   outColor = vec3(1.0f, 0.0f, 0.0f);\n"
                                     "   gl_Position = matrix * vec4(pos, 1.0);\n"
                                     "}\n";

    std::string fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "in vec3 outColor;\n"
                                       "void main() {\n"
                                       "    FragColor = vec4(outColor, 0.5);\n"
                                       "    //vec4 texColor = texture(texture1, texCoord);\n"
                                       "    //if(texColor.a < 0.1)\n"
                                       "    //     discard;\n"
                                       "    //FragColor = texColor;\n"
                                       "}\n";

    auto shaderList = appendShader(
      { { GL_VERTEX_SHADER, vertexShaderSource }, { GL_FRAGMENT_SHADER, fragmentShaderSource } });

    programObject_ = gl->CreateProgram();

    for (const auto &shHandle : shaderList) {
        gl->AttachShader(programObject_, shHandle);
    }

    gl->LinkProgram(programObject_);

    GLint success;
    gl->GetProgramiv(programObject_, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) {
        int32_t logLength;
        gl->GetProgramiv(programObject_, GL_INFO_LOG_LENGTH, &logLength);

        std::shared_ptr<GLchar[]> log(new GLchar[logLength]);
        gl->GetProgramInfoLog(programObject_, logLength, nullptr, log.get());

        spdlog::critical("RenderGL::linkProgram(): ERROR! Can't link program with trace:\n{}",
                         log.get());
        std::exit(1);
    }

    gl->UseProgram(programObject_);
    auto matrix = gl->GetUniformLocation(programObject_, "matrix");
    glm::mat4 projection = glm::perspective(
      50.0f, static_cast<float>(width_) / static_cast<float>(height_), 0.1f, 100.0f);
    glm::mat4 offset = glm::translate(projection, glm::vec3(0.0f, 0.0f, -15.0f));
    gl->UniformMatrix4fv(matrix, 1, GL_FALSE, &offset[0][0]);
}

void RenderGL::traverse() {
    gl->EnableVertexAttribArray(0);
    gl->BindVertexArray(vertexObject_);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    gl->DisableVertexAttribArray(0);
};

}  // namespace tire
