//
// Created by Milan van Zanten on 09.02.18.
//

#include "Renderer.h"
#include "../io/FileLoader.h"
#include "../exception/Exception.h"
#include "../util/Log.h"
#include "model/MeshFactory.h"

Renderer::Renderer() {
    if(!glfwInit())
        throw Exception("Could not initialize glfw."); // NOLINT
}

static void glfwErrorCallback(int error, const char* description) {
    Log::log << LOG_ERROR << description << ". Code: " << error;
    throw Exception(description, error); // NOLINT
}

void Renderer::init(int width, int height) {
    glfwSetErrorCallback(glfwErrorCallback);

    // set window hints
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
#ifdef __APPLE__ // MacOS requires at least OpenGL version 3.2 core profile with forward compatibility
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
#ifdef BESTEST_GAME_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    // open a window and create its OpenGL context
    window = glfwCreateWindow(width, height, "bestest game", nullptr, nullptr);
    if(window == nullptr) {
        glfwTerminate();
        throw Exception("Could not open GLFW window."); // NOLINT
    }
    glfwMakeContextCurrent(window);

    // initialize glew
    glewExperimental = static_cast<GLboolean>(true); // necessary in core profile
    if(glewInit() != GLEW_OK)
        throw Exception("Could not initialize glew."); // NOLINT

    // VAO and vertex buffer
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // techniques
    lighting.init();

    // font
    font = new BitmapFont("assets/fonts/bitmap/font_bitmap.bmp", glm::vec2(8, 8), glm::vec2(16, 16),
                          {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                           'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5',
                           '6', '7', '8', '9', ':', ';', '#', '%', '+', '-', '/', '\\', '!', '?', '.', ',',
                           '\'', '*', '(', ')', '[', ']', '{', '}', '<', '>', '"', '=', ' ', '_', '|', '\0'});

    // options
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    initialized = true;
}

void Renderer::renderScene(Scene *scene, long long int lag) {
    if(!initialized)
        throw NotInitialisedException("Renderer");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOLINT

    lighting.enable();
    lighting.setDirectionalLights(scene->directionalLights.size(), scene->directionalLights);
    lighting.setPointLights(scene->pointLights.size(), scene->pointLights);
    lighting.setSpotLights(scene->spotLights.size(), scene->spotLights);
    lighting.setView(scene->activeCamera->getView());
    for(auto entity: scene->entities)
        entity->getModel()->render(lighting, scene->activeCamera->getProjection()*scene->activeCamera->getView());

    glfwSwapBuffers(window);
}

GLFWwindow* Renderer::getWindow() const {
    return window;
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &vertexArrayID);

    glfwTerminate();
}
