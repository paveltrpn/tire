
#include <format>

#include "mainApp.h"

#include "bitmap.h"
#include "mesh.h"
#include "timer.h"
#include "vec4.h"

GLenum check_gl_error(const char* file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
            error = "STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            error = "STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define check_gl_error() check_gl_error(__FILE__, __LINE__)

/* ========================================= */
/* ============ mainApp_glfw_c ============== */
/* ========================================= */

void mainApp_glfw_c::setup() {
    vec4 ambient = vec4{ 0.5, 0.5, 0.5, 1 };
    vec4 diffuse = vec4{ 2, 2, 2, 2 };
    vec4 lightPosition = vec4{ 0, 0, 10, 1 };
    bitmap_c tex_image;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glLightfv(GL_LIGHT0, GL_AMBIENT, &ambient[0]);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, &diffuse[0]);
    glLightfv(GL_LIGHT0, GL_POSITION, &lightPosition[0]);
    glEnable(GL_LIGHT0);

    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    tex_image.load_jpg("../../../assets/textures/texture.jpg");
    tex_image.show_info();
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 3,
                 tex_image.get_widht(),
                 tex_image.get_height(),
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 tex_image.get_data_ptr());
    // gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex_image.get_widht(), tex_image.get_height(), GL_RGB,
    // GL_UNSIGNED_BYTE, tex_image.get_data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    text.load_font("../../../assets/img_fonts/RobotoMono-2048-1024-64-128.tga");

    box.append(vec3(-4.0, 0.0, 0.0), mtrx3FromEuler(0.01, -0.02, 0.02));
    box.append(vec3(3.0, 6.0, 0.0), mtrx3FromEuler(0.01, -0.01, 0.03));
    box.append(vec3(-4.0, -6.0, 4.0), mtrx3FromEuler(0.01, 0.02, -0.01));
    box.append(vec3(-7.0, -5.0, 2.0), mtrx3FromEuler(0.02, 0.015, 0.01));
    box.append(vec3(7.0, 5.0, 3.0), mtrx3FromEuler(-0.02, -0.01, -0.02));
    box.append(vec3(-8.0, 6.0, 0.0), mtrx3FromEuler(-0.02, 0.01, 0.01));

    BodyList.push_back(
      Body(Body::PRISM, vec3(6.0f, -4.5f, -2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(3.5f, 4.0f, 2.0f)));
    BodyList.push_back(
      Body(Body::BOX, vec3(-6.0f, 3.0f, -2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(3.0f, 2.5f, 0.2f)));
    BodyList.push_back(Body(
      Body::ICOSAHEDRON, vec3(2.0f, 0.0f, -2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(4.0f, 4.0f, 4.0f)));

    camera.setCameraPosition(vec3(0.0f, 0.0f, -20.0f));
    camera.updatePosition();
}

void mainApp_glfw_c::init_app(int argc, char* argv[]) {
    if (glfwInit() != GLFW_TRUE) {
        std::cout << "mainApp_glfw_c::init_app(): glfwInit() return - GLFW_FALSE!"
                  << "\n";
        exit(1);
    }

    auto errorCallback
      = [](int, const char* err_str) { std::cout << "GLFW Error: " << err_str << std::endl; };
    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // В VirtualBox с OpenGL 2.1 выбрасывает ошибку
    // "Context profiles are only defined for OpenGL version 3.2 and above"
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(x_res, y_res, appName.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "mainApp_glfw_c::init_app(): Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    };

    aspect = static_cast<float>(x_res) / static_cast<float>(y_res);

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cout << "mainApp_glfw_c::init_app(): Failed to initialize GLEW" << std::endl;
        exit(1);
    };

    // Выключаем вертикальную синхронизацию (VSYNC)
    glfwSwapInterval(0);

    auto keyCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        if (key == GLFW_KEY_E && GLFW_PRESS) {
            std::cout << std::format("Key E pressed\n");
        }
    };
    glfwSetKeyCallback(window, keyCallback);

    glfwSetTime(110.0);

    gl_render = (const char*)glGetString(GL_RENDERER);
    gl_version = (const char*)glGetString(GL_VERSION);
    glsl_version = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

    std::cout << std::format(
      "GL_RENDER - {}\nGL_VERSION - {}\nGLSL_VERSION - {}\n", gl_render, gl_version, glsl_version);

    setup();
}

vec3 orn = {};

void mainApp_glfw_c::looper() {
    while (!glfwWindowShouldClose(window)) {
        glfwGetCursorPos(window, &curPositionX, &curPositionY);

        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // gluPerspective(45.0f, aspect, 0.1f, 100.0f);

        glLoadMatrixf(camera.getCmrMatrixPointer());
        // glTranslatef(0.0f, 0.0f, -20.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // ---------------
        // Отрисовка кубов
        // ---------------
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_LIGHTING);

        glEnable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, texture);
        box.show();
        // ---------------

        // ---------------
        // Отрисовка тел из BodyList
        // ---------------
        orn = vec3Sum(orn, vec3(0.01f, 0.02f, 0.001f));
        for (auto& bdy : BodyList) {
            bdy.setOrientation(orn[0], orn[1], orn[2]);
            bdy.updateAndDraw();
        }
        // ---------------

        // ----------------
        // Отрисовка текста
        // ----------------
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        text.set_text_position(-10.0f, 8.0f);
        text.draw_string(std::format("cursor X = {}, cursor Y = {}", curPositionX, curPositionY));

        text.set_text_position(-10.0f, 7.0f);
        text.draw_string(std::format("fps = {}", fps));

        text.set_text_position(-10.0f, 6.0f);
        text.draw_string(std::format("glfwGetTime = {}", glfwGetTime()));
        // ---------------

        frames++;
        auto [watch, cdelta] = timer.watch_millisec(2000);
        if (watch) {
            fps = (float)frames / cdelta * 1000;
            frames = 0;
        }
        //

        glFlush();
        glfwSwapBuffers(window);
    }
}

mainApp_glfw_c::~mainApp_glfw_c() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
